#include "Arduino_BMI270_BMM150.h"


const float imuThres = 60;
// Variables
float x, y, z;
int degreesX = 0;
int degreesY = 0;

const int frontMotor = A7;
const int sideMotor = D5;
float sideVibration, frontVibration;
float distanceThres = 12;
float distanceFront, distanceSide;

// Distance Sensor Pins
const int trigPinFront = 7;
const int echoPinFront = 6;
const int trigPinSide = 10;  // 11
const int echoPinSide = 11; // 10


void setup() {
  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);
  pinMode(trigPinSide, OUTPUT);
  pinMode(echoPinSide, INPUT);
  pinMode(frontMotor, OUTPUT);
  pinMode(sideMotor, OUTPUT);
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
        distanceSide = getDistance(trigPinSide, echoPinSide);
        Serial.print("Side: ");
        Serial.print(distanceSide);
        Serial.print("Front: ");
        Serial.print(distanceFront);

        if (distanceFront < distanceThres) {
          analogWrite(frontMotor, 255);  // vibrate
          Serial.println(" Front Motor ON");
        }
        else {
          analogWrite(frontMotor, 0);
          Serial.println(" front motor OFF");
        }

        if (distanceSide < distanceThres) {
          digitalWrite(sideMotor, HIGH);
          Serial.println("Side motor On");
        }
        else {
          digitalWrite(sideMotor, LOW);
          Serial.println("side motor off");
        }

      delay(500);  // Run for 3 second
      } 
      else {
        analogWrite(frontMotor, 0);
        digitalWrite(sideMotor, LOW);
        Serial.println(" both motor off bc bad angle");

      }
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
