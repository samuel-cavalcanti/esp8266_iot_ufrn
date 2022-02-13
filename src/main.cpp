#include <Arduino.h>
#include <hardware_esp8266.h>
#include <mqtt_connection.h>
#include "ESP8266WiFi.h"

#include "secrets.h"
#include "adafruit_io_secrets.h"

HardwareAbstractionLayer *hardware;
MQTTConnection *connection;

void connect_to_my_wifi();
// void multicast_dns_setup();
void serial_connection_setup();
void setup()
{
  hardware = new HardwareEsp8266();
  auto mqtt_params = new MQTTConnectionParameters{"io.adafruit.com", 1883, ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY};
  connection = new MQTTConnection(mqtt_params, hardware);

  serial_connection_setup();
  connect_to_my_wifi();
  // multicast_dns_setup();
  hardware->setup();
  connection->setup();

  Serial.println("local ip  " + WiFi.localIP().toString());
}

void serial_connection_setup()
{
  Serial.begin(115200);
  delay(500);
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
// void multicast_dns_setup()
// {
//   if (MDNS.begin("esp8266"))
//     Serial.println("MDNS responder started on http://esp8266.local/");
//   else
//     Serial.println("Unable to start MDNS");
// }

void loop()
{
  connection->loop();
  // MDNS.update();
}
