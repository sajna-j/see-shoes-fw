#include <ArduinoBLE.h>
#include "Arduino_BMI270_BMM150.h"

// Motor and Sensor Pins
const int sideMotorPin = A5;
const int frontMotorPin = A7;

const int trigPinSide = 7;
const int echoPinSide = 6;
const int trigPinFront = 11;
const int echoPinFront = 10;

// Variables
float x, y, z;
int degreesX = 0;
int degreesY = 0;
float distanceFront = 0;
float distanceSide = 0;
const int distanceThresholdCane = 46.8; // Threshold distance to activate the motor
const int distanceThresholdStep = 24;
const float percentRaiseBtwnThresFront = 0.4;
const float percentRaiseBtwnThresSide = 0.6;
const float remainingPercentFront = 1 - percentRaiseBtwnThresFront;
const float remainingPercentSide = 1 - percentRaiseBtwnThresSide;
float sideVibration, frontVibration;

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

    // Read IMU data
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(x, y, z);
      //handleIMU();
    }

    // Read distance data
    distanceFront = getDistance(trigPinFront, echoPinFront);
    distanceSide = getDistance(trigPinSide, echoPinSide);
    Serial.print("Front: ");
    Serial.print(distanceFront);
    Serial.print("\tSide: ");
    Serial.println(distanceSide);
    /*
    //Control motor based on distance
    if (distanceFront < distanceThresholdCane && distanceSide < distanceThresholdCane) {
      activateBoth(distanceFront, distanceSide);

    } else if (distanceFront < distanceThresholdCane) {
      activateMotorFront(distanceFront);
      deactivateMotorSide();
    } else if (distanceSide < distanceThresholdCane) {
      activateMotorSide(distanceSide);
      deactivateMotorFront();
    }
    else {
      deactivateBoth();
    }
    */

    if (distanceFront < distanceThresholdCane) {
      activateMotorSide(distanceSide);
    } else {
      deactivateMotorSide();
    }

    delay(100);

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
