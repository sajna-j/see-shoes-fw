#include <ArduinoBLE.h>
#include "Arduino_BMI270_BMM150.h"


const int trigPinFront = 7;
const int echoPinFront = 6;
const int trigPinSide = 11;
const int echoPinSide = 10;
const int imuThres = 60;

// Variables
bool imu_angled_down;
float x, y, z;
int degreesX = 0;
int degreesY = 0;
float distanceFront = 0;
float distanceSide = 0;
const int distanceThreshold = 3; // Threshold distance to activate the motor

void setup() {
  
  // Set up pins for motor and sensors
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

}

void loop() {


    // Read distance data
    /*

    */
    // Read IMU data
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(x, y, z);
      if (IMU_okay_angle()) {
        distanceFront = getDistance(trigPinFront, echoPinFront);
        distanceSide = getDistance(trigPinSide, echoPinSide);
        Serial.print("Front: ");
        Serial.print(distanceFront);
        Serial.print("\tSide: ");
        Serial.println(distanceSide);
      }
    }

    delay(100);

}

bool IMU_okay_angle() {
  if (x < -0.1) {
    
    x = 100 * x;
    degreesX = map(x, 0, -100, 0, 90);
    Serial.print("Tilting down ");
    Serial.print(degreesX);
    Serial.println(" degrees");
    return degreesX < imuThres;
  }
  return true;

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
