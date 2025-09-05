// Arduino stub for VS Code IntelliSense
#ifndef Arduino_h
#define Arduino_h

// Basic Arduino types and constants
typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long uint32_t;

#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define A0 14
#define A1 15
#define A2 16

// Basic Arduino functions
void pinMode(int pin, int mode);
void digitalWrite(int pin, int value);
int digitalRead(int pin);
int analogRead(int pin);
void analogWrite(int pin, int value);
unsigned long millis();
void delay(unsigned long ms);

// Forward declarations for project functions (only declare, don't define)
void followLine();
void checkTableArrival();
void checkHomeArrival();
void blinkLED();
void sendResponse(String message);
void returnHome();
void stopRobot();
void sendStatus();
void goToTable(int tableNumber);
void arrivedAtTable();
void arrivedAtHome();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void stopMotors();
void processCommand(String command);
void processSimpleCommand(String command);

// Serial class
class HardwareSerial {
public:
    void begin(long speed);
    int available();
    int read();
    String readStringUntil(char terminator);
    void print(const char* str);
    void print(String str);
    void print(int num);
    void println(const char* str);
    void println(String str);
    void println(int num);
    void println();
};

extern HardwareSerial Serial;

// String class with operators
class String {
public:
    String();
    String(const char* str);
    String(int num);
    String(unsigned long num);
    void trim();
    void replace(const char* find, const char* replace);
    String substring(int start);
    String substring(int start, int end);
    int indexOf(const char* str);
    int indexOf(const char* str, int start);
    int length();
    bool startsWith(const char* str);
    bool equals(const char* str);
    void toUpperCase();
    int toInt();
    const char* c_str();
    
    // Operators
    String operator+(const String& other);
    String operator+(const char* str);
    String operator+(int num);
    bool operator==(const String& other);
    bool operator==(const char* str);
};

// Global operator overloads
String operator+(const char* str, const String& s);

#endif
