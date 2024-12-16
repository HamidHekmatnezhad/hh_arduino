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
int counter=0, cc=5;

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

heartbeats_r="23"; oxygen_r="76"; weight_r="54"; temperature_r="42";

if (flg == "1") { // access to collect data from another Microcontroller
  Serial.println("---------------<< Flag 1 >>---------------");
    // Write2Server(heartbeats_r, oxygen_r, weight_r, temperature_r);
    Write2Flag(client);
  }
delay(delay_time);
request="";
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
    Serial.println("connection failed - (print_flag)");
  }
}

void Write2Server(String heartbeat, String oxygen, String weight_kg, String temperature) {
   
      if(client.connect(server, 80)){
        Serial.println("Uploading.... (Write2Server)");
        // client.print("GET /sql_personal_data/write_variables_to_server.php?"); //Connecting and Sending values to database

        // +----------------------+
        // | sens_1 = hearthbeat  |
        // | sens_2 = oxygen      |
        // | sens_3 = weight_kg   |
        // | sens_4 = temperature |
        // | sens_5 = None        |
        // +----------------------+
        // local/sql_personal_data/write_variables_to_server.php?sens_1=32&sens_2=32&sens_3=32&sens_4=32
      
        // send data
        // client.print("sens_1=");
        // client.print(heartbeats_r);
        // client.print("&sens_2=");
        // client.print(oxygen_r);
        // client.print("&sens_3=");
        // client.print(weight_r);
        // client.print("&sens_4=");
        // client.print(temperature_r);
        // client.print("&sens_5=");
        // client.println(999);

        String u,u1,u2,u3,u4,u5;

        u1 = "GET /sql_personal_data/write_variables_to_server.php?";
        u2 = "sens_1=" + heartbeats_r;
        u3 = "&sens_2=" + oxygen_r;
        u4 = "sens_3=" + weight_r;
        u5 = "&sens_4=" + temperature_r + "\n";
        u = u1 + u2 + u3 + u4 + u5;
        client.print(u); //Connecting and Sending values to database
        // client.println()

        client.stop(); //Closing the connection
      }
      else {
        // if you didn't get a connection to the server:
        Serial.println("Upload Failed!!!!! - (write2server)");
      }

 }

void Write2Flag(EthernetClient client) {
   
    if(client.connect(server, 80)){
    Serial.println("Uploading.... (Write2Flag)");
    client.print("GET /sql_personal_data/write_flag_to_server.php?"); //Connecting and Sending values to database
    client.println(" ");

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

  String data, ans;
  switch (serial_port)
  {
  case 1:
    data = Serial1.readStringUntil('\n');
    break;
  
  case 2:
    data = Serial2.readStringUntil('\n');
    break;
  
  case 3:
    data = Serial3.readStringUntil('\n');
    break;
  }

  Serial.println(">>>>>>>>>>>>>>> " + data);
  int s = data.indexOf(startStr) + startStr.length() ;
  int e = data.indexOf(endStr);
  ans = data.substring(s, e);
  
  if (ans == "") return "-1";
  else return ans;
}

