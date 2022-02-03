#include <Arduino.h>
#include <hardware.h>
#include <routes.h>

#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "ESP8266mDNS.h"

#include "secrets.h"

ESP8266WebServer WEB_SERVER(80);

HardwareAbstractionLayer *hardware = new Esp8266();
auto leds = LedsRequestHandle(hardware);
auto temperature = TemperatureRequestHandle(hardware);

void connect_to_my_wifi();

void set_routers();
const char *home_page_html();
void setup()
{
  Serial.begin(115200);
  delay(500);

  hardware->setup();

  connect_to_my_wifi();

  if (MDNS.begin("esp8266"))
    Serial.println("MDNS responder started on http://esp8266.local/");
  else
    Serial.println("Unable to start MDNS");

  set_routers();

  WEB_SERVER.begin();

  Serial.println("Http server started on  http://" + WiFi.localIP().toString() + ":80");
}

void connect_to_my_wifi()
{

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  Serial.println("\n\n\n\n");
  Serial.print("Trying to connect ");
  Serial.println(SSID);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(700);
    Serial.print(".");
  }

  Serial.print("connected to  ");
  Serial.println(SSID);
}

void loop()
{

  WEB_SERVER.handleClient();
  MDNS.update();
}

void set_router(const char *end_point, HTTPMethod method, std::function<const Response(const Request)> request_handle)
{
  WEB_SERVER.on(end_point, method, [request_handle]()
                {
                  auto request = Request(WEB_SERVER.arg("plain"));

                  Response response = request_handle(request);

                  String body = response.body;

                  WEB_SERVER.send(response.code, response.content_type, body); });
}

void set_routers()
{

  set_router(INDEX_END_POINT, HTTP_GET, get_index);

  set_router(TEMPERATURE_END_POINT, HTTP_GET, [](auto request)
             { return temperature.get_temperature(request); });

  set_router(LEDS_END_POINT, HTTP_GET, [](auto request)
             { return leds.get_leds(request); });

  set_router(LEDS_END_POINT, HTTP_POST, [](auto request)
             { return leds.post_led(request); });
}