#include <ArduinoBLE.h>
#include "Arduino_BMI270_BMM150.h"

//Motor pins
const int sideMotorPin = A5;
const int frontMotorPin = A7;
float sideVibration, frontVibration;

// Distance Sensor Pins
const int trigPinFront = 7;
const int echoPinFront = 6;
const int trigPinSide = 11;
const int echoPinSide = 10;

float distanceFront = 0;
float distanceSide = 0;

const int distanceThresholdCane = 46.8;
const int distanceThresholdStep = 24;
const float percentRaiseBtwnThresFront = 0.4;
const float percentRaiseBtwnThresSide = 0.6;
const float remainingPercentFront = 1 - percentRaiseBtwnThresFront;
const float remainingPercentSide = 1 - percentRaiseBtwnThresSide;

// IMU result variables
const int imuThres = 60;
bool imu_angled_down;
float x, y, z;
int degreesX = 0;
int degreesY = 0;

void setup() {
  
  // Set up pins for motor and sensors
  pinMode(sideMotorPin, OUTPUT);
  pinMode(frontMotorPin, OUTPUT);
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
  distanceFront = getDistance(trigPinFront, echoPinFront);
  distanceSide = getDistance(trigPinSide, echoPinSide);
  Serial.print("Front: ");
  Serial.print(distanceFront);
  Serial.print("\tSide: ");
  Serial.println(distanceSide);

  if (distanceFront < distanceThresholdCane) {
    activateMotorFront(distanceFront);
  }
  else {
    deactivateMotorFront();
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

void handleIMU() {
  if (x > 0.1) {
    x = 100 * x;
    degreesX = map(x, 0, 97, 0, 90);
    Serial.print("Tilting up ");
    Serial.print(degreesX);
    Serial.println(" degrees");
  } else if (x < -0.1) {
    x = 100 * x;
    degreesX = map(x, 0, -100, 0, 90);
    Serial.print("Tilting down ");
    Serial.print(degreesX);
    Serial.println(" degrees");
  }
/*
  if (y > 0.1) {
    y = 100 * y;
    degreesY = map(y, 0, 97, 0, 90);
    Serial.print("Tilting left ");
    Serial.print(degreesY);
    Serial.println(" degrees");
  } else if (y < -0.1) {
    y = 100 * y;
    degreesY = map(y, 0, -100, 0, 90);
    Serial.print("Tilting right ");
    Serial.print(degreesY);
    Serial.println(" degrees");
  }
  */
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

void activateBoth(float distanceFront, float distanceSide) {
  Serial.println("Both ON");
  activateMotorFront(distanceFront);
  activateMotorSide(distanceSide);
}

void deactivateBoth() {
  analogWrite(sideMotorPin, 0);
  analogWrite(frontMotorPin, 0);
  Serial.println("Both OFF");
}

void activateMotorSide(float distance) {
  // side motor does not need more alert for being within step range 
  sideVibration = 255 * (remainingPercentSide + percentRaiseBtwnThresSide*((distanceThresholdCane - distance) / distanceThresholdCane));
  analogWrite(sideMotorPin, sideVibration);
  Serial.print("Side vibrating @ ");
  Serial.println(sideVibration);
}

void deactivateMotorSide() {
  analogWrite(sideMotorPin, 0);   // Stop motor
  Serial.println("Side Motor OFF");
}

void activateMotorFront(float distance) {
  // front motor should vibrate 60 to 100% within the two thresholds, 100% always if less than step
  if (distance < distanceThresholdStep) {
    analogWrite(frontMotorPin, 255);
    Serial.println("Front vibrating @ 255");
  }
  else {
    frontVibration = 255 * (remainingPercentFront + percentRaiseBtwnThresFront*((distanceThresholdCane - distance) / (distanceThresholdCane - distanceThresholdStep)));
    analogWrite(frontMotorPin, frontVibration);  // Vibrate motor
    Serial.print("Front vibrating @ ");
    Serial.println(frontVibration);
  }
}

void deactivateMotorFront() {
  analogWrite(frontMotorPin, 0);   // Stop motor
  Serial.println("Front Motor OFF");
}
