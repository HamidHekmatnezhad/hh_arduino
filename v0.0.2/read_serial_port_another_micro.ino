#include <SPI.h>
unsigned long end_time, start_time;
unsigned short delay_time=500, delay_get_data=2000;
String data;

void setup() {
  Serial.begin(9600);
  // serial for uart
  Serial1.begin(9600);

}

void loop() {

    data = Serial1.readStringUntil('\n');
    Serial.println("-----------------------------------------------");
    Serial.println(data);
  
    String h = get_data(data, "heartbeat", "endhb");
    String o = get_data(data, "oxygen", "endo");
    String w = get_data(data, "weight", "endw");


    Serial.println("+---------------------+");
    Serial.println("| hea.. : " + h);
    Serial.println("| oxy.. : " + o);
    Serial.println("| wei.. : " + w);
    Serial.println("+---------------------+");
    
    delay(delay_time);
}

String get_data(String dt, String startStr, String endStr){
  int s = dt.indexOf(startStr)+ startStr.length() +1;
  int e = dt.indexOf(endStr);
  String ans = dt.substring(s, e);
  
  return ans;
}
