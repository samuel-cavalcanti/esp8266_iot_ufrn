#ifndef LED_H

#define LED_H

enum class LedColor
{
    white,
    red,
    blue,
    green,
    yellow
};
enum class LedStatus
{
    on,
    off
};

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
    // ~Led();
};


// Led::~Led()
// {
// }

#endif //LED_H