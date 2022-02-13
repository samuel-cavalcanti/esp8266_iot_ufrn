#include <hadware_abstraction_layer.h>
#include <vector>

class MockHardware : public HardwareAbstractionLayer
{
private:
public:
    const float read_temperature_in_celsius() override
    {
        return 42.0;
    };
    void setup() override{

    };
    const std::vector<Led> get_leds() override
    {

        return std::vector<Led>{
            Led(0, LedColor::blue, LedStatus::off),
            Led(1, LedColor::yellow, LedStatus::off),
        };
    }
    void update_led_status(const unsigned int led_id, LedStatus status) override{

    };
};
