#include <WiFi.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>

// Bluetooth Serial object
BluetoothSerial SerialBT;

// Motor pins (adjust based on your motor driver)
const int leftMotorPin1 = 14;
const int leftMotorPin2 = 27;
const int rightMotorPin1 = 26;
const int rightMotorPin2 = 25;
const int motorSpeed = 200; // PWM speed (0-255)

// Line following sensor pins
const int leftSensor = 34;
const int centerSensor = 35;
const int rightSensor = 32;

// LED indicator
const int ledPin = 2;

// Robot states
enum RobotState {
  IDLE,
  FOLLOWING_LINE,
  GOING_TO_TABLE,
  AT_TABLE,
  RETURNING_HOME
};

RobotState currentState = IDLE;
int targetTable = 0;
int currentTable = 0;
bool isAtHome = true;

// Table positions (adjust based on your restaurant layout)
// Each table has a distance from home in sensor readings/time
struct TablePosition {
  int number;
  int distanceFromHome; // in milliseconds of travel time
  bool isReached;
};

TablePosition tables[] = {
  {1, 5000, false},   // Table 1: 5 seconds from home
  {2, 8000, false},   // Table 2: 8 seconds from home
  {3, 12000, false},  // Table 3: 12 seconds from home
  {4, 15000, false},  // Table 4: 15 seconds from home
  {5, 18000, false},  // Table 5: 18 seconds from home
  {6, 22000, false},  // Table 6: 22 seconds from home
  {7, 25000, false},  // Table 7: 25 seconds from home
  {8, 28000, false},  // Table 8: 28 seconds from home
  {9, 32000, false},  // Table 9: 32 seconds from home
  {10, 35000, false}  // Table 10: 35 seconds from home
};

const int tableCount = sizeof(tables) / sizeof(tables[0]);
unsigned long journeyStartTime = 0;

void setup() {
  Serial.begin(115200);
  
  // Initialize Bluetooth
  SerialBT.begin("SmartWaiterRobot"); // Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  
  // Initialize motor pins
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  
  // Initialize sensor pins
  pinMode(leftSensor, INPUT);
  pinMode(centerSensor, INPUT);
  pinMode(rightSensor, INPUT);
  
  // Initialize LED
  pinMode(ledPin, OUTPUT);
  
  // Start at home
  stopMotors();
  digitalWrite(ledPin, HIGH);
  
  Serial.println("Smart Waiter Robot Ready!");
}

void loop() {
  // Check for Bluetooth commands
  if (SerialBT.available()) {
    String command = SerialBT.readStringUntil('\n');
    command.trim();
    processCommand(command);
  }
  
  // Execute current state behavior
  switch (currentState) {
    case IDLE:
      stopMotors();
      break;
      
    case FOLLOWING_LINE:
      followLine();
      checkTableArrival();
      break;
      
    case GOING_TO_TABLE:
      followLine();
      checkTableArrival();
      break;
      
    case AT_TABLE:
      stopMotors();
      digitalWrite(ledPin, LOW);
      delay(100);
      digitalWrite(ledPin, HIGH);
      delay(100);
      break;
      
    case RETURNING_HOME:
      followLineHome();
      checkHomeArrival();
      break;
  }
  
  delay(50); // Small delay for stability
}

void processCommand(String command) {
  Serial.println("Received command: " + command);
  
  // Parse JSON command
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, command);
  
  String cmd = doc["command"];
  
  if (cmd == "go_to_table") {
    int tableNumber = doc["table_number"];
    goToTable(tableNumber);
  }
  else if (cmd == "return_home") {
    returnHome();
  }
  else if (cmd == "stop") {
    stopRobot();
  }
  else if (cmd == "status") {
    sendStatus();
  }
  
  // Send acknowledgment
  DynamicJsonDocument response(512);
  response["status"] = "received";
  response["command"] = cmd;
  String responseStr;
  serializeJson(response, responseStr);
  SerialBT.println(responseStr);
}

void goToTable(int tableNumber) {
  targetTable = tableNumber;
  currentState = GOING_TO_TABLE;
  isAtHome = false;
  journeyStartTime = millis();
  
  Serial.println("Going to table " + String(tableNumber));
  
  // Send status update
  DynamicJsonDocument status(512);
  status["status"] = "moving";
  status["target_table"] = tableNumber;
  status["current_position"] = "en_route";
  String statusStr;
  serializeJson(status, statusStr);
  SerialBT.println(statusStr);
}

void returnHome() {
  currentState = RETURNING_HOME;
  targetTable = 0;
  journeyStartTime = millis();
  
  Serial.println("Returning home");
  
  // Send status update
  DynamicJsonDocument status(512);
  status["status"] = "returning";
  status["target"] = "home";
  String statusStr;
  serializeJson(status, statusStr);
  SerialBT.println(statusStr);
}

void stopRobot() {
  currentState = IDLE;
  stopMotors();
  
  Serial.println("Robot stopped");
}

void followLine() {
  int leftVal = digitalRead(leftSensor);
  int centerVal = digitalRead(centerSensor);
  int rightVal = digitalRead(rightSensor);
  
  // Line following logic (assuming sensors return LOW when detecting line)
  if (centerVal == LOW) {
    // On line - go straight
    moveForward();
  }
  else if (leftVal == LOW) {
    // Line is to the left - turn left
    turnLeft();
  }
  else if (rightVal == LOW) {
    // Line is to the right - turn right
    turnRight();
  }
  else {
    // No line detected - stop or continue last direction
    moveForward(); // or implement search pattern
  }
}

void followLineHome() {
  // Similar to followLine but might have different logic for returning
  followLine();
}

void checkTableArrival() {
  if (targetTable > 0 && targetTable <= tableCount) {
    unsigned long travelTime = millis() - journeyStartTime;
    int targetDistance = tables[targetTable - 1].distanceFromHome;
    
    // Check if we've traveled long enough to reach the table
    if (travelTime >= targetDistance) {
      arrivedAtTable();
    }
  }
}

void checkHomeArrival() {
  unsigned long travelTime = millis() - journeyStartTime;
  
  // Assume it takes similar time to return home
  if (travelTime >= 10000) { // 10 seconds to return home
    arrivedAtHome();
  }
}

void arrivedAtTable() {
  currentState = AT_TABLE;
  currentTable = targetTable;
  stopMotors();
  
  Serial.println("Arrived at table " + String(targetTable));
  
  // Send arrival notification
  DynamicJsonDocument status(512);
  status["status"] = "arrived";
  status["table_number"] = targetTable;
  status["current_position"] = "table_" + String(targetTable);
  String statusStr;
  serializeJson(status, statusStr);
  SerialBT.println(statusStr);
}

void arrivedAtHome() {
  currentState = IDLE;
  isAtHome = true;
  currentTable = 0;
  stopMotors();
  
  Serial.println("Arrived at home");
  
  // Send home arrival notification
  DynamicJsonDocument status(512);
  status["status"] = "home";
  status["current_position"] = "home";
  String statusStr;
  serializeJson(status, statusStr);
  SerialBT.println(statusStr);
}

void sendStatus() {
  DynamicJsonDocument status(512);
  status["state"] = getStateString();
  status["current_table"] = currentTable;
  status["target_table"] = targetTable;
  status["is_at_home"] = isAtHome;
  
  String statusStr;
  serializeJson(status, statusStr);
  SerialBT.println(statusStr);
}

String getStateString() {
  switch (currentState) {
    case IDLE: return "idle";
    case FOLLOWING_LINE: return "following_line";
    case GOING_TO_TABLE: return "going_to_table";
    case AT_TABLE: return "at_table";
    case RETURNING_HOME: return "returning_home";
    default: return "unknown";
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
