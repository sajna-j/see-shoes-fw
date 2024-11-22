const int sideMotor = D5;  // Use a PWM-capable pin (e.g., 3, 5, 6, 9, 10, or 11)
const int frontMotor = D4;
float userNum = 255; // To store the user input


void setup() {
  pinMode(sideMotor, OUTPUT);  // Set the motor pin as an output
  pinMode(frontMotor, OUTPUT);
  Serial.begin(9600);  // Initialize serial communication for debugging
}

void loop() {
  /*
  if (Serial.available() > 0) {
    // Read the user input
    userNum = Serial.parseFloat(); // Read the integer input

    // Clear the Serial buffer to discard trailing characters
    while (Serial.available() > 0) {
      Serial.read();
    }

    Serial.print("You entered: ");
    Serial.println(userNum); // Echo the input back to the Serial Monitor
  }
8*/
  //Serial.print("User num is: ");
  //Serial.println(userNum); // Echo the input back to the Serial Monitor
  // Turn on the motor
  digitalWrite(sideMotor, HIGH);  // vibrate
  Serial.print("Side Motor ON ");
  //Serial.println(userNum);
  delay(3000);  // Run for 3 second

  // Turn off the motor
  digitalWrite(sideMotor, LOW);  // Turn off
  Serial.println("Side Motor OFF!");
  delay(3000);  // Stay off for 3 sec

  // Turn on the motor
  digitalWrite(frontMotor, HIGH);  // vibrate
  Serial.print("Front Motor ON ");
  delay(3000);  // Run for 3 second

  // Turn off the motor
  digitalWrite(frontMotor, LOW);  // Turn off
  Serial.println("Front Motor OFF!");
  delay(3000);  // Stay off for 3 sec

}