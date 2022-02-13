
#ifndef MQTT_CONNECTION_H
#define MQTT_CONNECTION_H
#include <PubSubClient.h>
#include <memory>
#include "mqtt_connection_parameters.hpp"
#include "hadware_abstraction_layer.h"
class MQTTConnection
{
private:
    std::unique_ptr<Client> client;
    std::unique_ptr<PubSubClient> mqtt_client;
    std::unique_ptr<const MQTTConnectionParameters> parameters;
    std::unique_ptr<HardwareAbstractionLayer> hardware;
    bool need_update;
    unsigned long last_millis;
    void reconnect();
    void send_led_status();
    const char *get_status_topic(LedColor color);

public:
    MQTTConnection(const MQTTConnectionParameters *parameters, HardwareAbstractionLayer *hardware);
    ~MQTTConnection();

    void setup();
    void loop();
};

#endif // MQTT_CONNECTION_H