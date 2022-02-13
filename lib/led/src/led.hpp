#ifndef LED_H

#define LED_H

#include "led_color.h"
#include "led_status.h"

class Led
{

public:
    int id;
    LedColor color;
    LedStatus status;

    Led(int id, LedColor color, LedStatus status)
    {
        this->id = id;
        this->color = color;
        this->status = status;
    };
};

#endif // LED_H