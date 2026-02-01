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

int normal_speed = 60; 
int ramp_speed = 180;
int current_speed = 60;

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

  // --- PHASE 0: FIND FORK (RED/GREEN OVERLAP) ---
  if (phase == 0) {
    // If we see Red or Green values dropping (Detection of the fork tip)
    if ((g < 55 || r < 45) && b > 40) { 
      Serial.println("Fork detected! Clearing overlap...");
      
      // STEP 1: Move forward slightly to get the sensor past the Red overlap
      moveForward();
      delay(250); 
      
      // STEP 2: Now turn right to find the Green line
      Serial.println("Pivoting to Green...");
      executePivotRight(); 
      
      phase = 1; 
    } else {
      lineFollowIR(); // Keep following black until the fork
    }
  } 

  // --- PHASE 1: STAY ON GREEN (COLOR TRACING) ---
  else if (phase == 1) {
    // If sensor sees Green, stay the course
    if (g < 55) { 
      moveForward();
    } 
    // If it sees Black again (the ramp transition)
    else if (r > 80 && g > 80 && b > 70) { 
      Serial.println("Black detected! Switching to Ramp.");
      phase = 4;
    }
    // If it sees White, drift back left to find the green line
    else {
      moveLeft(); 
    }
  }

  // --- PHASE 4: RAMP (BACK TO IR) ---
  else if (phase == 4) {
    current_speed = ramp_speed;
    lineFollowIR();
  }
}

// ---------- MOVEMENT FUNCTIONS ----------

void executePivotRight() {
  // Pivot until the color sensor sees Green
  analogWrite(enLeft, 80); analogWrite(enRight, 80);
  digitalWrite(inLeft1, HIGH); digitalWrite(inLeft2, LOW);  // Left Forward
  digitalWrite(inRight1, LOW); digitalWrite(inRight2, HIGH); // Right Backward
  
  // Pivot until Green is found
  unsigned long startTime = millis();
  while(getGreen() > 65) {
    // Safety timeout: if it spins for more than 1.5s, stop (avoids infinite spinning)
    if (millis() - startTime > 1500) break; 
  }
  stopMotors();
}

void lineFollowIR() {
  bool leftWhite = (analogRead(leftIR) < threshold);
  bool rightWhite = (analogRead(rightIR) < threshold);

  if (leftWhite && rightWhite) moveForward();
  else if (leftWhite && !rightWhite) moveRight();
  else if (!leftWhite && rightWhite) moveLeft();
  else stopMotors();
}

void moveForward() {
  analogWrite(enLeft, current_speed); analogWrite(enRight, current_speed);
  digitalWrite(inLeft1, HIGH); digitalWrite(inLeft2, LOW);
  digitalWrite(inRight1, HIGH); digitalWrite(inRight2, LOW);
}

void moveLeft() {
  // Stronger turn to stay on the colored tape
  analogWrite(enLeft, 0); analogWrite(enRight, current_speed);
  digitalWrite(inLeft1, HIGH); digitalWrite(inLeft2, LOW);
  digitalWrite(inRight1, HIGH); digitalWrite(inRight2, LOW);
}

void moveRight() {
  analogWrite(enLeft, current_speed); analogWrite(enRight, 0);
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