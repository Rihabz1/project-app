#ifndef Arduino_h
#define Arduino_h

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

// Arduino pin definitions for Uno/Nano/Pro Mini
#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19
#define A6 20
#define A7 21

// Digital pins
#define PIN_A0   (14)
#define PIN_A1   (15)
#define PIN_A2   (16)
#define PIN_A3   (17)
#define PIN_A4   (18)
#define PIN_A5   (19)
#define PIN_A6   (20)
#define PIN_A7   (21)

// Arduino constants
#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

// Arduino data types
typedef unsigned int word;
typedef bool boolean;
typedef uint8_t byte;

// Function declarations
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);
int analogRead(uint8_t pin);
void analogReference(uint8_t mode);
void analogWrite(uint8_t pin, int val);

unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout);

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);

void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), int mode);
void detachInterrupt(uint8_t interruptNum);

void setup(void);
void loop(void);

// Math functions
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

long random(long min, long max);
long random(long max);
void randomSeed(unsigned long seed);

long map(long value, long fromLow, long fromHigh, long toLow, long toHigh);

// Characters
int isAlphaNumeric(int thisChar);
int isAlpha(int thisChar);
int isAscii(int thisChar);
int isWhitespace(int thisChar);
int isControl(int thisChar);
int isDigit(int thisChar);
int isGraph(int thisChar);
int isLowerCase(int thisChar);
int isPrintable(int thisChar);
int isPunct(int thisChar);
int isSpace(int thisChar);
int isUpperCase(int thisChar);
int isHexadecimalDigit(int thisChar);

int toAscii(int thisChar);
int toLowerCase(int thisChar);
int toUpperCase(int thisChar);

// String class forward declaration
class String;

#ifdef __cplusplus
} // extern "C"

// C++ only includes
#include "WString.h"
#include "HardwareSerial.h"
#include "Stream.h"
#include "Print.h"

// USB CDC class
class Serial_ : public HardwareSerial {
public:
    Serial_() {}
    void begin(unsigned long baud) { (void)baud; }
    void begin(unsigned long baud, uint8_t config) { (void)baud; (void)config; }
    void end() {}
    virtual int available() { return 0; }
    virtual int peek() { return -1; }
    virtual int read() { return -1; }
    virtual void flush() {}
    virtual size_t write(uint8_t c) { (void)c; return 1; }
    using Print::write;
    operator bool() { return true; }
};

extern Serial_ Serial;

#endif

#endif
