
#ifndef HARDWARE_ABSTRACTION_LAYER_H
#define HARDWARE_ABSTRACTION_LAYER_H

#include "led.hpp"
#include "led_status.h"
#include <vector>

class HardwareAbstractionLayer
{

public:
    virtual const float read_temperature_in_celsius() = 0;
    virtual void setup() = 0;
    virtual const std::vector<Led> get_leds() = 0;
    virtual void update_led_status(const unsigned int led_id, LedStatus status) = 0;
};

#endif // HARDWARE_ABSTRACTION_LAYER_H