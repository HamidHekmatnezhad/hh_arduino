#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

BLEClient* pClient;
BLERemoteService* pRemoteService;
BLERemoteCharacteristic* pRemoteCharacteristic;
BLEAddress pulseOximeterAddress("24:56:48:8f:95:73"); // Replace with actual address of Pulse Oximeter

BLEUUID serviceUUID("0000ffe0-0000-1000-8000-00805f9b34fb");  // Replace with the correct service UUID
BLEUUID charUUID("0000ffe2-0000-1000-8000-00805f9b34fb");     // UUID for the characteristic supporting NOTIFY

void notifyCallback(BLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* data, size_t length, bool isNotify) {
  if (length > 0) {
    
//    Serial.print("Received data with length: ");
//    Serial.println(length);

    // Process the received data
    int d1 = data[1];  // Assuming heart rate is at data[1]
    int d2 = data[2];       // Assuming SpO2 is at data[2]
    int d3 = data[3];       // Assuming SpO2 is at data[2]
    int d4 = data[4];       // Assuming SpO2 is at data[2]
    int d5 = data[5];       // Assuming SpO2 is at data[2]
    int d6 = data[6];       // Assuming SpO2 is at data[2]
    int d7 = data[7];       // Assuming SpO2 is at data[2]
    int d8 = data[8];       // Assuming SpO2 is at data[2]
    int d9 = data[9];       // Assuming SpO2 is at data[2]
    int d10 = data[10];       // Assuming SpO2 is at data[2]
    int d11 = data[11];       // Assuming SpO2 is at data[2]
    int d12 = data[12];       // Assuming SpO2 is at data[2]
    int d13 = data[13];       // Assuming SpO2 is at data[2]
    int d14 = data[14];       // Assuming SpO2 is at data[2]
    int d15 = data[15];       // Assuming SpO2 is at data[2]
    int d16 = data[16];       // Assuming SpO2 is at data[2]
    int d17 = data[17];       // Assuming SpO2 is at data[2]
    int d18 = data[18];       // Assuming SpO2 is at data[2]
    int d19 = data[19];       // Assuming SpO2 is at data[2]
    int d20 = data[20];       // Assuming SpO2 is at data[2]
    
//    Serial.print("d1: ");
//    Serial.println(d1);
//    Serial.print("d2: ");
//    Serial.println(d2);
//        Serial.print("d3: ");
//    Serial.println(d3);
//        Serial.print("d4: ");
//    Serial.println(d4);
//        Serial.print("d5: ");
//    Serial.println(d5);
//        Serial.print("d6 (SpO2): ");
//    Serial.println(d6);
//        Serial.print("d7 (BPM): ");
//    Serial.println(d7);
//        Serial.print("d8 (PI%): ");
//    Serial.println(d8);
//        Serial.print("d9: ");
//    Serial.println(d9);
//        Serial.print("d10: ");
//    Serial.println(d10);
//        Serial.print("d11 (RP/min): ");
//    Serial.println(d11);
//
//        Serial.print("d12: ");
//    Serial.println(d12);

//            Serial.print("d13 (PPG1): ");
//    Serial.println(d13);
    
//            Serial.print("d14: ");
//    Serial.println(d14);
//            Serial.print("d15: ");
//    Serial.println(d15);
//            Serial.print("d16: ");
//    Serial.println(d16);
//            Serial.print("d17: ");
//    Serial.println(d17);

//            Serial.print("d18 PPG2): ");
//    Serial.println(d18);
//            Serial.print("d19: ");
//    Serial.println(d19);
//            Serial.print("d20: ");
//    Serial.println(d20);

///////////////////////////////////////////////////////////////////////// check 
//Serial.println(d13+d18);
 Serial.print(d13); // Serial.print(d[13]);
  Serial.print(",");  
  Serial.println(d18); // Serial.println(d[18]);

  } else {
    Serial.println("Received data with length 0.");
  }
}

void setup() {
  Serial.begin(115200);
  BLEDevice::init("");

  // Create a client to connect to the BLE server
  pClient = BLEDevice::createClient();

  // Try to connect to the device
  if (pClient->connect(pulseOximeterAddress)) {
    Serial.println("Connected to Pulse Oximeter!");

    // Access the remote service
    pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService != nullptr) {
      // Access the characteristic we need for notifications
      pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
      if (pRemoteCharacteristic != nullptr && pRemoteCharacteristic->canNotify()) {
        Serial.println("Found the characteristic for notifications!");

        // Register for notifications
        pRemoteCharacteristic->registerForNotify(notifyCallback);
      } else {
        Serial.println("Characteristic does not support notifications.");
      }
    } else {
      Serial.println("Failed to find the service.");
    }
  } else {
    Serial.println("Failed to connect to Pulse Oximeter.");
  }
}

void loop() {
  // Nothing in the loop, the notifyCallback will handle incoming data
  delay(3000);  // Add a delay to avoid busy looping
}
