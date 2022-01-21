#include <Arduino.h>
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "ESP8266mDNS.h"

#include "secrets.h"

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

                  WEB_SERVER.send(201, "text/plain", request_body);

                  auto result = parse_request_body(request_body);

                  digitalWrite(result.led_pin, result.digital_option);
                });

  WEB_SERVER.begin();

  Serial.println("Http server started on  http://" + WiFi.localIP().toString() + ":80");
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
        <p>change led status</p>
        <div style="display:flex; width: 100%; height: 100%;">

            <form id="led_form" action="/led" method="post" target="dummyframe" style="width: 100%;">
                <div class="form-floating" style=" width: 7%;margin-bottom: 1%;">

                    <select class="form-select" aria-label="Floating label select example" id="led" name="led">
                        <option value="Yellow">Yellow</option>
                        <option value="blue">Blue</option>
                    </select>

                    <label for="led">Choose a led:</label>
                </div>



                <input class="btn btn-success" style="margin-right: 1.2%;" id="submit" type="submit" name="action"
                    value="ON">
                <input class="btn btn-danger" id="submit" type="submit" name="action" value="OFF">
            </form>
        </div>
    </div>


    <iframe name="dummyframe" id="dummyframe" style="display: none;"></iframe>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js"
        integrity="sha384-ka7Sk0Gln4gmtz2MlQnikT1wXgYsOg+OMhuP+IlRH9sENBO0LRn5q+8nbTov4+1p"
        crossorigin="anonymous"></script>
</body>

</html>
)";
}