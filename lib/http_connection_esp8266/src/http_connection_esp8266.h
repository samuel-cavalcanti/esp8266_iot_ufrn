#include "api/leds/leds_request_handle.h"
#include "api/temperature/temperature_request_handle.h"
#include "index/index_request_handle.hpp"
#include "ESP8266WebServer.h"
#include "routes.h"
#include <hadware_abstraction_layer.h>

#ifndef HTTP_SERVER_ESP8266_H
#define HTTP_SERVER_ESP8266_H

/*
    HttpConnectionEsp8266 é um simples http server que rescebe requests /index e /api

*/

class HttpConnectionEsp8266
{
private:
    /* data */
    std::unique_ptr<LedsRequestHandle> leds_request_handle;
    std::unique_ptr<TemperatureRequestHandle> temperature_request_handle;
    std::unique_ptr<IndexRequestHandle> index_request_handle;
    std::unique_ptr<ESP8266WebServer> server;

    void set_router(const char *end_point, HTTPMethod method, const std::function<const Response(const Request)> request_handle);

public:
    HttpConnectionEsp8266(HardwareAbstractionLayer *hardware);

    void loop();
    void setup();

    ~HttpConnectionEsp8266();
};

#endif // HTTP_SERVER_ESP8266_H