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

int normal_speed = 50;  // Dropped slightly for better color reading
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
    // Logic: Look for when Green or Red becomes the strongest color
    // We compare Green/Red to Blue because White/Black have high Blue values
    if ((g < b - 15 || r < b - 15) && g < 100) { 
      delay(50); // Filter: wait a tiny bit...
      if (getGreen() < 100) { // ...and check again to confirm it's not noise
        Serial.println("Fork Confirmed. Clearing overlap...");
        moveForward();
        delay(350); // Drive onto the "messy" junction center
        
        Serial.println("Pivoting Right toward Green path...");
        executePivotRight(650); // Fixed time pivot is more reliable than sensor loops here
        
        phase = 1; 
      }
    } else {
      lineFollowIR(); 
    }
  } 

  // --- PHASE 1: STAY ON GREEN (HYBRID TRACING) ---
  else if (phase == 1) {
    // If Green is still dominant
    if (g < b && g < r) { 
      // If we are on the line, follow it with IR
      lineFollowIR(); 
    } 
    // If we see Black (Transition to ramp)
    else if (g > 100 && r > 100) { 
      Serial.println("Black/Ramp detected.");
      phase = 4;
    }
    // If we see White, it means we missed the green. 
    // Since we turned RIGHT, we correction steer LEFT.
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
  // Pivot on the spot
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
    // On the ramp, don't stop if sensors get confused
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
  analogWrite(enLeft, 40); analogWrite(enRight, current_speed + 10);
  digitalWrite(inLeft1, HIGH); digitalWrite(inLeft2, LOW);
  digitalWrite(inRight1, HIGH); digitalWrite(inRight2, LOW);
}

void moveRight() {
  analogWrite(enLeft, current_speed + 10); analogWrite(enRight, 40);
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
