#include "mqtt_connection.h"
#include <ESP8266WiFi.h>
#include <Arduino.h>
#define TWO_SECONDS_IN_MILLIS 2000
#define UPDATE_BLUE_LED_TOPIC "samuel_cavalcanti/feeds/update-blue-led"
#define STATUS_BLUE_LED_TOPIC "samuel_cavalcanti/feeds/blue-led"

#define UPDATE_YELLOW_LED_TOPIC "samuel_cavalcanti/feeds/update-yellow-led"
#define STATUS_YELLOW_LED_TOPIC "samuel_cavalcanti/feeds/yellow-led"

MQTTConnection::MQTTConnection(const MQTTConnectionParameters *parameters, HardwareAbstractionLayer *hardware)
{
    this->client.reset(new WiFiClient());
    this->mqtt_client.reset(new PubSubClient(*this->client));
    this->parameters.reset(parameters);
    this->hardware.reset(hardware);
    this->last_millis = millis();
    this->need_update = true;
}

MQTTConnection::~MQTTConnection()
{
}

void MQTTConnection::setup()
{

    Serial.print("Server name: ");
    Serial.println(parameters->server_name);

    Serial.print("port: ");
    Serial.println(parameters->port);

    Serial.print("User name: ");
    Serial.println(parameters->username);

    Serial.print("password: ");
    Serial.println(parameters->password);

    mqtt_client->setServer(parameters->server_name, parameters->port);

    auto callback = [this](char *topic, byte *payload, unsigned int length)
    {
        String topic_str(topic);
        Serial.print("Topic ");
        Serial.println(topic_str);
        

        String yellow_topic(UPDATE_YELLOW_LED_TOPIC);
        String blue_topic(UPDATE_BLUE_LED_TOPIC);

        if (topic_str == yellow_topic)
        {
            auto status = (char)payload[0] == '1' ? LedStatus::on : LedStatus::off;

            hardware->update_led_status(D2, status);
        }

        if (topic_str == blue_topic)
        {
            auto status = (char)payload[0] == '1' ? LedStatus::on : LedStatus::off;
            hardware->update_led_status(D1, status);
        }
         need_update = true;
    };
    mqtt_client->setCallback(callback);
    
}

void MQTTConnection::loop()
{

    reconnect();
    mqtt_client->loop();

    auto now = millis();

    if (now - last_millis > TWO_SECONDS_IN_MILLIS)
    {
        last_millis = now;
        if (need_update)
        {
            need_update = false;
            send_led_status();
        }
    }
}

void MQTTConnection::reconnect()
{
    while (!mqtt_client->connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP8266Client-" + String(random(0xffff), HEX);

        // Attempt to connect
        if (mqtt_client->connect(clientId.c_str(), parameters->username, parameters->password))
        {
            Serial.println("connected");
            mqtt_client->subscribe(UPDATE_BLUE_LED_TOPIC);
            mqtt_client->subscribe(UPDATE_YELLOW_LED_TOPIC);
            Serial.print("Subscribed on: ");
            Serial.println(UPDATE_BLUE_LED_TOPIC);
            Serial.print("Subscribed on: ");
            Serial.println(UPDATE_YELLOW_LED_TOPIC);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(mqtt_client->state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void MQTTConnection::send_led_status()
{

    for (auto &led : hardware->get_leds())
    {
        auto status = led.status == LedStatus::on ? "1" : "0";
        auto topic = get_status_topic(led.color);
        mqtt_client->publish(topic, status);
    }
}

const char *MQTTConnection::get_status_topic(LedColor color)
{
    if (color == LedColor::blue)
        return STATUS_BLUE_LED_TOPIC;

    if (color == LedColor::yellow)
        return STATUS_YELLOW_LED_TOPIC;

    return nullptr;
}
