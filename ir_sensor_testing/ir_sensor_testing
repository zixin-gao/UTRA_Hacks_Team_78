#include <Arduino.h>

// IR sensors
int leftIR  = A0;
int rightIR = A1;

// Threshold (calculated from your data)
int threshold = 501;

void setup() {
  Serial.begin(9600);

  analogWrite(enA, 0);
  analogWrite(enB, 0);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void loop() {

  int leftValue  = analogRead(leftIR);
  int rightValue = analogRead(rightIR);

  bool leftBlack  = (leftValue  < threshold);
  bool rightBlack = (rightValue < threshold);

  if (!leftBlack && !rightBlack) {
    moveForward();
  }
  else if (leftBlack && !rightBlack) {
    turnLeft();
  }
  else if (!leftBlack && rightBlack) {
    turnRight();
  }
  else {
    stopMotors();
  }
}

// ---------- MOTOR FUNCTIONS BELOW ----------

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