int time=3000, i=10;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
// hearthbeat	oxygen	weight_kg	temperature
delay(time);
Serial.print("\nheartbeat=");Serial.print(i+60);Serial.print("endhb   ");
Serial.print("oxygen=");Serial.print(i+85);Serial.print("endo   ");
Serial.print("weight=");Serial.print(i+102);Serial.print("endw   ");
i++;
}