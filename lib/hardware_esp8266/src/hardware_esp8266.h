#ifndef HARDWARE_ESP8266_H
#define HARDWARE_ESP8266_H

#include <hadware_abstraction_layer.h>

class HardwareEsp8266 : public HardwareAbstractionLayer
{
private:
    float max_voltage_in_volts = 3.0;
    float max_raw_input_value = 1024;
    float volts_per_celsius = 0.01;
    void set_pins();
    const LedStatus get_led_Status(int led_pin);

public:
    HardwareEsp8266(/* args */);
    ~HardwareEsp8266();

    const float read_temperature_in_celsius() override;
    void setup() override;
    const std::vector<Led> get_leds() override;
    void update_led_status(const unsigned int led_id, LedStatus status) override;
};

#endif // HARDWARE_ESP8266_H