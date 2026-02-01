#include <Arduino.h>

int enA = 10;
int in1 = 9;
int in2 = 8;

int enB = 5;
int in3 = 7;
int in4 = 6;

void setup() {
  // Set all the motor control pins to outputs
    pinMode(enA, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);

    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);

    analogWrite(enA, 255); // Set motor A speed to maximum
    digitalWrite(in1, HIGH); // Set motor A direction forward
    digitalWrite(in2, LOW);
    
    analogWrite(enB, 255); // Set motor B speed to maximum
    digitalWrite(in3, HIGH); // Set motor B direction forward
    digitalWrite(in4, LOW);
    delay(10000); // Run motors for 2 seconds

}

void loop(){
    // analogWrite(enA, 255); // Set motor A speed to maximum
    // digitalWrite(in1, HIGH); // Set motor A direction forward
    // digitalWrite(in2, LOW);
    
    // analogWrite(enB, 255); // Set motor B speed to maximum
    // digitalWrite(in3, HIGH); // Set motor B direction forward
    // digitalWrite(in4, LOW);
    // delay(10000); // Run motors for 2 seconds
}