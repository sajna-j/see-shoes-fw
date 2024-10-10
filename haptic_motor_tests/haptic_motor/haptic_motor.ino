// const int motorPin = D5;  // Use a PWM-capable pin (e.g., 3, 5, 6, 9, 10, or 11)

void setup() {
  pinMode(D5, OUTPUT);  // Set the motor pin as an output
  Serial.begin(9600);  // Initialize serial communication for debugging
}

void loop() {
  // Turn on the motor
  digitalWrite(D5, 255);  // vibrate
  Serial.println("Motor ON");
  delay(3000);  // Run for 3 second

  // Turn off the motor
  digitalWrite(D5, 0);  // Turn off
  Serial.println("Motor OFF!");
  delay(3000);  // Stay off for 3 sec
}