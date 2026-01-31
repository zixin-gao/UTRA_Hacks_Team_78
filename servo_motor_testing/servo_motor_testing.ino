#include <Servo.h> // Include the Servo library

Servo myServo;     // Create a servo object to control the servo
int angle = 0;     // Variable to store the servo angle

void setup() {
  myServo.attach(9); // Attach the servo to pin 9
  for (angle = 0; angle <= 45; angle += 1) { 
    myServo.write(angle); // Set servo to angle
    delay(15);            // Wait for the servo to reach the position
  }
}

void loop() {
  // Sweep from 0 to 180 degrees
}
