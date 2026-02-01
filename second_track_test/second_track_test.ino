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

// --- SPEED SETTINGS ---
int normal_speed = 50; 
int ramp_speed = 180;   // Increased for the incline
int turn_speed = 30;    
int current_speed = 50;

// PHASES: 0=Black Line, 1=Green Path, 4=Ramp
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

  // --- PHASE 0: FIND GREEN FORK ---
  if (phase == 0) {
    current_speed = normal_speed;
    // Detect Green (Strongest color and below threshold)
    if (g < r && g < b && g < 55) { 
      Serial.println("Green Fork Found! Aligning...");
      smartAlignRight(); // Fixed: Replaces hardTurnRight(delay)
      phase = 1;
    } else {
      lineFollow();
    }
  } 
  
  // --- PHASE 1: FOLLOW GREEN UNTIL BLACK (RAMP START) ---
  else if (phase == 1) {
    current_speed = normal_speed;
    
    // We stay in Phase 1 as long as we see Green. 
    // If Green disappears (meaning we are back on Black or at the Ramp), switch to Ramp Phase.
    if (g > 100 && r > 100) { 
      Serial.println("Green ended. Switching to Ramp Phase.");
      phase = 4;
    } else {
      lineFollow(); // Continue following the green line
    }
  }

  // --- PHASE 4: THE RAMP ---
  else if (phase == 4) {
    current_speed = ramp_speed;
    lineFollow();
  }
}

// ---------- IMPROVED MOVEMENT FUNCTIONS ----------

void lineFollow() {
  bool leftWhite = (analogRead(leftIR) < threshold);
  bool rightWhite = (analogRead(rightIR) < threshold);

  if (leftWhite && rightWhite) {
    moveForward();
  } else if (leftWhite && !rightWhite) {
    moveRight();
  } else if (!leftWhite && rightWhite) {
    moveLeft();
  } else {
    if (phase == 4) moveForward(); // Ramp safety
    else stopMotors();
  }
}

// Fixed: Pivots until the sensor actually finds the line
// void smartAlignRight() {
//   // 1. Kickstart the turn to get off the current line
//   analogWrite(enLeft, 60); analogWrite(enRight, 60);
//   digitalWrite(inLeft1, HIGH); digitalWrite(inLeft2, LOW);
//   digitalWrite(inRight1, LOW); digitalWrite(inRight2, HIGH);
//   delay(300); // Small initial burst to move away from black line

//   // 2. Keep pivoting until the Left IR sensor hits the green/new line
//   while(analogRead(leftIR) < threshold) {
//     // Keep spinning
//   }
  
//   stopMotors();
//   Serial.println("Aligned with new path!");
// }

void smartAlignRight() {
  // 1. Kickstart the turn to get off the black line
  analogWrite(enLeft, 70); analogWrite(enRight, 70);
  digitalWrite(inLeft1, HIGH); digitalWrite(inLeft2, LOW);
  digitalWrite(inRight1, LOW); digitalWrite(inRight2, HIGH);
  
  // Give it a moment to move so it doesn't immediately "re-detect" the old spot
  delay(400); 

  // 2. Keep pivoting until the Color Sensor sees Green again
  // We use a "while NOT green" loop
  while(getGreen() > 50) { 
    // Just keep spinning...
    // This ensures we stop exactly when the color sensor is over the green tape
  }
  
  stopMotors();
  Serial.println("Green Path Locked!");
}

void moveForward() {
  analogWrite(enLeft, current_speed); analogWrite(enRight, current_speed);
  digitalWrite(inLeft1, HIGH); digitalWrite(inLeft2, LOW);
  digitalWrite(inRight1, HIGH); digitalWrite(inRight2, LOW);
}

void moveLeft() {
  analogWrite(enLeft, 0); // Harder correction for better tracking
  analogWrite(enRight, current_speed);
  digitalWrite(inLeft1, HIGH); digitalWrite(inLeft2, LOW);
  digitalWrite(inRight1, HIGH); digitalWrite(inRight2, LOW);
}

void moveRight() {
  analogWrite(enLeft, current_speed);
  analogWrite(enRight, 0);
  digitalWrite(inLeft1, HIGH); digitalWrite(inLeft2, LOW);
  digitalWrite(inRight1, HIGH); digitalWrite(inRight2, LOW);
}

void stopMotors() {
  analogWrite(enLeft, 0); analogWrite(enRight, 0);
  digitalWrite(inLeft1, LOW); digitalWrite(inLeft2, LOW);
  digitalWrite(inRight1, LOW); digitalWrite(inRight2, LOW);
}

// ---------- COLOR SENSING ----------
int getRed() { digitalWrite(S2, LOW); digitalWrite(S3, LOW); return pulseIn(sensorOut, LOW); }
int getGreen() { digitalWrite(S2, HIGH); digitalWrite(S3, HIGH); return pulseIn(sensorOut, LOW); }
int getBlue() { digitalWrite(S2, LOW); digitalWrite(S3, HIGH); return pulseIn(sensorOut, LOW); }