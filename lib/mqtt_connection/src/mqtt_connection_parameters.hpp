
#ifndef MQTT_CONNECTION_PARAMETERS
#define MQTT_CONNECTION_PARAMETERS
#include <stdint.h>

struct MQTTConnectionParameters
{
    const char *server_name;
    const uint16_t port;
    const char *username;
    const char *password;
};
#endif