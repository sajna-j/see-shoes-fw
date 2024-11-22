#include <ArduinoBLE.h>
#include "Arduino_BMI270_BMM150.h"

// Motor and Sensor Pins
const int sideMotorPin = D5;
const int frontMotorPin = D4;

const int trigPinFront = 7;
const int echoPinFront = 6;
const int trigPinSide = 11;
const int echoPinSide = 10;

// Variables
float x, y, z;
int degreesX = 0;
int degreesY = 0;
float distanceFront = 0;
float distanceSide = 0;
const int distanceThreshold = 3; // Threshold distance to activate the motor

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
      handleIMU();
    }

    // Read distance data
    distanceFront = getDistance(trigPinFront, echoPinFront);
    distanceSide = getDistance(trigPinSide, echoPinSide);
    Serial.print("Front: ");
    Serial.print(distanceFront);
    Serial.print("\tSide: ");
    Serial.println(distanceSide);

    //Control motor based on distance
    if (distanceFront < distanceThreshold && distanceSide < distanceThreshold) {
      activateBoth();

    } else if (distanceFront < distanceThreshold) {
      activateMotorFront();
      deactivateMotorSide();
    } else if (distanceSide < distanceThreshold) {
      activateMotorSide();
      deactivateMotorFront();
    }
    else {
      deactivateBoth();
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

void activateBoth() {
  digitalWrite(sideMotorPin, HIGH);
  digitalWrite(frontMotorPin, HIGH);
  Serial.println("Both ON");
}

void deactivateBoth() {
  digitalWrite(sideMotorPin, LOW);
  digitalWrite(frontMotorPin, LOW);
  Serial.println("Both OFF");
}

void activateMotorSide() {
  digitalWrite(sideMotorPin, HIGH);  // Vibrate motor
  Serial.println("Side Motor ON");
}

void deactivateMotorSide() {
  digitalWrite(sideMotorPin, LOW);   // Stop motor
  Serial.println("Side Motor OFF");
}

void activateMotorFront() {
  digitalWrite(frontMotorPin, HIGH);  // Vibrate motor
  Serial.println("Front Motor ON");
}

void deactivateMotorFront() {
  digitalWrite(frontMotorPin, LOW);   // Stop motor
  Serial.println("Front Motor OFF");
}
