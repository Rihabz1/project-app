// Complete Arduino core stub for VS Code
#ifndef Arduino_h
#define Arduino_h

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Digital pin modes
#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

// Digital values
#define LOW 0x0
#define HIGH 0x1

// Analog reference types
#define DEFAULT 1
#define EXTERNAL 0

// Pin definitions for Arduino Uno
#define LED_BUILTIN 13

// Math constants
#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif

// Arduino basic functions
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);
int analogRead(uint8_t pin);
void analogReference(uint8_t mode);
void analogWrite(uint8_t pin, int val);

// Timing functions
unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);

// Math functions
long map(long value, long fromLow, long fromHigh, long toLow, long toHigh);
long constrain(long amt, long low, long high);
int abs(int x);
long abs(long x);
double abs(double x);
double sqrt(double x);
double pow(double x, double y);

// Random functions
void randomSeed(unsigned long seed);
long random(long max);
long random(long min, long max);

// Serial communication
extern class HardwareSerial Serial;

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus

// C++ Arduino String class
class String {
public:
    String();
    String(const char *cstr);
    String(const String &str);
    String(char c);
    String(unsigned char c, unsigned char base = 10);
    String(int value, unsigned char base = 10);
    String(unsigned int value, unsigned char base = 10);
    String(long value, unsigned char base = 10);
    String(unsigned long value, unsigned char base = 10);
    String(float value, unsigned char decimalPlaces = 2);
    String(double value, unsigned char decimalPlaces = 2);
    ~String();
    
    unsigned int length() const;
    char charAt(unsigned int index) const;
    void setCharAt(unsigned int index, char c);
    char operator[](unsigned int index) const;
    char& operator[](unsigned int index);
    void getBytes(unsigned char *buf, unsigned int bufsize, unsigned int index = 0) const;
    void toCharArray(char *buf, unsigned int bufsize, unsigned int index = 0) const;
    const char* c_str() const;
    
    // Concatenation
    String& operator+=(const String &rhs);
    String& operator+=(const char *cstr);
    String& operator+=(char c);
    String& operator+=(unsigned char num);
    String& operator+=(int num);
    String& operator+=(unsigned int num);
    String& operator+=(long num);
    String& operator+=(unsigned long num);
    String& operator+=(float num);
    String& operator+=(double num);
    
    friend String operator+(const String &lhs, const String &rhs);
    friend String operator+(const String &lhs, const char *cstr);
    friend String operator+(const char *cstr, const String &rhs);
    
    // Comparison
    int compareTo(const String &s) const;
    bool equals(const String &s) const;
    bool equals(const char *cstr) const;
    bool operator==(const String &rhs) const;
    bool operator==(const char *cstr) const;
    bool operator!=(const String &rhs) const;
    bool operator!=(const char *cstr) const;
    bool operator<(const String &rhs) const;
    bool operator>(const String &rhs) const;
    bool operator<=(const String &rhs) const;
    bool operator>=(const String &rhs) const;
    bool equalsIgnoreCase(const String &s) const;
    
    // Search
    bool startsWith(const String &prefix) const;
    bool startsWith(const String &prefix, unsigned int offset) const;
    bool endsWith(const String &suffix) const;
    int indexOf(char ch) const;
    int indexOf(char ch, unsigned int fromIndex) const;
    int indexOf(const String &str) const;
    int indexOf(const String &str, unsigned int fromIndex) const;
    int lastIndexOf(char ch) const;
    int lastIndexOf(char ch, unsigned int fromIndex) const;
    int lastIndexOf(const String &str) const;
    int lastIndexOf(const String &str, unsigned int fromIndex) const;
    
    // Modification
    String substring(unsigned int beginIndex) const;
    String substring(unsigned int beginIndex, unsigned int endIndex) const;
    void replace(char find, char replace);
    void replace(const String& find, const String& replace);
    void remove(unsigned int index);
    void remove(unsigned int index, unsigned int count);
    void toLowerCase();
    void toUpperCase();
    void trim();
    
    // Conversion
    long toInt() const;
    float toFloat() const;
    double toDouble() const;
};

// Print class
class Print {
public:
    virtual size_t write(uint8_t) = 0;
    virtual size_t write(const uint8_t *buffer, size_t size);
    
    size_t print(const String &s);
    size_t print(const char str[]);
    size_t print(char c);
    size_t print(unsigned char b, int base = 10);
    size_t print(int n, int base = 10);
    size_t print(unsigned int n, int base = 10);
    size_t print(long n, int base = 10);
    size_t print(unsigned long n, int base = 10);
    size_t print(double n, int digits = 2);
    
    size_t println(const String &s);
    size_t println(const char c[]);
    size_t println(char c);
    size_t println(unsigned char b, int base = 10);
    size_t println(int num, int base = 10);
    size_t println(unsigned int num, int base = 10);
    size_t println(long num, int base = 10);
    size_t println(unsigned long num, int base = 10);
    size_t println(double num, int digits = 2);
    size_t println(void);
};

// Stream class
class Stream : public Print {
public:
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    virtual void flush() = 0;
    
    size_t readBytes(char *buffer, size_t length);
    size_t readBytesUntil(char terminator, char *buffer, size_t length);
    String readString();
    String readStringUntil(char terminator);
    long parseInt();
    float parseFloat();
    void setTimeout(unsigned long timeout);
    bool find(const char *target);
    bool findUntil(const char *target, const char *terminator);
};

// HardwareSerial class
class HardwareSerial : public Stream {
public:
    void begin(unsigned long baud);
    void begin(unsigned long baud, uint8_t config);
    void end();
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *buffer, size_t size);
    
    using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool();
};

// Global Serial object
extern HardwareSerial Serial;

#endif // __cplusplus

#endif // Arduino_h
