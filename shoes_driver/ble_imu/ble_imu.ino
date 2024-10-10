#include <ArduinoBLE.h>
#include "Arduino_BMI270_BMM150.h"

BLEService imuService("180D");
BLEIntCharacteristic xDegreesCharacteristic("1111111", BLERead | BLENotify);
BLEIntCharacteristic yDegreesCharacteristic("2222222", BLERead | BLENotify);

float x, y, z;
int degreesX = 0;
int degreesY = 0;
const int StrigPin = 9;
const int SechoPin = 10;
const int FtrigPin = 7;
const int FechoPin = 8;
float duration, distance;

void setup() {s
  pinMode(FtrigPin, OUTPUT);
  pinMode(FehcoPin, INPUT);
  pinMode(StrigPin, OUTPUT);
  pinMode(SehcoPin, INPUT);
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

  // TODO: once the real sensors are connected use these! (Test bench only has one so this has to be changed anyways)
  // distanceF = getDistanceFront();
  // distanceB = getDistanceBack();

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

float getDistanceFront()
{
  float echoTimeF;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistanceF;         //variable to store the distance calculated from the echo time
  
  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(FtrigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(FtrigPin, LOW);

  echoTimeF = pulseIn(FechoPin, HIGH);      //use the pulsein command to see how long it takes for the pulse to bounce back to the sensor

  calculatedDistanceF = echoTimeF / 148.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)
  
  return calculatedDistanceF;              //send back the distance that was calculated
}

float getDistanceSide()
{
  float echoTimeS;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistanceS;         //variable to store the distance calculated from the echo time
  
  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(StrigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(StrigPin, LOW);

  echoTimeS = pulseIn(SechoPin, HIGH);      //use the pulsein command to see how long it takes for the pulse to bounce back to the sensor

  calculatedDistanceB = echoTimeS / 148.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)
  
  return calculatedDistanceS;              //send back the distance that was calculated
}
