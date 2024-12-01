#include "Arduino_BMI270_BMM150.h"


const float imuThres = 60;
// Variables
float x, y, z;
int degreesX = 0;
int degreesY = 0;

const int frontMotor = A3;
const int sideMotor = A7;
float sideVibration, frontVibration;
float distanceThres = 12;
float distanceFront, distanceSide;

// Distance Sensor Pins
const int trigPinSide = 6;
const int echoPinSide = 7;
const int trigPinFront = 10; // 6;
const int echoPinFront = 11; //7;


void setup() {
  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);
  pinMode(frontMotor, OUTPUT);
  Serial.begin(9600);  // Initialize serial communication for debugging
    // Initialize IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop() {

  if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(x, y, z);
      if (IMU_okay_angle()) {

        distanceFront = getDistance(trigPinFront, echoPinFront);
        Serial.print("Front: ");
        Serial.print(distanceFront);

        distanceSide = getDistance(trigPinSide, echoPinSide);
        Serial.print("\tSide: ");
        Serial.println(distanceSide);

        if (distanceFront < distanceThres) {
          analogWrite(frontMotor, 255);  // vibrate
          Serial.println("Front Motor ON");
        }
        else {
          analogWrite(frontMotor, 0);
          Serial.println("front motor OFF");
        }

        if (distanceSide < distanceThres) {
          analogWrite(sideMotor, 255);
          Serial.println("Side Motor ON");
        }
        else {
          analogWrite(sideMotor, 0);
          Serial.println("Side motor OFF");
        }

      }
      else {
        analogWrite(sideMotor, 0);
        analogWrite(frontMotor, 0);
      }
  }
  delay(500);  // Run for 3 second
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
