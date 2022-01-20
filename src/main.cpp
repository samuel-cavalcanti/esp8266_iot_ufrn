#include <Arduino.h>
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include <ESP8266mDNS.h>

#define BLUE_LED D1
#define YELLOW_LED D2

struct ParserResult
{
  uint8 led_pin;
  int digital_option;
};

ESP8266WebServer WEB_SERVER(80);
void connect_to_my_wifi();
void set_pins();
void set_routers();
const char *get_home_page();
ParserResult parse_request_body(String &body);


void setup()
{
  Serial.begin(115200);

  set_pins();

  connect_to_my_wifi();
  set_routers();
}

void set_pins()
{
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
}
void connect_to_my_wifi()
{
  auto ssid = "SSID";
  auto password = "PASSWORD";

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Trying to connect ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(700);
    Serial.print(".");
  }

  Serial.print("connected to  ");
  Serial.println(ssid);

  return;
}

void set_routers()
{
  if (MDNS.begin("esp8266"))
    Serial.println("MDNS responder started on http://esp8266.local/");
  else
    Serial.println("Unable to start MDNS");

  WEB_SERVER.on("/", HTTP_GET, []()
                { WEB_SERVER.send(200, "text/html", get_home_page()); });

  WEB_SERVER.on("/led", HTTP_POST, []()
                {
                  auto request_body = WEB_SERVER.arg("plain");

                  if (request_body.isEmpty())
                  {
                    WEB_SERVER.send(200, "text/plain", "You must send somthing!");
                    return;
                  }

                  WEB_SERVER.send(200, "text/html", get_home_page());

                  auto result = parse_request_body(request_body);

                  digitalWrite(result.led_pin, result.digital_option);
                });

  WEB_SERVER.begin();

  Serial.println("Http server started on " + WiFi.localIP().toString() + ":80");
}

void loop()
{

  WEB_SERVER.handleClient();
  MDNS.update();
}

ParserResult parse_request_body(String &body)
{
  auto delimiter = body.indexOf("&");
  auto action_token = body.substring(delimiter + 1);
  auto led_token = body.substring(0, delimiter);

  delimiter = action_token.indexOf("=");
  auto action_value = action_token.substring(delimiter + 1);

  auto digital_option = action_value == "ON" ? HIGH : LOW;

  delimiter = led_token.indexOf("=");
  auto led_color = led_token.substring(delimiter + 1);

  auto led_pin = led_color == "blue" ? BLUE_LED : YELLOW_LED;

  Serial.println("Body: " + body);
  Serial.println("led: " + led_color + " action: " + action_value);

  return ParserResult{led_pin, digital_option};
}

const char *get_home_page()
{
  return R"(<!DOCTYPE html>
<html>
<body>

<h2>ESP 8266</h2>

<form action="/led" method="post">
  <label for="led">Choose a led:</label>
<select id="led" name="led">
  <option value="Yellow">Yellow</option>
  <option value="blue">Blue</option>
</select>

  <input id="submit" type="submit" name="action" value="ON">
  <input id="submit" type="submit" name="action" value="OFF">
</form> 

<p>change led status</p>

</body>
</html>
)";
}