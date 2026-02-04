#include <Arduino.h>
//tise 
// IR sensors
int leftIR  = A0;
int rightIR = A1;

// Threshold (calculated from your data)
int threshold = 501;

int enA = 10;
int in1 = 9;
int in2 = 8;

int enB = 5;
int in3 = 7;
int in4 = 6;

int full_speed = 150;
int turn_speed = 10;

void setup() {
  Serial.begin(9600);
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

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

  bool leftWhite  = (leftValue  < threshold);
  bool rightWhite = (rightValue < threshold);

  if (leftWhite && rightWhite) {
    moveForward();
  }
  else if (leftWhite && !rightWhite) {
    turnRight();
  }
  else if (!leftWhite && rightWhite) {
    turnLeft();
  }
  else {
    stopMotors();
  }
}

// ---------- MOTOR FUNCTIONS BELOW ----------

void moveForward() {
  analogWrite(enA, full_speed);   // speed (0–255)
  analogWrite(enB, full_speed);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}


void turnLeft() {
  analogWrite(enB, turn_speed);   // left motor slower
  analogWrite(enA, full_speed);   // right motor faster

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnRight() {
  analogWrite(enB, full_speed);   // left motor faster
  analogWrite(enA, turn_speed);   // right motor slower

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
