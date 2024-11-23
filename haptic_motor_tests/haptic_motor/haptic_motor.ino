
const int frontMotor = A7;
float userNum = 255; // To store the user input


void setup() {
  pinMode(frontMotor, OUTPUT);
  Serial.begin(9600);  // Initialize serial communication for debugging
}

void loop() {

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
  
  // Turn on the motor
  analogWrite(frontMotor, userNum);  // vibrate
  Serial.print("Front Motor ON ");
  delay(500);  // Run for 3 second


}