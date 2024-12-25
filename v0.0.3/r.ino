// After Comments from Dr.Iman Sadrinejad

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Final ESP32 MySQL Database
//======================================== Including the libraries.
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
//#include "DHT.h"
//========================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Defines the Digital Pin of the "On Board LED".
#define ON_Board_LED 2

// Defines GPIO 13 as LED_1.
#define LED_01 13

// Defines GPIO 12 as LED_2.
#define LED_02 12

//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(8, 9); // RX, TX
String request="";
//////////////////////////
float sensorValues[10];
int count = 0;
float average = 0;
float weight = 0;
float Final_average = 0;
float S1;
float S2=0;
float weight2 = 0;

#define PB_rev 26
#define PB_fwd 27
#define Manual 32
#define Auto 33

int value_PBfwd=0;
int value_PBrev=0;
int value_Man=0;
int value_Auto=0;

//======================================== SSID and Password of your WiFi router.
const char* ssid = "pars";
const char* password = "09111271592";

//const char* ssid = "deviceG01";
//const char* password = "deviceG01";
//========================================
int countWL1 = 0;
int countWL2 = 0;
//======================================== Variables for HTTP POST request data.
String postData = ""; //--> Variables sent for HTTP POST request data.
String payload = "";  //--> Variable for receiving response from HTTP POST.
//========================================

//======================================== Variables for DHT11 sensor data.
float send_Temp;
int send_Humd;
String send_Status_Read_DHT11 = "";
//========================================
int Flag;

// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6)
const int potPin = 34;
// variable for storing the potentiometer value
int potValue = 0;

// ________________________________________________________________________________ Subroutine to read and get data from the DHT11 sensor.
void get_DHT11_sensor_data() {
  Serial.println();
  Serial.println("-------------get_DHT11_sensor_data()");
      while (Serial2.available()) {
    char c = Serial2.read();
    String mystring(c);
    request+=mystring;
    String F_Event = "=";
    int Eventstop = request.indexOf(F_Event);
  }
 
// Serial.print("out number:");
// Serial.println(request);

String HotKey = request.substring(1, 8);

// Serial.print("HotKey(2):");
// Serial.println(HotKey[2]);
 
 char b0=HotKey[0];
 char b1=HotKey[1];
 char b3=HotKey[3];
 char b4=HotKey[4];
 char b5=HotKey[5];
 
 int b0int = b0 - '0';
 int b1int = b1 - '0';
 int b3int = b3 - '0';
 int b4int = b4 - '0';
 int b5int = b5 - '0';

 float weight=(b0int*0.01)+(b1int*0.1)+(b3int*1)+(b4int*10)+(b5int*100);
 //Serial.print("Weight (Kg):");
 //Serial.println(weight);

///////////////
  //sensorValues[count] = (float)sensorValue*100.0/1023.0;

//////////////

 if (weight>0&&weight<400){
  Serial.print("Weight = ");
  Serial.println(weight);

  //S1=weight;
  if(abs(weight-S2)<10){
    S2=weight;
    send_Temp=weight;
  }else{
    send_Temp=S2;
    weight2=S2;
    }
 
  lcd.clear();
  Serial.print("weight2 = ");
  Serial.println(weight2);
  lcd.setCursor(0,1);
  lcd.print("Weight = ");
  lcd.print(weight);
 }

request="";


potValue = analogRead(potPin);
send_Humd=potValue;
  // Check if any reads failed.
  if (isnan(send_Temp) || isnan(send_Humd)) {
    //Serial.println("Failed to read from DHT sensor!");
    send_Temp = 0.00;
    send_Humd = 0;
    send_Status_Read_DHT11 = "FAILED";
  } else {
    send_Status_Read_DHT11 = "SUCCEED";
  }
 
}



void setup() {
  // put your setup code here, to run once:
 
  Serial.begin(9600); //--> Initialize serial communications with the PC.
      // initialize the LCD
    lcd.begin();
    // Turn on the blacklight and print a message.
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Smart Pasta");    
    lcd.setCursor(0,1);
    lcd.print("IAU 2023");
   
pinMode(PB_fwd,INPUT);
pinMode(PB_rev,INPUT);
pinMode(Manual,INPUT);
pinMode(Auto,INPUT);

 // set the data rate for the SoftwareSerial port
 Serial2.begin(9600);

  pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output.
  pinMode(LED_01,OUTPUT); //--> LED_01 port Direction output.
  pinMode(LED_02,OUTPUT); //--> LED_02 port Direction output.

  digitalWrite(ON_Board_LED, LOW); //--> Turn off Led On Board.
  digitalWrite(LED_01, LOW); //--> Turn off Led LED_01.
  digitalWrite(LED_02, LOW); //--> Turn off Led LED_02.

  //---------------------------------------- Make WiFi on ESP32 in "STA/Station" mode and start connecting to WiFi Router/Hotspot.
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  //----------------------------------------
 
  Serial.println();
  Serial.println("-------------");
  Serial.print("Connecting");


   while(WiFi.status() != WL_CONNECTED && countWL1<=20) {
    delay(500);
    Serial.print("Trying Default...");Serial.println(countWL1);
    lcd.setCursor(0,1);
    lcd.print("Trying Def. ");
    lcd.print(countWL1);
    lcd.print("   ");
    countWL1=countWL1+1;
  }
 
  digitalWrite(ON_Board_LED, LOW); //--> Turn off the On Board LED when it is connected to the wifi router.
 
  //---------------------------------------- If successfully connected to the wifi router, the IP Address that will be visited is displayed in the serial monitor
  Serial.println();
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());
  Serial.println("-------------");
  //----------------------------------------
    lcd.setCursor(0,1);
    lcd.print("Connected 2 SSID");
  // Setting up the DHT sensor (DHT11).
//  dht11_sensor.begin();

  delay(400);
}

void loop() {
  sendData2serv();
  value_Man=digitalRead(Manual);
  value_Auto=digitalRead(Auto);
  value_PBfwd=digitalRead(PB_fwd);
  value_PBrev=digitalRead(PB_rev);

  if(value_Man==HIGH){
      lcd.setCursor(0,0);
      lcd.print("Manual is READY ");
     
      if(value_PBfwd==HIGH){
    digitalWrite(LED_01,HIGH);
    Serial.println("FWD");
      lcd.setCursor(0,0);
      lcd.print("Manual: Forward ");
    }
  else{digitalWrite(LED_01,LOW);}

    if(value_PBrev==HIGH){
      digitalWrite(LED_02,HIGH);
      Serial.println("REV");
      lcd.setCursor(0,0);
      lcd.print("Manual: Reverse ");
      }
  else{digitalWrite(LED_02,LOW);}

  if(PB_fwd==1&&PB_rev==1){
    digitalWrite(LED_01,LOW);
    digitalWrite(LED_02,LOW);
    }
    }else if (value_Auto==HIGH){
      Serial.println("Auto");
      lcd.setCursor(0,0);
      lcd.print("Automatic.......");
      digitalWrite(LED_02,HIGH);
      delay(50);//T_ON = 50mSec
      digitalWrite(LED_02,LOW);
      sendData2serv();
      sendData2serv();
      sendData2serv();
      sendData2serv();
      sendData2serv();
      sendData2serv();
      sendData2serv();
      sendData2serv();
      sendData2serv();
      sendData2serv();
      }else{
      lcd.setCursor(0,0);
      lcd.print("...STOP  Mode...");
      digitalWrite(LED_01,LOW);
      digitalWrite(LED_02,LOW);
        }
}
 

void sendData2serv(){
  //---------------------------------------- Check WiFi connection status.
  if(WiFi.status()== WL_CONNECTED) {
    HTTPClient http;  //--> Declare object of class HTTPClient.
    int httpCode;     //--> Variables for HTTP return code.
    get_DHT11_sensor_data();
    //........................................ The process of sending the DHT11 sensor data and the state of the LEDs to the database.
    String LED_01_State = "";
    String LED_02_State = "";
    postData = "id=esp32_01";
    postData += "&temperature=" + String(send_Temp);
    postData += "&humidity=" + String(send_Humd);
    postData += "&status_read_sensor_dht11=" + send_Status_Read_DHT11;
    postData += "&led_01=" + LED_01_State;
    postData += "&led_02=" + LED_02_State;
    payload = "";
 
    digitalWrite(ON_Board_LED, HIGH);
    //if (Flag==1){
    Serial.println();
    Serial.println("---------------updateDHT11data_and_recordtable.php");
    // Example : http.begin("http://192.168.0.0/ESP32_MySQL_Database/Final/updateDHT11data_and_recordtable.php");
    http.begin("http://192.168.1.51/ESP32_MySQL_Database/Final/updateDHT11data_and_recordtable.php");  //--> Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");  //--> Specify content-type header
   
    httpCode = http.POST(postData); //--> Send the request
    payload = http.getString();  //--> Get the response payload
 
    Serial.print("httpCode : ");
    Serial.println(httpCode); //--> Print HTTP return code
    Serial.print("payload  : ");
    Serial.println(payload);  //--> Print request response payload
   
    http.end();  //Close connection
    Serial.println("---------------");
    digitalWrite(ON_Board_LED, LOW);
    lcd.setCursor(0,1);
    lcd.print("Data Recording..");
    //delay(500);
  }
}
