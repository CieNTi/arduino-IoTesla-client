/**
 * @file
 * @brief      IoTesla client library, to easily use iot.tesla.studio services
 */
 
/* Define to prevent recursive inclusion */
#ifndef __IOTESLA_CLIENT_H
#define __IOTESLA_CLIENT_H

/* Required libraries */
#include "Arduino.h"
#include <Wire.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

/* If USE_PUBSUB is defined, 'PubSub' mqtt client library will be used */
#define USE_PUBSUB
#if defined(USE_PUBSUB)
  /* https://github.com/knolleary/pubsubclient */
  #include <PubSubClient.h>
#else
  /* https://github.com/256dpi/arduino-mqtt */
  #include <MQTT.h>
#endif

/* IoTesla Client Class */
class IoTeslaClient
{
  public:
    /* Variables */
    /* Constructor */
    IoTeslaClient(void);

    /* Methods */
    /* Main setup and loop */
    uint8_t begin(void);
    uint8_t loop();
    uint8_t connect();
    uint8_t connected();
    uint8_t disconnect();
  private:
    /* Variables */
    uint8_t _connected = 0;
    /* Constructor */
    /* Methods */
    void close();
};

#endif /* __IOTESLA_CLIENT_H */
