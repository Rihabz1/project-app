#ifndef ARDUINO_STUB_H
#define ARDUINO_STUB_H

// Basic type definitions
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed long int32_t;
typedef unsigned int size_t;

// Arduino pin definitions
#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19

// Arduino constants
#define HIGH 0x1
#define LOW  0x0
#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

// Arduino function declarations
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);
int analogRead(uint8_t pin);
void analogWrite(uint8_t pin, int val);
unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);

// String class for Arduino
class String {
private:
    char *buffer;
    unsigned int capacity;
    unsigned int len;

public:
    // Constructors
    String(const char *cstr = "");
    String(const String &str);
    String(char c);
    String(int value, int base = 10);
    String(unsigned int value, int base = 10);
    String(long value, int base = 10);
    String(unsigned long value, int base = 10);
    String(float value, int decimalPlaces = 2);
    String(double value, int decimalPlaces = 2);
    ~String();

    // Assignment operators
    String& operator=(const String &rhs);
    String& operator=(const char *cstr);

    // Concatenation
    String& operator+=(const String &rhs);
    String& operator+=(const char *cstr);
    String& operator+=(char c);
    String& operator+=(int num);
    String& operator+=(unsigned int num);
    String& operator+=(long num);
    String& operator+=(unsigned long num);
    String& operator+=(float num);
    String& operator+=(double num);

    // Comparison
    bool operator==(const String &rhs) const;
    bool operator==(const char *cstr) const;
    bool operator!=(const String &rhs) const;
    bool operator!=(const char *cstr) const;
    bool equals(const String &s) const;
    bool equals(const char *cstr) const;

    // Access
    char charAt(unsigned int index) const;
    char operator[](unsigned int index) const;
    char& operator[](unsigned int index);
    
    // Information
    unsigned int length() const { return len; }
    const char* c_str() const { return buffer; }
    
    // Search
    int indexOf(char ch) const;
    int indexOf(const char *str) const;
    int indexOf(char ch, unsigned int fromIndex) const;
    int indexOf(const char *str, unsigned int fromIndex) const;
    int indexOf(const String &str, unsigned int fromIndex) const;
    int lastIndexOf(char ch) const;
    int lastIndexOf(const char *str) const;
    
    // String methods your code uses
    bool startsWith(const String &prefix) const;
    bool startsWith(const char *prefix) const;
    bool endsWith(const String &suffix) const;
    bool endsWith(const char *suffix) const;
    
    // Modification
    String substring(unsigned int beginIndex) const;
    String substring(unsigned int beginIndex, unsigned int endIndex) const;
    void replace(char find, char replace);
    void replace(const String& find, const String& replace);
    void toLowerCase();
    void toUpperCase();
    void trim();
    
    // Conversion
    long toInt() const;
    float toFloat() const;
    double toDouble() const;
};

// Stream class
class Stream {
public:
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    virtual size_t write(uint8_t) = 0;
    virtual void flush() {}
    
    // String reading methods
    String readString();
    String readStringUntil(char terminator);
    
    // Other Stream methods
    void setTimeout(unsigned long timeout) {}
    bool find(const char *target) { return false; }
    bool findUntil(const char *target, const char *terminator) { return false; }
    long parseInt() { return 0; }
    float parseFloat() { return 0.0; }
    size_t readBytes(char *buffer, size_t length) { return 0; }
    size_t readBytesUntil(char terminator, char *buffer, size_t length) { return 0; }
};

// Print class
class Print {
public:
    virtual size_t write(uint8_t) = 0;
    virtual size_t write(const char *str) { return 0; }
    
    // print methods
    size_t print(const char str[]) { return 0; }
    size_t print(char c) { return write(c); }
    size_t print(unsigned char c, int base = 10) { return 0; }
    size_t print(int n, int base = 10) { return 0; }
    size_t print(unsigned int n, int base = 10) { return 0; }
    size_t print(long n, int base = 10) { return 0; }
    size_t print(unsigned long n, int base = 10) { return 0; }
    size_t print(double n, int digits = 2) { return 0; }
    size_t print(const String &s) { return 0; }
    
    // println methods
    size_t println(const char str[]) { return 0; }
    size_t println(char c) { return 0; }
    size_t println(unsigned char c, int base = 10) { return 0; }
    size_t println(int n, int base = 10) { return 0; }
    size_t println(unsigned int n, int base = 10) { return 0; }
    size_t println(long n, int base = 10) { return 0; }
    size_t println(unsigned long n, int base = 10) { return 0; }
    size_t println(double n, int digits = 2) { return 0; }
    size_t println(const String &s) { return 0; }
    size_t println(void) { return 0; }
};

// HardwareSerial class
class HardwareSerial : public Print, public Stream {
public:
    void begin(unsigned long baud) {}
    void begin(unsigned long baud, uint8_t config) {}
    void end() {}
    virtual int available() { return 0; }
    virtual int peek() { return -1; }
    virtual int read() { return -1; }
    virtual void flush() {}
    virtual size_t write(uint8_t c) { return 1; }
    operator bool() { return true; }
};

// SoftwareSerial class
class SoftwareSerial : public Stream {
private:
    uint8_t _receivePin;
    uint8_t _transmitPin;

public:
    SoftwareSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic = false);
    ~SoftwareSerial();
    void begin(long speed);
    bool listen();
    void end();
    bool isListening() { return true; }
    bool stopListening();
    bool overflow() { return false; }
    int peek();

    virtual size_t write(uint8_t byte);
    virtual int read();
    virtual int available();
    virtual void flush();
    operator bool() { return true; }
    
    // Print methods for SoftwareSerial
    size_t print(const char str[]) { return 0; }
    size_t print(char c) { return write(c); }
    size_t print(unsigned char c, int base = 10) { return 0; }
    size_t print(int n, int base = 10) { return 0; }
    size_t print(unsigned int n, int base = 10) { return 0; }
    size_t print(long n, int base = 10) { return 0; }
    size_t print(unsigned long n, int base = 10) { return 0; }
    size_t print(double n, int digits = 2) { return 0; }
    size_t print(const String &s) { return 0; }
    
    size_t println(const char str[]) { return 0; }
    size_t println(char c) { return 0; }
    size_t println(unsigned char c, int base = 10) { return 0; }
    size_t println(int n, int base = 10) { return 0; }
    size_t println(unsigned int n, int base = 10) { return 0; }
    size_t println(long n, int base = 10) { return 0; }
    size_t println(unsigned long n, int base = 10) { return 0; }
    size_t println(double n, int digits = 2) { return 0; }
    size_t println(const String &s) { return 0; }
    size_t println(void) { return 0; }
};

// Global Serial object
extern HardwareSerial Serial;

#endif
