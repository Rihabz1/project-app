// Smart Waiter Robot - Simple Version (No External Libraries)
// This version uses only built-in Arduino libraries for easy testing

// Include Arduino headers (use local stubs for VS Code IntelliSense)
#ifdef __INTELLISENSE__
#include ".vscode/Arduino.h"
#include ".vscode/SoftwareSerial.h"
#else
#include <SoftwareSerial.h>
#endif

// Bluetooth Serial object
SoftwareSerial bluetooth(2, 3); // RX, TX pins

// Motor pins (Arduino Uno compatible PWM pins)
const int leftMotorPin1 = 5;   // PWM pin
const int leftMotorPin2 = 6;   // PWM pin  
const int rightMotorPin1 = 9;  // PWM pin
const int rightMotorPin2 = 10; // PWM pin
const int motorSpeed = 200;    // PWM speed (0-255)

// Line following sensor pins
const int leftSensor = A0;     // Analog pin A0
const int centerSensor = A1;   // Analog pin A1  
const int rightSensor = A2;    // Analog pin A2

// LED indicator
const int ledPin = 13;         // Built-in LED

// Robot states
enum RobotState {
  IDLE,
  GOING_TO_TABLE,
  AT_TABLE,
  RETURNING_HOME
};

RobotState currentState = IDLE;
int targetTable = 0;
int currentTable = 0;
bool isAtHome = true;

// Simple table timing (in milliseconds)
int tableDistances[] = {0, 3000, 5000, 7000, 9000, 11000}; // Tables 0-5
unsigned long journeyStartTime = 0;

void setup() {
  Serial.begin(9600);
  
  // Initialize Bluetooth
  bluetooth.begin(9600);
  Serial.println("Simple Smart Waiter Robot Ready!");
  Serial.println("Commands: GO1-GO5 (go to table), HOME (return), STOP, STATUS");
  
  // Initialize motor pins
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  
  // Initialize sensor pins (analog, no pinMode needed)
  
  // Initialize LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // Robot ready indicator
  
  Serial.println("Waiting for commands...");
}

void loop() {
  // Check for commands from Bluetooth or Serial
  if (bluetooth.available()) {
    String command = bluetooth.readStringUntil('\n');
    command.trim();
    processSimpleCommand(command);
  }
  
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    processSimpleCommand(command);
  }
  
  // Execute current state behavior
  switch (currentState) {
    case IDLE:
      stopMotors();
      digitalWrite(ledPin, HIGH);
      break;
      
    case GOING_TO_TABLE:
      followLine();
      checkTableArrival();
      break;
      
    case AT_TABLE:
      stopMotors();
      blinkLED(); // Indicate arrived at table
      break;
      
    case RETURNING_HOME:
      followLine();
      checkHomeArrival();
      break;
  }
  
  delay(50);
}

void processSimpleCommand(String command) {
  command.toUpperCase(); // Make case insensitive
  Serial.print("Received: ");
  Serial.println(command);
  
  if (command.startsWith("GO")) {
    // Extract table number (GO1, GO2, etc.)
    int tableNum = command.substring(2).toInt();
    if (tableNum >= 1 && tableNum <= 5) {
      goToTable(tableNum);
    } else {
      sendResponse("ERROR: Invalid table number (1-5)");
    }
  }
  else if (command.equals("HOME")) {
    returnHome();
  }
  else if (command.equals("STOP")) {
    stopRobot();
  }
  else if (command.equals("STATUS")) {
    sendStatus();
  }
  else {
    sendResponse("ERROR: Unknown command");
    sendResponse("Available: GO1-GO5, HOME, STOP, STATUS");
  }
}

void goToTable(int tableNumber) {
  targetTable = tableNumber;
  currentState = GOING_TO_TABLE;
  isAtHome = false;
  journeyStartTime = millis();
  
  Serial.print("Moving to table ");
  Serial.println(tableNumber);
  
  bluetooth.print("Moving to table ");
  bluetooth.println(tableNumber);
}

void returnHome() {
  currentState = RETURNING_HOME;
  targetTable = 0;
  journeyStartTime = millis();
  
  sendResponse("Returning home");
  Serial.println("Returning home");
}

void stopRobot() {
  currentState = IDLE;
  stopMotors();
  
  sendResponse("Robot stopped");
  Serial.println("Robot stopped");
}

void sendStatus() {
  String state = getStateString();
  
  Serial.print("Status: State=");
  Serial.print(state);
  Serial.print(", Table=");
  Serial.print(currentTable);
  Serial.print(", Home=");
  Serial.println(isAtHome ? "Yes" : "No");
  
  bluetooth.print("State: ");
  bluetooth.print(state);
  bluetooth.print(", Table: ");
  bluetooth.print(currentTable);
  bluetooth.print(", Home: ");
  bluetooth.println(isAtHome ? "Yes" : "No");
}

void sendResponse(String message) {
  bluetooth.println(message);
  Serial.print("Sent: ");
  Serial.println(message);
}

String getStateString() {
  switch (currentState) {
    case IDLE: return "IDLE";
    case GOING_TO_TABLE: return "MOVING";
    case AT_TABLE: return "AT_TABLE";
    case RETURNING_HOME: return "RETURNING";
    default: return "UNKNOWN";
  }
}

void followLine() {
  // Read analog sensor values
  int leftVal = analogRead(leftSensor);
  int centerVal = analogRead(centerSensor);
  int rightVal = analogRead(rightSensor);
  
  // Simple threshold (adjust based on your sensors)
  int threshold = 512; // Middle value for 10-bit ADC
  
  bool leftDetected = leftVal < threshold;
  bool centerDetected = centerVal < threshold;
  bool rightDetected = rightVal < threshold;
  
  // Line following logic
  if (centerDetected) {
    moveForward();
  }
  else if (leftDetected) {
    turnLeft();
  }
  else if (rightDetected) {
    turnRight();
  }
  else {
    // No line detected - stop and search
    stopMotors();
    delay(100);
    // Could add search pattern here
  }
}

void checkTableArrival() {
  if (targetTable > 0 && targetTable <= 5) {
    unsigned long travelTime = millis() - journeyStartTime;
    
    if (travelTime >= tableDistances[targetTable]) {
      arrivedAtTable();
    }
  }
}

void checkHomeArrival() {
  unsigned long travelTime = millis() - journeyStartTime;
  
  // Simple time-based home arrival
  if (travelTime >= 8000) { // 8 seconds to return home
    arrivedAtHome();
  }
}

void arrivedAtTable() {
  currentState = AT_TABLE;
  currentTable = targetTable;
  stopMotors();
  
  Serial.print("Arrived at table ");
  Serial.println(targetTable);
  
  bluetooth.print("Arrived at table ");
  bluetooth.println(targetTable);
}

void arrivedAtHome() {
  currentState = IDLE;
  isAtHome = true;
  currentTable = 0;
  stopMotors();
  
  sendResponse("Arrived home");
  Serial.println("Arrived home");
}

void blinkLED() {
  static unsigned long lastBlink = 0;
  static bool ledState = false;
  
  if (millis() - lastBlink > 500) {
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    lastBlink = millis();
  }
}

// Motor control functions
void moveForward() {
  analogWrite(leftMotorPin1, motorSpeed);
  analogWrite(leftMotorPin2, 0);
  analogWrite(rightMotorPin1, motorSpeed);
  analogWrite(rightMotorPin2, 0);
}

void moveBackward() {
  analogWrite(leftMotorPin1, 0);
  analogWrite(leftMotorPin2, motorSpeed);
  analogWrite(rightMotorPin1, 0);
  analogWrite(rightMotorPin2, motorSpeed);
}

void turnLeft() {
  analogWrite(leftMotorPin1, 0);
  analogWrite(leftMotorPin2, motorSpeed/2);
  analogWrite(rightMotorPin1, motorSpeed);
  analogWrite(rightMotorPin2, 0);
}

void turnRight() {
  analogWrite(leftMotorPin1, motorSpeed);
  analogWrite(leftMotorPin2, 0);
  analogWrite(rightMotorPin1, 0);
  analogWrite(rightMotorPin2, motorSpeed/2);
}

void stopMotors() {
  analogWrite(leftMotorPin1, 0);
  analogWrite(leftMotorPin2, 0);
  analogWrite(rightMotorPin1, 0);
  analogWrite(rightMotorPin2, 0);
}
