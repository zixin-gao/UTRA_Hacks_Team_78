#include <Arduino.h>

// --- PIN MAPPING ---
#define S0 4
#define S1 11
#define S2 12
#define S3 13
#define sensorOut 2

int enLeft = 5; int inLeft1 = 7; int inLeft2 = 6;
int enRight = 10; int inRight1 = 9; int inRight2 = 8;

int leftIR = A0; int rightIR = A1;
int threshold = 501; 

int normal_speed = 50; 
int ramp_speed = 180;
int current_speed = 50;

int phase = 0; 

void setup() {
  Serial.begin(9600);
  pinMode(S0, OUTPUT); pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT); pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  digitalWrite(S0, HIGH); digitalWrite(S1, LOW);

  pinMode(enLeft, OUTPUT); pinMode(inLeft1, OUTPUT); pinMode(inLeft2, OUTPUT);
  pinMode(enRight, OUTPUT); pinMode(inRight1, OUTPUT); pinMode(inRight2, OUTPUT);

  delay(5000); 
}

void loop() {
  int r = getRed();
  int g = getGreen();
  int b = getBlue();

  // --- PHASE 0: FIND FORK ---
  if (phase == 0) {
    // NEW LOGIC: Only trigger if Green or Red is much stronger than Blue.
    // If Blue is strong (b is a small number), this condition will fail.
    if ((g < b - 20 || r < b - 20) && b > 50) { 
      delay(50); 
      if (getGreen() < 100) { 
        Serial.println("Fork Confirmed (Not Blue). Clearing overlap...");
        moveForward();
        delay(350); 
        
        Serial.println("Pivoting Right toward Green path...");
        executePivotRight(650); 
        
        phase = 1; 
      }
    } else {
      // If it sees Blue, it stays here and just follows the line
      lineFollowIR(); 
    }
  } 

  // --- PHASE 1: STAY ON GREEN ---
  else if (phase == 1) {
    if (g < b && g < r) { 
      lineFollowIR(); 
    } 
    else if (g > 120 && r > 120) { // Values increased to ensure it doesn't drop phase early
      Serial.println("Black/Ramp detected.");
      phase = 4;
    }
    else {
      moveLeft(); 
    }
  }

  // --- PHASE 4: RAMP ---
  else if (phase == 4) {
    current_speed = ramp_speed;
    lineFollowIR();
  }
}

// ---------- MOVEMENT FUNCTIONS ----------

void executePivotRight(int duration) {
  analogWrite(enLeft, 70); analogWrite(enRight, 70);
  digitalWrite(inLeft1, HIGH); digitalWrite(inLeft2, LOW);
  digitalWrite(inRight1, LOW); digitalWrite(inRight2, HIGH);
  delay(duration);
  stopMotors();
}

void lineFollowIR() {
  bool leftWhite = (analogRead(leftIR) < threshold);
  bool rightWhite = (analogRead(rightIR) < threshold);

  if (leftWhite && rightWhite) moveForward();
  else if (leftWhite && !rightWhite) moveRight();
  else if (!leftWhite && rightWhite) moveLeft();
  else {
    if (phase == 4) moveForward(); 
    else stopMotors();
  }
}

void moveForward() {
  analogWrite(enLeft, current_speed); analogWrite(enRight, current_speed);
  digitalWrite(inLeft1, HIGH); digitalWrite(inLeft2, LOW);
  digitalWrite(inRight1, HIGH); digitalWrite(inRight2, LOW);
}

void moveLeft() {
  analogWrite(enLeft, 40); analogWrite(enRight, current_speed + 15);
  digitalWrite(inLeft1, HIGH); digitalWrite(inLeft2, LOW);
  digitalWrite(inRight1, HIGH); digitalWrite(inRight2, LOW);
}

void moveRight() {
  analogWrite(enLeft, current_speed + 15); analogWrite(enRight, 40);
  digitalWrite(inLeft1, HIGH); digitalWrite(inLeft2, LOW);
  digitalWrite(inRight1, HIGH); digitalWrite(inRight2, LOW);
}

void stopMotors() {
  analogWrite(enLeft, 0); analogWrite(enRight, 0);
  digitalWrite(inLeft1, LOW); digitalWrite(inLeft2, LOW);
  digitalWrite(inRight1, LOW); digitalWrite(inRight2, LOW);
}

// --- SENSOR FUNCTIONS ---
int getRed() { digitalWrite(S2, LOW); digitalWrite(S3, LOW); return pulseIn(sensorOut, LOW); }
int getGreen() { digitalWrite(S2, HIGH); digitalWrite(S3, HIGH); return pulseIn(sensorOut, LOW); }
int getBlue() { digitalWrite(S2, LOW); digitalWrite(S3, HIGH); return pulseIn(sensorOut, LOW); }
