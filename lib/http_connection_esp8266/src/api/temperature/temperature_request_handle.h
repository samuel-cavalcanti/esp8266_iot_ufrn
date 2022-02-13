#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "hadware_abstraction_layer.h"
#include "../../response.hpp"
#include "../../request.hpp"

class TemperatureRequestHandle
{
private:
    HardwareAbstractionLayer *hardware;

public:
    TemperatureRequestHandle(HardwareAbstractionLayer *hardware);
    const Response get_temperature(const Request &request);
};

#endif