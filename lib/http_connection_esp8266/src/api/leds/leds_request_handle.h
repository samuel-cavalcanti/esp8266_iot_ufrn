
#ifndef LEDS_REQUEST_HANDLE_H
#define LEDS_REQUEST_HANDLE_H

#include "hadware_abstraction_layer.h"
#include "../../request.hpp"
#include "../../response.hpp"


class LedsRequestHandle
{
private:
    HardwareAbstractionLayer *hardware;
    const char *get_color_name(LedColor color);


public:
    LedsRequestHandle(HardwareAbstractionLayer *hardware);

    const Response get_leds(const Request &request);
  

    const Response post_led(const Request &request);
};

#endif //LEDS_REQUEST_HANDLE_H