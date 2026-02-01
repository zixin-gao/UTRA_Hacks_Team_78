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
int ramp_speed = 50;   // Increased power for the incline
int turn_speed = 25;    // Increased slightly for better control
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

  delay(5000); // 5s to place on start line
}

void loop() {
  int r = getRed();
  int g = getGreen();
  int b = getBlue();

  // --- PHASE 0: LINE FOLLOWING & FINDING GREEN FORK ---
//   if (phase == 0) {
//     current_speed = normal_speed;
//     if (g < r && g < b && g < 55) { // Green detected
//       Serial.println("Green Path! Turning...");
//       hardTurnRight();
//       delay(800); 
//       phase = 1;
//     }
//   } 
// --- PHASE 0: FIND THE GREEN FORK ---
    if (phase == 0) {
        // If Green is definitely stronger than Red AND Blue
        if (g < r && g < b && g < 55) { 
            Serial.println("Confirmed Green! Turning...");
            hardTurnRight();
            delay(800); 
            phase = 1;
        } 
        // If it sees Red, we just log it and keep line following
        else if (r < g && r < b && r < 40) {
            Serial.println("Seeing Red overlap... ignoring and searching for Green.");
            lineFollow(); 
        }
        else {
            lineFollow();
        }
    }
  
  // --- PHASE 1: FIND BLUE (SKIP PICKUP -> GO TO RAMP) ---
  else if (phase == 1) {
    current_speed = normal_speed;
    if (b < r && b < g && b < 45) { // Blue detected
      Serial.println("Blue Found! Preparing for Ramp...");
      // For now, we skip U-turn and Pickup. 
      // We just pause to signify we saw blue, then transition to ramp phase.
      stopMotors();
      delay(5000); 
      phase = 4; 
    }
  }

  // --- PHASE 4: THE RAMP ---
  else if (phase == 4) {
    current_speed = ramp_speed; // Boost power for the climb
    Serial.println("Climbing Ramp!");
  }

  lineFollow(); // Run line following logic in every phase
}

// ---------- MOVEMENT FUNCTIONS ----------

void lineFollow() {
  bool leftWhite = (analogRead(leftIR) < threshold);
  bool rightWhite = (analogRead(rightIR) < threshold);

  // If both see black (or the ramp angle causes a false reading), keep pushing!
  if (leftWhite && rightWhite) {
    moveForward();
  } else if (leftWhite && !rightWhite) {
    moveRight();
  } else if (!leftWhite && rightWhite) {
    moveLeft();
  } else {
    // If we are in the Ramp Phase, don't stop just because of a weird reading
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
  analogWrite(enLeft, turn_speed); analogWrite(enRight, current_speed);
  digitalWrite(inLeft1, HIGH); digitalWrite(inLeft2, LOW);
  digitalWrite(inRight1, HIGH); digitalWrite(inRight2, LOW);
}

void moveRight() {
  analogWrite(enLeft, current_speed); analogWrite(enRight, turn_speed);
  digitalWrite(inLeft1, HIGH); digitalWrite(inLeft2, LOW);
  digitalWrite(inRight1, HIGH); digitalWrite(inRight2, LOW);
}

void hardTurnRight() {
  analogWrite(enLeft, current_speed); analogWrite(enRight, 10);
  digitalWrite(inLeft1, HIGH); digitalWrite(inLeft2, LOW);
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
