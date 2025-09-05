#ifndef SoftwareSerial_h
#define SoftwareSerial_h

// Basic type definitions for VS Code IntelliSense
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

// Forward declaration
class String;

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
    int indexOf(const String &str) const;
    int lastIndexOf(char ch) const;
    int lastIndexOf(const String &str) const;
    
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
;

// Forward declaration of Stream class  
class Stream {
public:
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    virtual size_t write(uint8_t) = 0;
    virtual void flush() {}
    
    // String reading methods (return types simplified for VS Code)
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

// Print class for Serial inheritance
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

// HardwareSerial class for Serial object
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

/******************************************************************************
* Definitions
******************************************************************************/

class SoftwareSerial : public Stream
{
private:
  // per object data
  uint8_t _receivePin;
  uint8_t _receiveBitMask;
  uint8_t _transmitBitMask;
  volatile uint8_t *_receivePortRegister;
  uint8_t _transmitPin;
  
  // Expressed as 4-cycle delays (must never be 0!)
  uint16_t _rx_delay_centering;
  uint16_t _rx_delay_intrabit;
  uint16_t _rx_delay_stopbit;
  uint16_t _tx_delay;

  uint16_t _buffer_overflow:1;
  uint16_t _inverse_logic:1;

  // static data
  static char _receive_buffer[64];
  static volatile uint8_t _receive_buffer_tail;
  static volatile uint8_t _receive_buffer_head;
  static SoftwareSerial *active_object;

  // private methods
  void recv();
  uint8_t rx_pin_read();
  void tx_pin_write(uint8_t pin_state);
  void setTX(uint8_t transmitPin);
  void setRX(uint8_t receivePin);

  // Return num - sub, or 1 if the result would be < 1
  static uint16_t subtract_cap(uint16_t num, uint16_t sub);

  // private static method for timing
  static inline void tunedDelay(uint16_t delay);

public:
  // public methods
  SoftwareSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic = false);
  ~SoftwareSerial();
  void begin(long speed);
  bool listen();
  void end();
  bool isListening() { return this == active_object; }
  bool stopListening();
  bool overflow() { bool ret = _buffer_overflow; if (ret) _buffer_overflow = false; return ret; }
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

  // public only for easy access by interrupt handlers
  static inline void handle_interrupt();
};

// Global Serial object declaration
extern HardwareSerial Serial;

#endif
