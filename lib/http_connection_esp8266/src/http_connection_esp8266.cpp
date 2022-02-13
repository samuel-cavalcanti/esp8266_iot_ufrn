#include "http_connection_esp8266.h"

HttpConnectionEsp8266::HttpConnectionEsp8266(HardwareAbstractionLayer *hardware)
{
  this->leds_request_handle.reset(new LedsRequestHandle(hardware));
  this->temperature_request_handle.reset(new TemperatureRequestHandle(hardware));
  this->index_request_handle.reset(new IndexRequestHandle());

  this->server.reset(new ESP8266WebServer(80));
}

void HttpConnectionEsp8266::set_router(const char *end_point, HTTPMethod method, const std::function<const Response(const Request)> request_handle)
{

  Serial.print("setting ");
  Serial.println(end_point);

  auto callback = [request_handle, this]()
  {
    
    auto request = Request(server->arg("plain"));

    Response response = request_handle(request);

    String body = response.body;

    server->send(response.code, response.content_type, body);
  };

  server->on(end_point, method, callback);
}

void HttpConnectionEsp8266::setup()
{

  set_router(INDEX_END_POINT, HTTP_GET, [this](auto request)
             { return this->index_request_handle->get_index(request); });

  set_router(TEMPERATURE_END_POINT, HTTP_GET, [this](auto request)
             { return temperature_request_handle->get_temperature(request); });

  set_router(LEDS_END_POINT, HTTP_GET, [this](auto request)
             { return leds_request_handle->get_leds(request); });

  set_router(LEDS_END_POINT, HTTP_POST, [this](auto request)
             { return leds_request_handle->post_led(request); });

  server->begin();
}

void HttpConnectionEsp8266::loop()
{
  server->handleClient();
}