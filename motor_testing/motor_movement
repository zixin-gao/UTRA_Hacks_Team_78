#include <Arduino.h>

// Motor A (Left motor)
int enA = 10;
int in1 = 9;
int in2 = 8;

// Motor B (Right motor)
int enB = 5;
int in3 = 7;
int in4 = 6;

void setup() {
  // Set all motor pins as outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  stopMotors();   // safety
}

void loop() {

  // 1️⃣ Move forward
  moveForward();
  delay(2000);

  // 2️⃣ Stop
  stopMotors();
  delay(1000);

  // 3️⃣ Turn left
  turnLeft();
  delay(1500);

  // 4️⃣ Stop
  stopMotors();
  delay(1000);

  // 5️⃣ Turn right
  turnRight();
  delay(1500);

  // 6️⃣ Stop
  stopMotors();
  delay(2000);
}

// -------------------- MOTOR FUNCTIONS --------------------

void moveForward() {
  analogWrite(enA, 200);   // speed (0–255)
  analogWrite(enB, 200);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnLeft() {
  analogWrite(enA, 150);   // left motor slower
  analogWrite(enB, 200);   // right motor faster

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnRight() {
  analogWrite(enA, 200);   // left motor faster
  analogWrite(enB, 150);   // right motor slower

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void stopMotors() {
  analogWrite(enA, 0);
  analogWrite(enB, 0);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}