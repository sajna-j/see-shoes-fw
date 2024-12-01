#include "Arduino_BMI270_BMM150.h"


const int frontMotor = A7;
float userNum = 255; // To store the user input
const float imuThres = 60;
// Variables
float x, y, z;
int degreesX = 0;
int degreesY = 0;

void setup() {
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
        analogWrite(frontMotor, 255);  // vibrate
        Serial.print("Front Motor ON");
      } else {
        analogWrite(frontMotor, 0);
        Serial.print("Front Motor OFF");
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
