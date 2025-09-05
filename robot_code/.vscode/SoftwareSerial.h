// Complete SoftwareSerial library stub for VS Code
#ifndef SoftwareSerial_h
#define SoftwareSerial_h

#include "Print.h"
#include "Stream.h"

class SoftwareSerial : public Stream {
private:
    uint8_t _receivePin;
    uint8_t _transmitPin;
    
public:
    SoftwareSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic = false);
    ~SoftwareSerial();
    
    void begin(long speed);
    void end();
    bool listen();
    bool isListening();
    bool stopListening();
    bool overflow();
    
    virtual int peek();
    virtual size_t write(uint8_t byte);
    virtual int read();
    virtual int available();
    virtual void flush();
    
    // Print class methods
    size_t print(const String &s);
    size_t print(const char str[]);
    size_t print(char c);
    size_t print(unsigned char b, int base = DEC);
    size_t print(int n, int base = DEC);
    size_t print(unsigned int n, int base = DEC);
    size_t print(long n, int base = DEC);
    size_t print(unsigned long n, int base = DEC);
    size_t print(double n, int digits = 2);
    
    size_t println(const String &s);
    size_t println(const char c[]);
    size_t println(char c);
    size_t println(unsigned char b, int base = DEC);
    size_t println(int num, int base = DEC);
    size_t println(unsigned int num, int base = DEC);
    size_t println(long num, int base = DEC);
    size_t println(unsigned long num, int base = DEC);
    size_t println(double num, int digits = 2);
    size_t println(void);
    
    using Print::write;
};

#endif
