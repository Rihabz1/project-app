// Complete SoftwareSerial library stub for VS Code
#ifndef SoftwareSerial_h
#define SoftwareSerial_h

#include <stdint.h>
#include <stddef.h>

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

class Print {
public:
    virtual size_t write(uint8_t) = 0;
    virtual size_t write(const uint8_t *buffer, size_t size);
    
    size_t print(const char str[]);
    size_t print(char c);
    size_t print(unsigned char b, int base = DEC);
    size_t print(int n, int base = DEC);
    size_t print(unsigned int n, int base = DEC);
    size_t print(long n, int base = DEC);
    size_t print(unsigned long n, int base = DEC);
    size_t print(double n, int digits = 2);
    
    size_t println(const char c[]);
    size_t println(char c);
    size_t println(unsigned char b, int base = DEC);
    size_t println(int num, int base = DEC);
    size_t println(unsigned int num, int base = DEC);
    size_t println(long num, int base = DEC);
    size_t println(unsigned long num, int base = DEC);
    size_t println(double num, int digits = 2);
    size_t println(void);
};

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

class SoftwareSerial : public Stream {
private:
    uint8_t _receivePin;
    uint8_t _transmitPin;
    
public:
    SoftwareSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic = false);
    ~SoftwareSerial();
    
    void begin(long speed);
    void end();
    bool isListening();
    bool overflow();
    int peek();
    
    // Stream virtual methods
    virtual size_t write(uint8_t byte);
    virtual int read();
    virtual int available();
    virtual void flush();
    
    // Enable/disable listening
    bool listen();
    
    // Compatibility
    using Print::write; // pull in write(str) and write(buf, size) from Print
};

// String class forward declaration stub
class String {
public:
    String() {}
    String(const char* str) {}
    String(char c) {}
    String(int n, int base = DEC) {}
    String(unsigned int n, int base = DEC) {}
    String(long n, int base = DEC) {}
    String(unsigned long n, int base = DEC) {}
    String(double n, int digits = 2) {}
};

#endif
