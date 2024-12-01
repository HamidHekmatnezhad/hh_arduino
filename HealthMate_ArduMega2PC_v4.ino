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
unsigned short delay_time=1000;
unsigned int heartbeat;
float temperature, weight, oxygen;
String result;

// Define TX and RX pins for UART 
// conncetion 1 
// #define TXD1 19
// #define RXD1 21
// HardwareSerial Serial1(1);

// connection 2
// #define TXD2 18
// #define RXD2 20
// HardwareSerial Serial_conn2(2);

// coccention 3
// #define TXD3 17
// #define RXD3 16
// HardwareSerial Serial_conn3(3);

void setup() {

  Serial.begin(9600);
  Serial.println(F("Initialize System"));

  Ethernet.begin(mac, ip);

  Serial.println(F("W5100 initialized"));
  Serial.print(F("IP Address: "));
  Serial.println(Ethernet.localIP());
  pinMode(30,OUTPUT);

  // set up for UART 
  // Serial_conn1.begin(115200, SERIAL_8N1, RXD1, TXD1); // bayad serail port tarrif beshe baraye har esp
  // Serial_conn2.begin(404, SERIAL_8N1, RXD2, TXD2);
  // Serial_conn3.begin(404, SERIAL_8N1, RXD3, TXD3;

}

void loop() {

ReadServer(); // check connect to server
delay(delay_time);
print_flag(); // get flag value
get_data("xflag=", "endf");
Serial.print(result);

if (result == "1") { // access to collect data from another Microcontroller
  // GetData_esp32_1();
  Serial.println("Hi");

  // Write2Server(client, heartbeat, oxygen, temperature, weight); // TODO mishe vasashon ye return tarif kard ke beshe bahash log va ro ye display namayesh dad.
  // Write2Flag(client); // TODO "
}
delay(delay_time);
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

void print_flag() { // read flag from server  --Done--
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


void Write2Server(EthernetClient client, int heartbeat, float oxygen, float temperature, float weight) { // --Done--
    
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
      client.print("&sens_5=");
      client.println(999);

      client.stop(); //Closing the connection
      }
      else {
      // if you didn't get a connection to the server:
      Serial.println("Upload Failed       ");
      }
 }

void Write2Flag(EthernetClient client) { // --Done--
    
       if(client.connect(server, 80)){
       Serial.println("connected R2");
       Serial.println("Uploading....   +++++++++++++++++++++++    ");
       client.print("GET /sql_personal_data/write_flag_to_server.php?"); //Connecting and Sending values to database

       client.print("flagg=");
       client.println(0);
       
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


 void LCDdispServ () {

  //Serial.print("Request Length = ");
  //Serial.println(request.length());
  if (request.length()<10){
      Serial.print("Waiting for Server..");
      }else{
  String H_Rel = "xflag=";
  int Relstart = request.indexOf(H_Rel);
  String F_Rel = "endf";
  int Relstop = request.indexOf(F_Rel);
  String RelStatus = request.substring(Relstart+H_Rel.length(), Relstop);
  Serial.print("RelStatus: ");
  Serial.println(RelStatus);
  if ((RelStatus== "1")) {
     digitalWrite(30, HIGH);
    //  Write2Server(client);
     delay(2000);
    //  Write2Flag(client);
     }
if (RelStatus== "0") {
     digitalWrite(30, LOW);
    }
    
   }
 }

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