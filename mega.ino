#include <SPI.h>
#include <Ethernet.h>
//Parameters
String request="";
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//Objects
IPAddress ip(192,168,2,51); //IP address for your arduino.
char server[] = "192.168.2.50"; //IP address of your computer.
EthernetClient client;

// variables
unsigned long end_time, start_time;
unsigned short delay_time=3000, delay_get_data=1000, late=60000;
String heartbeats_r="-1", oxygen_r="-1", weight_r="-1", temperature_r="-1", flg="", height_r="-1";
int counter=0, cc=5;
int pin_esp_blue=46, pin_esp_w=47;

// this data, setting for serial port read DATA
int wsp = 1; // for weight.
int hsp = 2; // for hearthbeats
int osp = 2; // for oxygen
int tsp = 0; // for tempreture


void setup() {


  Serial.begin(9600);
  // serial for uart
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);

  Serial.println(F("Initialize System"));

  Ethernet.begin(mac, ip);

  Serial.println(F("W5100 initialized"));
  Serial.print(F("IP Address: "));
  Serial.println(Ethernet.localIP());
  pinMode(30,OUTPUT);
 
  pinMode(pin_esp_blue, OUTPUT);
  pinMode(pin_esp_w, OUTPUT);

}

void loop() {

ReadServer(); // check connect to server
delay(delay_time);
print_flag();
flg = get_data("xflag=", "endf");
Serial.println(flg);
// Write2Server(client, "33", "33", "33", "33"); // for test

if (flg == "1") { // access to collect data from another Microcontroller
  Serial.println("---------------<< Flag 1 >>---------------");
  bool status[4];
  for (int i=0; i<4; i++) status[i]=true; // true: yet not get data

  if (true){
    start_time = millis();

    // Enable esp bluetooth
    digitalWrite(pin_esp_blue, LOW);
    delay(100);
    digitalWrite(pin_esp_blue, HIGH);

    // Enable esp weight
    digitalWrite(pin_esp_w, LOW);
    delay(100);
    digitalWrite(pin_esp_w, HIGH);
   
   
    while (true){
    delay(delay_get_data);
    Serial.println("---------------<< getting data >>---------------");

    // -------------- weight (1) --------------
    counter = 0;
    while (status[0] && (counter<cc)){
      weight_r = read_uart(wsp, "weight=", "endw");
      if (weight_r != "-1") {status[0] = false;Serial.println("++++++++++++ " + weight_r);
        break;}
    counter++;
    }
    // ----------------------------------------    

    // digitalWrite(pin_esp_blue, LOW);
    // delay(100);
    // digitalWrite(pin_esp_blue, HIGH);
    
    // -------------- BPN (2) --------------
    counter = 0;
    while (status[1] && (counter<cc)){
      heartbeats_r = read_uart(hsp, "heartbeat=", "endhb");
      if (heartbeats_r != "-1") {status[1] = false;break;}
    counter++;
    }

   
    digitalWrite(pin_esp_blue, LOW);
    delay(100);
    digitalWrite(pin_esp_blue, HIGH);
    counter = 0;
    while (status[2] && counter<cc){
      oxygen_r = read_uart(osp, "oxygen=", "endo");
      if (oxygen_r != "-1") {status[2] = false;break;}
    counter++;
    }
    // -------------------------------------

    // -------------- temperature (3) --------------
    // cancelled
    temperature_r = "0"; status[3] = false; // for test
    // ---------------------------------------------

    Serial.print("Heartbeats: ");    Serial.print(heartbeats_r);
    Serial.print(", Oxygen: ");      Serial.print(oxygen_r);
    Serial.print(", Weight: ");      Serial.print(weight_r);
    Serial.print(", Temperature: "); Serial.println(temperature_r);
     

    if (true ^ (status[0] || status[1] || status[2] || status[3])) break; // ^ == XOR
   
    end_time = millis();
    Serial.print("\n\t\t\tTIMER: ");
    Serial.print((end_time - start_time)/1000);
    Serial.print('\n');

    if (end_time - start_time > late) {Write2Flag(client);break;}

    }
  }
  if (true ^ (status[0] || status[1] || status[2] || status[3])){
    Write2Server(client, heartbeats_r, oxygen_r, weight_r, temperature_r);
    Write2Flag(client);
  }
}
delay(delay_time);
heartbeats_r="-1"; oxygen_r="-1"; weight_r="-1"; temperature_r="-1";
flg="";request="";Serial.println("-------------- end -----------");
}

void ReadServer() {
    while (client.available()) {
    char c = client.read();
    String mystring(c);
    request+=mystring;
  }
 
  Serial.print("request: ");
  Serial.println(request);
}

void print_flag() {
  // read flag from server  
  client.stop();
  if (client.connect(server, 80)) {
   
    Serial.println("connecting... (print_flag)");
    client.println("GET /sql_personal_data/read_variables_from_server.php? HTTP/1.1");
    client.println("Host: 192.168.2.50");
    client.println("Connection: close");
    client.println("");
//    client.stop();

  } else {
    Serial.println("connection failed - (print_flag)");
  }
}

void Write2Server(EthernetClient client, String heartbeat, String oxygen, String weight_kg, String temperature) {

      client.stop();
      if(client.connect(server, 80)){
        Serial.println("Uploading.... (Write2Server)");
        client.print("GET /sql_personal_data/write_variables_to_server.php?"); //Connecting and Sending values to database

        // +----------------------+
        // | sens_1 = hearthbeat  |
        // | sens_2 = oxygen      |
        // | sens_3 = weight_kg   |
        // | sens_4 = temperature |
        // | sens_5 = None        |
        // +----------------------+
     
        // send data
        client.print("sens_1=");
        client.print(heartbeat);
        client.print("&sens_2=");
        client.print(oxygen);
        client.print("&sens_3=");
        client.print(weight_kg);
        client.print("&sens_4=");
        client.print(temperature);
        // client.print("&sens_5=");
        // client.println(999);
       
//        client.println("Host: 192.168.2.50");
//        client.println("Connection: close");
       
        client.println();

        client.stop(); //Closing the connection
      }
      else {
        // if you didn't get a connection to the server:
        Serial.println("Upload Failed!!!!! - (write2server)");
      }
 }

void Write2Flag(EthernetClient client) {

    client.stop();
    if(client.connect(server, 80)){
    Serial.println("Uploading.... (Write2Flag)");
    client.print("GET /sql_personal_data/write_flag_to_server.php?"); //Connecting and Sending values to database
    client.println(" "); // this is important

    client.stop(); //Closing the connection
    }
    else {
    // if you didn't get a connection to the server:
    Serial.println("Upload Failed (write2flag)");
    }
  }

String get_data(String start, String end) {
 
  String H_Rel = start;
  int Relstart = request.indexOf(H_Rel);
  String F_Rel = end;
  int Relstop = request.indexOf(F_Rel);
  String RelStatus = request.substring(Relstart+H_Rel.length(), Relstop);
  Serial.print("RelStatus: ");
  Serial.println(RelStatus);
  if (RelStatus != "-1") return RelStatus;
}

String read_uart(int serial_port, String startStr, String endStr) {

  String data="", ans="";
//  Serial1.flush(); Serial2.flush(); Serial3.flush();

  switch (serial_port)
  {
  case 1:
    Serial1.begin(9600);
    data = Serial1.readStringUntil('\n');
    Serial1.end();
    break;
 
  case 2:
    Serial2.begin(9600);
    data = Serial2.readStringUntil('\n');
    Serial2.end();
    break;
 
  case 3:
    Serial3.begin(9600);
    data = Serial3.readStringUntil('\n');
    Serial3.end();
    break;
   
  }
 
  Serial.print(serial_port);
  Serial.println(">>>>>>>>>>>>>>> " + data);
  int s = data.indexOf(startStr) + startStr.length() ;
  int e = data.indexOf(endStr);
  ans = data.substring(s, e);

 
  if (ans == "") return "-1";
  else return ans;
}