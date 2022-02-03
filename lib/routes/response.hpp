#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include "Arduino.h"

#define JSON_CONTENT_TYPE "application/json"
#define HTML_CONTENT_TYPE "text/html"
#define TEXT_PLAIN_TYPE "text/plain"

class Response
{
public:
    Response(const unsigned int code, const char *content_type, const String &body)
    {
        this->body = body;
        this->code = code;
        this->content_type = content_type;
    }
    ~Response() {}
    String body;
    unsigned int code;
    const char *content_type;
};

#endif //RESPONSE_HPP