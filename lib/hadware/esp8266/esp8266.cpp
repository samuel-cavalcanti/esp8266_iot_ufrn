
#include "esp8266.h"
#include "Arduino.h"

#define BLUE_LED D1
#define YELLOW_LED D2
#define LM35 A0

Esp8266::Esp8266(/* args */)
{
}

Esp8266::~Esp8266()
{
}

const float Esp8266::read_temperature_in_celsius()
{

    auto raw_input = analogRead(LM35);

    /*
     Segundo o datasheet do LM35 possui 0.01V/°C
     ou seja para para 10mV - 1°C

     sabendo que  3.0V - 1024
     então:        yV  -  x
     
     y = 3.0V*x/1023

     sabendo que  0.01V - 1°C
                   yV   - t°C
  
     t = y/0.01 

     t = (3.0V*x/1023)/0.01
 
     */

    return (max_voltage_in_volts * ((float)raw_input) / max_raw_input_value) / volts_per_celsius;
}

void Esp8266::setup()
{
    set_pins();
}

void Esp8266::set_pins()
{
    pinMode(BLUE_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(LM35, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
}

const std::vector<Led> Esp8266::get_leds()
{
    return std::vector<Led>{
        Led(BLUE_LED, LedColor::blue, get_led_Status(BLUE_LED)),
        Led(YELLOW_LED, LedColor::yellow, get_led_Status(YELLOW_LED)),
    };
}

const LedStatus Esp8266::get_led_Status(int led_pin)
{
    return digitalRead(led_pin) ? LedStatus::on : LedStatus::off;
}

void Esp8266::update_led_status(unsigned int led_id, LedStatus status)
{
    auto value = status == LedStatus::on ? HIGH : LOW;
    digitalWrite(led_id, value);
}