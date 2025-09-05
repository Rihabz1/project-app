// Stream class stub for VS Code
#ifndef Stream_h
#define Stream_h

#include "Print.h"

class Stream : public Print {
public:
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    virtual void flush() = 0;
    
    String readStringUntil(char terminator);
    String readString();
    
    size_t readBytes(char *buffer, size_t length);
    size_t readBytesUntil(char terminator, char *buffer, size_t length);
    
    long parseInt();
    float parseFloat();
    
    size_t find(const char *target);
    size_t find(const char *target, size_t length);
    size_t findUntil(const char *target, const char *terminator);
    
    virtual size_t write(uint8_t) = 0;
    using Print::write;
};

#endif
