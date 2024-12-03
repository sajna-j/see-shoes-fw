#include <Arduino_LSM6DSOX.h>
#define sideMotor A0
#define frontMotor A3

// Variables
float x, y, z;
int degreesX = 0;
int degreesY = 0;
const float imuThres = -30;

float sideVibration, frontVibration, sideVibe, frontVibe;
float distanceFront, distanceSide;

const int distanceThresholdCane = 46.8;
const int distanceThresholdStep = 24;
const float percentRaiseBtwnThresFront = 0.4;
const float percentRaiseBtwnThresSide = 0.6;
const float remainingPercentFront = 1 - percentRaiseBtwnThresFront;
const float remainingPercentSide = 1 - percentRaiseBtwnThresSide;

// Distance Sensor Pins
const int trigPinSide = 9;
const int echoPinSide = 11;
const int trigPinFront = 4; // 6;
const int echoPinFront = 2; //7;


void setup() {
  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);
  pinMode(trigPinSide, OUTPUT);
  pinMode(echoPinSide, INPUT);
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

        if (distanceFront < distanceThresholdCane) {
          frontVibe = get_vibe_front(distanceFront);
          analogWrite(frontMotor, frontVibe);  // vibrate
          Serial.print("Front Motor ON @ ");
          Serial.println(frontVibe);
        }
        else {
          analogWrite(frontMotor, 0);
          Serial.println("front motor OFF");
        }

        if (distanceSide < distanceThresholdCane) {
          sideVibe = get_vibe_side(distanceSide);
          analogWrite(sideMotor, sideVibe);
          Serial.print("Side Motor ON @ ");
          Serial.println(sideVibe);
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

delay(500);

}

float get_vibe_side(float distance) {
  sideVibration = 255 * (remainingPercentSide + percentRaiseBtwnThresSide*((distanceThresholdCane - distance) / distanceThresholdCane));
  return sideVibration;
}

float get_vibe_front(float distance) {
  if (distance < distanceThresholdStep) {
    frontVibration = 255;
  }
  else {
    frontVibration = 255 * (remainingPercentFront + percentRaiseBtwnThresFront*((distanceThresholdCane - distance) / (distanceThresholdCane - distanceThresholdStep)));
  }
  return frontVibration;
}


bool IMU_okay_angle() {
  if (x > 0.1) {
    x = 100 * x;
    degreesX = map(x, 0, -100, 0, 90);
    Serial.print("Tilting down ");
    Serial.print(degreesX);
    Serial.println(" degrees");
    return degreesX > imuThres;
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
