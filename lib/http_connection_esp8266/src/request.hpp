#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Arduino.h"

class Request
{
public:
    Request(const String &body)
    {
        this->body = body;
    }
    ~Request() {}
    String body;
};

#endif //REQUEST_HPP