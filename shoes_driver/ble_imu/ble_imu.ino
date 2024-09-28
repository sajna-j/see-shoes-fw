#include <ArduinoBLE.h>
#include "Arduino_BMI270_BMM150.h"

BLEService imuService("180D");
BLEIntCharacteristic xDegreesCharacteristic("1111111", BLERead | BLENotify);
BLEIntCharacteristic yDegreesCharacteristic("2222222", BLERead | BLENotify);

float x, y, z;
int degreesX = 0;
int degreesY = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  // Set up BLE service and characteristic
  BLE.setLocalName("Nano33BLE_IMU");
  BLE.setAdvertisedService(imuService);
  imuService.addCharacteristic(yDegreesCharacteristic);
  imuService.addCharacteristic(xDegreesCharacteristic);
  BLE.addService(imuService);

  // Start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(x, y, z);
        

        // 
        if(x > 0.1){
          x = 100*x;
          degreesX = map(x, 0, 97, 0, 90);
          Serial.print("Tilting up ");
          Serial.print(degreesX);
          Serial.println("  degrees");
          xDegreesCharacteristic.writeValue(degreesX);
        }
        else if(x < -0.1){
          x = 100*x;
          degreesX = map(x, 0, -100, 0, 90);
          Serial.print("Tilting down ");
          Serial.print(degreesX);
          Serial.println("  degrees");
          xDegreesCharacteristic.writeValue(degreesX);
        }

        if(y > 0.1){
          y = 100*y;
          degreesY = map(y, 0, 97, 0, 90);
          Serial.print("Tilting left ");
          Serial.print(degreesY);
          Serial.println("  degrees");
          yDegreesCharacteristic.writeValue(degreesY);
        }
        else if(y < -0.1){
          y = 100*y;
          degreesY = map(y, 0, -100, 0, 90);
          Serial.print("Tilting right ");
          Serial.print(degreesY);
          Serial.println("  degrees");
          yDegreesCharacteristic.writeValue(degreesY);
        }

        }
      }
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());

}