// Define TX and RX pins for UART (change if needed)
#define TXD1 19
#define RXD1 21

// Use Serial1 for UART communication
HardwareSerial mySerial(1);

int counter = 0;

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600, SERIAL_8N1, RXD1, TXD1);  // UART setup
    Serial.println("ESP32 UART Transmitter");
}

void loop() {
    // Send message over UART
    mySerial.println(String(counter));
    Serial.println("Sent: " + String(counter));
    counter++;  // increment the counter
    delay(1000);  // send data every second
}