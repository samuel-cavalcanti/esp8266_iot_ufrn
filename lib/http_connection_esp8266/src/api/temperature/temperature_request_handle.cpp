#include "temperature_request_handle.h"
#include "ArduinoJson.h"
TemperatureRequestHandle::TemperatureRequestHandle(HardwareAbstractionLayer *hardware)
{
    this->hardware = hardware;
}

const Response TemperatureRequestHandle::get_temperature(const Request &request)
{
    StaticJsonDocument<16> doc;

    doc["temperature_in_celsius"] = hardware->read_temperature_in_celsius();

    String stringJson;
    serializeJson(doc, stringJson);

    return Response(200, JSON_CONTENT_TYPE, stringJson);
}
