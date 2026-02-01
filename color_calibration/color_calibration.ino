#include <Arduino.h>

// --- TCS230 / TCS3200 Pin Mapping ---
#define S0 4
#define S1 11
#define S2 12
#define S3 13
#define sensorOut 2

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);
  
  // Setting frequency scaling to 20% (Common for Arduino)
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  
  Serial.println("--- Color Sensor Calibration Tool ---");
  Serial.println("Place sensor over Green, then Blue, and record the values.");
}

void loop() {
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  int redFrequency = pulseIn(sensorOut, LOW);
  
  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  int greenFrequency = pulseIn(sensorOut, LOW);
  
  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  int blueFrequency = pulseIn(sensorOut, LOW);

  // Print results to Serial Monitor
  Serial.print("R: ");
  Serial.print(redFrequency);
  Serial.print(" | G: ");
  Serial.print(greenFrequency);
  Serial.print(" | B: ");
  Serial.println(blueFrequency);

  delay(500); // Wait half a second between readings
}