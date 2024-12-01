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

void setup() {

 Serial.begin(9600);
 Serial.println(F("Initialize System"));
 
 Ethernet.begin(mac, ip);
 
 Serial.println(F("W5100 initialized"));
 Serial.print(F("IP Address: "));
 Serial.println(Ethernet.localIP());
 pinMode(30,OUTPUT);
}

void loop() {

ReadServer();
delay(1000);
httpRequest();
LCDdispServ();
delay(1000);

//Write2Server(client);
//Write2Flag(client);
delay(1000);
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

void httpRequest() {
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

void Write2Server(EthernetClient client) {
    
       if(client.connect(server, 80)){
       Serial.println("connected R1");
       Serial.println("Uploading....       ");
       client.print("GET /sql_personal_data/write_variables_to_server.php?"); //Connecting and Sending values to database

//       client.print("hearthbeat=");
//       client.print(15);
//       client.print("&oxygen=");
//       client.print(453);
//       client.print("&weight_kg=");
//       client.print(98);
//       client.print("&temperature=");
//       client.print(345);

/////////////////////////
// sens_1 = hearthbeat
// sens_2 = oxygen
// sens_3 = weight_kg
// sens_4 = temperature
/////////////////////////
       client.print("sens_1=");
       client.print(105);
       client.print("&sens_2=");
       client.print(98.2);
       client.print("&sens_3=");
       client.print(67.4);
       client.print("&sens_4=");
       client.print(36.5);
       client.print("&sens_5=");
       client.println(664);
       
       client.stop(); //Closing the connection
       }
       else {
       // if you didn't get a connection to the server:
       Serial.println("Upload Failed       ");
       }
 }

 void Write2Flag(EthernetClient client) {
    
       if(client.connect(server, 80)){
       Serial.println("connected R2");
       Serial.println("Uploading....   +++++++++++++++++++++++    ");
       client.print("GET /sql_personal_data/write_flag_to_server.php?"); //Connecting and Sending values to database

       client.print("flagg=");
       client.println(10);
       
       client.stop(); //Closing the connection
       }
       else {
       // if you didn't get a connection to the server:
       Serial.println("Upload Failed       ");
       }
 }

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
     Write2Server(client);
     delay(2000);
     Write2Flag(client);
     }
if (RelStatus== "0") {
     digitalWrite(30, LOW);
    }
    
   }
 }
