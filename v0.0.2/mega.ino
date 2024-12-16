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
unsigned short delay_time=1000, delay_get_data=2000;
String heartbeats_r="-1", oxygen_r="-1", weight_r="-1", temperature_r="-1", flg="";

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

}

void loop() {

ReadServer(); // check connect to server
delay(delay_time);
print_flag(); // get flag value
flg = get_data("xflag=", "endf");
Serial.println(flg);

if (flg == "1") { // access to collect data from another Microcontroller
  Serial.println("---------------<< Flag 1 >>---------------");
  bool status[3];
  for (int i=0; i<3; i++) status[i]=true; // true: yet not get data 

//   String ht = get_data("xheight=", "endh");
//   if (result == "true"){
  if (true){ 
    start_time = millis();
    while (true){
    delay(delay_get_data);
    Serial.println("---------------<< getting data >>---------------");

    // -------------- weight --------------
    if (status[0]){
      read_uart("weight");
      weight_r = get_data("weight=", "endw");
      if (weight_r != "-1") status[0] = false;
    }
    // ------------------------------------    

    // -------------- BPN --------------
    if (status[1]){
      read_uart("weight"); // for test *********************************************************
      // read_uart("bpn");
      heartbeats_r = get_data("heartbeat=", "endhb");
      oxygen_r = get_data("oxygen=", "endo");
      if ((heartbeats_r != "-1") && (oxygen_r != "-1")) status[1] = false;
    }
    // ---------------------------------

    // -------------- temperature --------------
    // cancelled
    temperature_r = "0"; status[2] = false; // for test
    // -----------------------------------------

    if (true ^ (status[0] || status[1] || status[2])){ // ^ == XOR
      Serial.print("Heartbeats: ");    Serial.print(heartbeats_r);
      Serial.print(", Oxygen: ");      Serial.print(oxygen_r);
      Serial.print(", Weight: ");      Serial.print(weight_r);
      Serial.print(", Temperature: "); Serial.println(temperature_r);

      break;
    }
    end_time = millis();
    Serial.print("\n\t\t\tTIMER: ");
    Serial.print(end_time - start_time);

    if (end_time - start_time > delay_get_data * 10) break;

    }
  }
  if (true ^ (status[0] || status[1] || status[2])){
    Write2Server(heartbeats_r, oxygen_r, weight_r, temperature_r);
    Write2Flag();
  }
}
delay(delay_time);
heartbeats_r="-1"; oxygen_r="-1"; weight_r="-1"; temperature_r="-1";
flg="";request="";
}

void ReadServer() {
  // request = "";
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
   
    Serial.println("connecting...");
    client.println("GET /sql_personal_data/read_variables_from_server.php? HTTP/1.1");
    client.println("Host: 192.168.2.50");
    client.println("Connection: close");
    client.println();

  } else {
    Serial.println("connection failed");
  }
}

void Write2Server(String heartbeat, String oxygen, String weight_kg, String temperature) {
   
      if(client.connect(server, 80)){
        Serial.println("connected R1");
        Serial.println("Uploading....       ");
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

        client.stop(); //Closing the connection
      }
      else {
        // if you didn't get a connection to the server:
        Serial.println("Upload Failed!!!!! ");
      }
 }

void Write2Flag() {
   
    if(client.connect(server, 80)){
    Serial.println("connected");
    Serial.println("Uploading....");
    client.print("GET /sql_personal_data/write_flag_to_server.php?"); //Connecting and Sending values to database

    client.stop(); //Closing the connection
    }
    else {
    // if you didn't get a connection to the server:
    Serial.println("Upload Failed (write2flag)");
    }
  }

String get_data(String start, String end) {
  if (request.length()<10){
      Serial.print("Waiting for Server..");
      }else{
  String H_Rel = start;
  int Relstart = request.indexOf(H_Rel);
  String F_Rel = end;
  int Relstop = request.indexOf(F_Rel);
  String RelStatus = request.substring(Relstart+H_Rel.length(), Relstop);
  Serial.print("RelStatus: ");
  Serial.println(RelStatus);
  return RelStatus;

  }
}

void read_uart(String option) {
  
  //                +-----------------------+
  //                | Serial 1: Weight      |
  //                | Serial 2: BPN         |
  //                | Serial 3: Temperature |
  //                +-----------------------+

  // ------------------------ weight ------------------------
  if (option == "weight")
    if(Serial1.available()){
        String msg = Serial1.readStringUntil('\n');
        Serial.println("Weight from Serial1: ");
        Serial.println(msg);
    }
    else Serial.print("\tSerial1 is not available!!! >> 1");
  // --------------------------------------------------------

  // ------------------------- BPN --------------------------
  else if (option == "bpn")
    if(Serial2.available()){
        String msg = Serial2.readStringUntil('\n');
        Serial.println("bpn from Serial2: "); 
        Serial.println(msg);
    }
    else Serial.print("\tSerial2 is not available!!! >> 2");
  // --------------------------------------------------------
  
  // --------------------- temperature ----------------------
  else if (option == "temperature")
    if(Serial3.available()){
        String msg = Serial3.readStringUntil('\n');
        Serial.println("temperature from Serial3: "); 
        Serial.println(msg);
    }
    else Serial.print("\tSerial3 is not available!!! >> 3");
  // --------------------------------------------------------

  ReadServer();
}
