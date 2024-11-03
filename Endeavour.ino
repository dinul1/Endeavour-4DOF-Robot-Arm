// Made by Dinul Sasnada for Endeavour - 4DOF Robot Arm. All rights reserved.
// Post-Development Reset

#include <Servo.h>

Servo baseServo, shoulderServo, elbowServo, wristServo;

int basePos = 90;       
int shoulderPos = 90;   
int elbowPos = 90;      
int wristPos = 90;       

const int BASE_MIN = 0, BASE_MAX = 180;
const int SHOULDER_MIN = 0, SHOULDER_MAX = 180;
const int ELBOW_MIN = 0, ELBOW_MAX = 180;
const int WRIST_MIN = 0, WRIST_MAX = 180;

unsigned long previousMillis = 0; // Store the last time the servo was updated
const int updateInterval = 20;     // Interval for servo movement update
int targetBasePos, targetShoulderPos, targetElbowPos, targetWristPos;

// Track the movement state
bool isMovingBase = false, isMovingShoulder = false, isMovingElbow = false, isMovingWrist = false;

void moveServoSmoothly(Servo &servo, int &currentPos, int targetPos, int step = 1) {
  targetPos = constrain(targetPos, 0, 180);
  if (currentPos != targetPos) {
    if (abs(currentPos - targetPos) <= step) {
      currentPos = targetPos; // Snap to target
    } else {
      currentPos += (currentPos < targetPos) ? step : -step;
    }
    servo.write(currentPos);
  }
}

void moveBase(int delta) {
  targetBasePos = constrain(basePos + delta, BASE_MIN, BASE_MAX);
  isMovingBase = true;
}

void moveShoulder(int delta) {
  targetShoulderPos = constrain(shoulderPos + delta, SHOULDER_MIN, SHOULDER_MAX);
  isMovingShoulder = true;
}

void moveElbow(int delta) {
  targetElbowPos = constrain(elbowPos + delta, ELBOW_MIN, ELBOW_MAX);
  isMovingElbow = true;
}

void moveWrist(int delta) {
  targetWristPos = constrain(wristPos + delta, WRIST_MIN, WRIST_MAX);
  isMovingWrist = true;
}

void P() {
  Serial.write(wristPos);
  Serial.write(elbowPos);
  Serial.write(shoulderPos);
  Serial.write(basePos);
}

void R() {
  // Write to servos directly to reset
  moveBase (90 - basePos);
  moveShoulder(90 - shoulderPos);
  moveElbow(90 - elbowPos);
  moveWrist(90 - wristPos);
}

void K() {
  baseServo.detach();
  shoulderServo.detach();
  elbowServo.detach();
  wristServo.detach();

}

void Q() {
  baseServo.attach(8);
  shoulderServo.attach(9);
  elbowServo.attach(10);
  wristServo.attach(11);
}

void setup() { 
  Serial.begin(9600);
    
  baseServo.attach(8);
  shoulderServo.attach(9);
  elbowServo.attach(10);
  wristServo.attach(11);

  // Move servos smoothly to their initial positions
  moveServoSmoothly(baseServo, basePos, 90);
  moveServoSmoothly(shoulderServo, shoulderPos, 90);
  moveServoSmoothly(elbowServo, elbowPos, 90);
  moveServoSmoothly(wristServo, wristPos, 90);

  Serial.println("Endeavour - 4DOF Robot Arm Ready!");
}

void loop() { 
  if (Serial.available() > 0) {
    char value = Serial.read();  
    switch (value) {
      case 'A': moveBase(-10); break;
      case 'B': moveBase(10); break;
      case 'C': moveShoulder(-5); break;
      case 'D': moveShoulder(5); break;
      case 'E': moveElbow(-5); break;
      case 'F': moveElbow(5); break;
      case 'G': moveWrist(-5); break;
      case 'H': moveWrist(5); break;
      case 'K': K(); break;
      case 'M': moveWrist(180 - wristPos); break;
      case 'P': P(); break;
      case 'Q': Q(); break;
      case 'R': R(); break;
      case 'S': moveBase(180 - basePos); break;
      case 'T': moveShoulder(180 - shoulderPos); break;
      case 'U': moveElbow(180 - elbowPos); break;
      case 'V': moveBase(basePos = 0); break;
      case 'X': moveShoulder(shoulderPos = 60); break;
      case 'Y': moveElbow(elbowPos = 0); break;
      case 'Z': moveWrist(wristPos = 0); break;
      case 'a': baseServo.detach(); break;
      case 'b': shoulderServo.detach(); break;
      case 'c': elbowServo.detach(); break;
      case 'd': wristServo.detach(); break;
      case 'e': baseServo.attach(8); break;
      case 'f': shoulderServo.attach(9); break;
      case 'g': elbowServo.attach(10); break;
      case 'h': wristServo.attach(11); break;
      case 'i': moveBase(90 - basePos); break;
      case 'j': moveShoulder(90 - shoulderPos); break;
      case 'k': moveElbow(90 - elbowPos); break;
      case 'l': moveWrist(90 - wristPos); break;
      default: Serial.println("Invalid Command..."); break;
    }
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= updateInterval) {
    previousMillis = currentMillis; // Update the last update time

    // Move servos smoothly
    if (isMovingBase) {
      moveServoSmoothly(baseServo, basePos, targetBasePos);
      if (basePos == targetBasePos) isMovingBase = false;
    }
    if (isMovingShoulder) {
      moveServoSmoothly(shoulderServo, shoulderPos, targetShoulderPos);
      if (shoulderPos == targetShoulderPos) isMovingShoulder = false;
    }
    if (isMovingElbow) {
      moveServoSmoothly(elbowServo, elbowPos, targetElbowPos);
      if (elbowPos == targetElbowPos) isMovingElbow = false;
    }
    if (isMovingWrist) {
      moveServoSmoothly(wristServo, wristPos, targetWristPos);
      if (wristPos == targetWristPos) isMovingWrist = false;
    }
  }
}
