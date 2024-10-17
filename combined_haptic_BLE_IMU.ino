#include <ArduinoBLE.h>
#include "Arduino_BMI270_BMM150.h"

// BLE Service and Characteristics
BLEService imuService("180D");
BLEIntCharacteristic xDegreesCharacteristic("1111111", BLERead | BLENotify);
BLEIntCharacteristic yDegreesCharacteristic("2222222", BLERead | BLENotify);
BLEIntCharacteristic distanceCharacteristic("3333333", BLERead | BLENotify); // New characteristic for distance

// Motor and Sensor Pins
const int motorPin = D5;
const int trigPinFront = 7;
const int echoPinFront = 8;
const int trigPinSide = 9;
const int echoPinSide = 10;

// Variables
float x, y, z;
int degreesX = 0;
int degreesY = 0;
float distanceFront = 0;
float distanceSide = 0;
const int distanceThreshold = 50; // Threshold distance to activate the motor

void setup() {
  
  
  // Set up pins for motor and sensors
  pinMode(motorPin, OUTPUT);
  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);
  pinMode(trigPinSide, OUTPUT);
  pinMode(echoPinSide, INPUT);
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  // Initialize IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  // Set up BLE service and characteristics
  BLE.setLocalName("Nano33BLE_IMU");
  BLE.setAdvertisedService(imuService);
  imuService.addCharacteristic(xDegreesCharacteristic);
  imuService.addCharacteristic(yDegreesCharacteristic);
  imuService.addCharacteristic(distanceCharacteristic); // Add distance characteristic
  BLE.addService(imuService);

  // Start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  BLEDevice central = BLE.central();
  
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      // Read IMU data
      if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(x, y, z);
        handleIMU();
      }

      // Read distance data
      distanceFront = getDistance(trigPinFront, echoPinFront);
      distanceSide = getDistance(trigPinSide, echoPinSide);
      Serial.print("Distance Front: ");
      Serial.println(distanceFront);
      Serial.print("Distance Side: ");
      Serial.println(distanceSide);
      
      // Publish distance to BLE
      distanceCharacteristic.writeValue((int)distanceFront);

      // Control motor based on distance
      if (distanceFront < distanceThreshold || distanceSide < distanceThreshold) {
        activateMotor();
      } else {
        deactivateMotor();
      }

      delay(100); // Small delay for stability
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}

void handleIMU() {
  if (x > 0.1) {
    x = 100 * x;
    degreesX = map(x, 0, 97, 0, 90);
    Serial.print("Tilting up ");
    Serial.print(degreesX);
    Serial.println(" degrees");
    xDegreesCharacteristic.writeValue(degreesX);
  } else if (x < -0.1) {
    x = 100 * x;
    degreesX = map(x, 0, -100, 0, 90);
    Serial.print("Tilting down ");
    Serial.print(degreesX);
    Serial.println(" degrees");
    xDegreesCharacteristic.writeValue(degreesX);
  }

  if (y > 0.1) {
    y = 100 * y;
    degreesY = map(y, 0, 97, 0, 90);
    Serial.print("Tilting left ");
    Serial.print(degreesY);
    Serial.println(" degrees");
    yDegreesCharacteristic.writeValue(degreesY);
  } else if (y < -0.1) {
    y = 100 * y;
    degreesY = map(y, 0, -100, 0, 90);
    Serial.print("Tilting right ");
    Serial.print(degreesY);
    Serial.println(" degrees");
    yDegreesCharacteristic.writeValue(degreesY);
  }
}

float getDistance(int trigPin, int echoPin) {
  float echoTime, calculatedDistance;

  // Send ultrasonic pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Calculate distance based on pulse time
  echoTime = pulseIn(echoPin, HIGH);
  calculatedDistance = echoTime / 148.0;  // Speed of sound in air (cm/us)
  
  return calculatedDistance;
}

void activateMotor() {
  digitalWrite(motorPin, HIGH);  // Vibrate motor
  Serial.println("Motor ON");
}

void deactivateMotor() {
  digitalWrite(motorPin, LOW);   // Stop motor
  Serial.println("Motor OFF");
}
