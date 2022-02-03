#include <Arduino.h>
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "ESP8266mDNS.h"
#include "ArduinoJson.h"

#include "secrets.h"

#define BLUE_LED D1
#define YELLOW_LED D2
#define LM35 A0
#define JSON_CONTENT_TYPE "application/json"

ESP8266WebServer WEB_SERVER(80);
void connect_to_my_wifi();
void set_pins();
void set_routers();
const char *get_home_page();
float read_temperature_in_celsius();
void setup()
{
  Serial.begin(115200);
  delay(500);

  set_pins();

  connect_to_my_wifi();
  set_routers();
}

void set_pins()
{
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(LM35, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
}
void connect_to_my_wifi()
{

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  Serial.print("Trying to connect ");
  Serial.println(SSID);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(700);
    Serial.print(".");
  }

  Serial.print("connected to  ");
  Serial.println(SSID);

  return;
}

void set_routers()
{
  if (MDNS.begin("esp8266"))
    Serial.println("MDNS responder started on http://esp8266.local/");
  else
    Serial.println("Unable to start MDNS");

  auto ledsRouter = "/api/leds";
  auto temperatureRouter = "/api/temperature";

  WEB_SERVER.on("/", HTTP_GET, []()
                { WEB_SERVER.send(200, "text/html", get_home_page()); });

  WEB_SERVER.on(temperatureRouter, HTTP_GET, []()
                {
                  StaticJsonDocument<16> doc;

                  doc["temperature_in_celsius"] = read_temperature_in_celsius();

                  String stringJson;
                  serializeJson(doc, stringJson);
                  WEB_SERVER.send(200, JSON_CONTENT_TYPE, stringJson);
                });

  WEB_SERVER.on(ledsRouter, HTTP_GET, []()
                {
                  StaticJsonDocument<192> doc;

                  JsonObject doc_0 = doc.createNestedObject();
                  doc_0["id"] = BLUE_LED;
                  doc_0["color"] = "blue";
                  doc_0["status"] = (bool)digitalRead(BLUE_LED);

                  JsonObject doc_1 = doc.createNestedObject();
                  doc_1["id"] = YELLOW_LED;
                  doc_1["color"] = "yellow";
                  doc_1["status"] = (bool)digitalRead(YELLOW_LED);

                  String stringJson;
                  serializeJson(doc, stringJson);

                  WEB_SERVER.send(200, JSON_CONTENT_TYPE, stringJson);
                });

  WEB_SERVER.on(ledsRouter, HTTP_POST, []()
                {
                  auto request_body = WEB_SERVER.arg("plain");

                  if (request_body.isEmpty())
                  {
                    WEB_SERVER.send(400, "text/plain", "You must send somthing in JSON Format");
                    return;
                  }

                  StaticJsonDocument<96> doc;
                  DeserializationError error = deserializeJson(doc, request_body);
                  if (error)
                  {
                    Serial.print(F("deserializeJson() failed: "));
                    Serial.println(error.f_str());
                    WEB_SERVER.send(500, "text/plain", F("deserializeJson() failed: "));
                    return;
                  }
                  int id = doc["id"];
                  bool status = doc["status"];

                  digitalWrite(id, status);

                  WEB_SERVER.send(201, "text/plain", request_body);
                });

  WEB_SERVER.begin();

  Serial.println("Http server started on  http://" + WiFi.localIP().toString() + ":80");
}

void loop()
{

  WEB_SERVER.handleClient();
  MDNS.update();
}

float read_temperature_in_celsius()
{

  int raw_input = analogRead(LM35);

  float max_voltage_in_voltz = 3.0;
  float max_raw_input_value = 1024;
  float voltz_per_celsius = 0.01;
  /*
     Segundo o datasheet do LM35 possui 0.01V/°C
     ou seja para para 10mV - 1°C

     sabendo que  3.0V - 1024
     então:        yV  -  x
     
     y = 3.0V*x/1023

     sabendo que  0.01V - 1°C
                   yV   - t°C
  
     t = y/0.01 

     t = (3.0V*x/1023)/0.01
 
     */

  return (max_voltage_in_voltz * ((float)raw_input) / max_raw_input_value) / voltz_per_celsius;
}

const char *get_home_page()
{
  return R"(<!DOCTYPE html>
<html>

<head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <!-- Bootstrap CSS -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css" rel="stylesheet"
        integrity="sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3" crossorigin="anonymous">

    <title>ESP 8266</title>
</head>

<body>

    <div style="width: 100%; height: 100%; margin-left: 1%;">
        <h1>ESP 8266</h1>
        
    <ul>
        <li>/api/leds</li>
        <li>/api/temperature</li>
    </ul>

</body>

</html>
)";
}