/**
 * @file
 * @brief IoTesla client library, to easily use iot.tesla.studio services
 * 
 * Configuration variables:
 * - IOTESLA_WITHOUT_PUBSUB:  Define to stop using PubSub, starting with MQTT
 * - IOTESLA_WITHOUT_BME280:  Define to disable BME280 requirements and usage
 * - IOTESLA_WITHOUT_MPU6050: Define to disable MPU6050 requirements and usage
 */
 
/* Define to prevent recursive inclusion */
#ifndef __IOTESLA_CLIENT_H
#define __IOTESLA_CLIENT_H

/* Required libraries */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

/* BME280: Temperature, Pressure and Humidity sensor */
#if !defined(IOTESLA_WITHOUT_BME280)
  #include <Wire.h>
  #include <SparkFunBME280.h>
#endif

/* MPU6050: Accelerometer and Gyroscope sensor */
#if !defined(IOTESLA_WITHOUT_MPU6050)
  #include <Wire.h>
  #include <I2Cdev.h>
  #include <MPU6050.h>
#endif

/* 'PubSub' vs 'MQTT' mqtt client library selection */
#if !defined(IOTESLA_WITHOUT_PUBSUB)
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
