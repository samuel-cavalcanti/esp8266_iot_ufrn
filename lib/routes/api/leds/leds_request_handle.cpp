#include "leds_request_handle.h"
#include "ArduinoJson.h"
LedsRequestHandle::LedsRequestHandle(HardwareAbstractionLayer *hardware)
{
    this->hardware = hardware;
}

const Response LedsRequestHandle::get_leds(const Request &request)
{
    StaticJsonDocument<192> doc;

    for (auto &led : hardware->get_leds())
    {
        JsonObject json_led = doc.createNestedObject();
        json_led["id"] = led.id;
        json_led["color"] = get_color_name(led.color);
        json_led["status"] = led.status == LedStatus::on ? true : false;
    }

    String stringJson;
    serializeJson(doc, stringJson);

    return Response(200, JSON_CONTENT_TYPE, stringJson);
}

const char *LedsRequestHandle::get_color_name(LedColor color)
{
    switch (color)
    {
    case LedColor::blue:
        return "blue";

    case LedColor::green:
        return "green";

    case LedColor::red:
        return "red";

    case LedColor::white:
        return "white";

    case LedColor::yellow:
        return "yellow";
    }

    return nullptr;
}


const Response LedsRequestHandle::post_led(const Request &request)
{
    if (request.body.isEmpty())
    {

        return Response(400, TEXT_PLAIN_TYPE, "You must send somthing in JSON Format");
    }

    StaticJsonDocument<96> doc;
    DeserializationError error = deserializeJson(doc, request.body);
    if (error)
    {
        auto error_message = "deserializeJson() failed: ";
        Serial.print(error_message);
        Serial.println(error.f_str());

        return Response(500, TEXT_PLAIN_TYPE, error_message);
    }
    auto led_id = doc["id"];
    auto status = doc["status"] ? LedStatus::on : LedStatus::off;

    hardware->update_led_status(led_id, status);

    return Response(201, JSON_CONTENT_TYPE, request.body);
}