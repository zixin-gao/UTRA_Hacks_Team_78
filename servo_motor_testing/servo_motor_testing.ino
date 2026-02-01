#include <Servo.h> // Include the Servo library

Servo myServo;     // Create a servo object to control the servo
int angle = 60;

void setup() {
  myServo.attach(11);
  // myServo.write(angle);
  for (angle = 60; angle > 40; angle -= 10) { 
    myServo.write(angle); // Set servo to angle
    delay(10000);            // Wait for the servo to reach the position
    Serial.println(angle);
  }
}

void loop() {
}
