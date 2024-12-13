#include <SPI.h>
#include <Ethernet.h>
//Parameters
String request="";
bool Flag = true;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//Objects
IPAddress ip(192,168,2,51); //IP address for your arduino.
char server[] = "192.168.2.50"; //IP address of your computer.
// char server[] = "0.0.0.0";
EthernetClient client;

// variables 
String flg;
unsigned long end_time, start_time;
unsigned short delay_time=1000, delay_get_data=3000;
unsigned int heartbeat;
float temperature, weight, oxygen;
String result;
String heartbeats_r="-1", oxygen_r="-1", weight_r="-1", temperature_r="-1";

void setup() {

  Serial.begin(9600);

  // serial for uart

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
get_data("xflag=", "endf");
Serial.print(result);

if (result == "1") { // access to collect data from another Microcontroller
  Serial.println("Hi");

  // Get data from another microcontroller
  // 1. check: is person there
  // 2. a delay for 3 seconds
  // 3. get data: heartbeat and oxygen
  // 4. get data: weight
  // 5. get data: temperature --cancelled--

  get_data("xheight=", "endh");
  if (result == "true"){
    start_time = milist();
    while (true){
    delay(delay_get_data);
    
    // -------------- BPN --------------
    read_uart("bpn");
    // ReadServer(); // bayad dobare serial port khonde beshe. to khode func read_uart hastesh --check beshe--
    get_data("heartbeat=", "endhb");
    heartbeats_r = result;
    get_data("oxygen=", "endo");
    oxygen_r = result;
    // ---------------------------------

    // -------------- weight --------------
    // age serial port khast bayad ezafe beshe
    get_data("weight=", "endw");
    weight_r = result;
    // ------------------------------------

    // -------------- temperature --------------
    // cancelled 
    temperature_r = "0";

    // -----------------------------------------

    if (heartbeats_r != "-1" && oxygen_r != "-1" && weight_r != "-1" && temperature != "-1"){
      Serial.print("Heartbeats: ");
      Serial.print(heartbeats_r);
      Serial.print(", Oxygen: ");
      Serial.print(oxygen_r);
      Serial.print(", Weight: ");
      Serial.print(weight_r);
      Serial.print(", Temperature: ");
      Serial.print(temperature_r);

      break;
    }
    end_time = milist();

    if (end_time - start_time > delay_get_data * 10){
      break;
    }
    }
  }

  if (heartbeats_r != "-1" && oxygen_r != "-1" && weight_r != "-1" && temperature != "-1"){
  Write2Server(heartbeats_r, oxygen_r, weight_r, temperature_r);
  Write2Flag();
  }
  // for test

  result = "0";
}
delay(delay_time);
heartbeats_r="-1", oxygen_r="-1", weight_r="-1", temperature_r="-1";
request="";
}

void ReadServer() {
    while (client.available()) {
    char c = client.read();
    String mystring(c);
    request+=mystring;
  }
  Serial.print("Flag = ");
  Serial.println(request);

}

void print_flag() { // read flag from server  
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

void Write2Server(int heartbeat, float oxygen, float weight_kg, float temperature) { 
    
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
      
      // testing
      // heartbeat = 80; oxygen = 95.0; temperature = 36.5; weight = 70.0;

      // send data
      client.print("sens_1=");
      client.print(heartbeat);
      client.print("&sens_2=");
      client.print(oxygen);
      client.print("&sens_3=");
      client.print(weight);
      client.print("&sens_4=");
      client.print(temperature);
      // client.print("&sens_5=");
      // client.println(999);

      client.stop(); //Closing the connection
      }
      else {
      // if you didn't get a connection to the server:
      Serial.println("Upload Failed       ");
      }
 }

void Write2Flag() { // --Done--
    
    if(client.connect(server, 80)){
    Serial.println("connected R2");
    Serial.println("Uploading....   +++++++++++++++++++++++    ");
    client.print("GET /sql_personal_data/write_flag_to_server.php?"); //Connecting and Sending values to database

    //  client.print("flagg=");
    //  client.println(0);

    client.stop(); //Closing the connection
    }
    else {
    // if you didn't get a connection to the server:
    Serial.println("Upload Failed       ");
    }
  }

// void GetData_esp32_1(void) { // bayd rosh kar beshe  --Done-- 
//   if (Serial_conn1.available()) {
//     String data = Serial_conn1.readStringUntil('\n'); // 'end'
//     if (data.indexOf("sensor") >= 0) {
//       // read data from serial
//       // String sens_1 = data.substring(data.indexOf("sens_1=") + 7, data.indexOf("sens_2="));
//       return data;
//     }
//   }
// }

void get_data(String start, String end) {
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
  result = RelStatus;
  }
}

void read_uart(String option) {
  // faghat maghadir az serial port migire va roye serial port 0 minivise 
  // khondane etelaat az func get_data hastesh
  switch (option) {
    case "bpn":
      // az serial khodesh mikhonim bad ro serial print mikonim
      // felan malom nist serial ha
      break;
    
    case "temp":
      // az serial khodesh mikhonim bad ro serial print mikonim 
      // felan malom nist serial ha
      // va bakhshe tempreture ghate
      break;

    case "weight":
      // agar niyaz be uart dasht
      break;
    }
  ReadServer(); 
  }  