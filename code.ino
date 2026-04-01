#include <NewPing.h>

bool start = false;
const int trigPin = 13;
const int echoPin = 12;
const int maxDistance = 20;

NewPing sonar(trigPin, echoPin, maxDistance);

// --- PIN DEFINITIONS ---
const int leftIR = A0;  
const int rightIR = A3; // Changed from A1 to A3 per your wiring

void setup() {
  pinMode(5, OUTPUT); 
  pinMode(6, OUTPUT); 
  pinMode(7, OUTPUT); 
  pinMode(8, OUTPUT); 

  pinMode(leftIR, INPUT); 
  pinMode(rightIR, INPUT); 
  
  Serial.begin(9600); 
  Serial.println("System Initialized. Pins: A0(L), A3(R). Send 'x' to start.");
}

void Forward() {
  digitalWrite(5, HIGH); digitalWrite(6, LOW);
  digitalWrite(7, LOW);  digitalWrite(8, HIGH);
}

void Stop() {
  digitalWrite(5, LOW); digitalWrite(6, LOW);
  digitalWrite(7, LOW); digitalWrite(8, LOW);
}

void Left() {
  digitalWrite(5, HIGH); digitalWrite(6, LOW); // Right wheel moves
  digitalWrite(7, LOW);  digitalWrite(8, LOW);  // Left wheel stops
}

void Right() {
  digitalWrite(5, LOW);  digitalWrite(6, LOW);  // Right wheel stops
  digitalWrite(7, LOW);  digitalWrite(8, HIGH); // Left wheel moves
}

void loop() {
  if (Serial.available()) {
    char a = Serial.read();
    if (a == 'x' || a == 'X') {
      start = true;
      Serial.println("START COMMAND RECEIVED");
    }
  }

  if (start) { 
    int d0 = digitalRead(leftIR); 
    int d1 = digitalRead(rightIR); 

    // --- DEBUGGING: Uncomment the lines below to see sensor values in Serial Monitor ---
    // Serial.print("L: "); Serial.print(d0); 
    // Serial.print(" | R: "); Serial.println(d1);

    int distanceCm = sonar.ping_cm();
    
    if (distanceCm > 0 && distanceCm < 15) {
      Stop();
    } 
    else {
      // LINE FOLLOWING LOGIC
      if (d0 == 0 && d1 == 0) {
        Forward();
      }
      else if (d0 == 0) { // Left sensor hits line
        Left();
      }
      else if (d1 == 0) { // Right sensor hits line
        Right();
      }
      else {
        Forward(); 
      }
    }
  }
}
