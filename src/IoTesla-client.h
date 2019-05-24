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
#include <FS.h>

/* BME280: Temperature, Pressure and Humidity sensor */
#if !defined(IOTESLA_WITHOUT_BME280)
  #include <Wire.h>
  #include <SparkFunBME280.h>
  #define IOTESLA_BME280_ADDRESS 0x76
#endif

/* MPU6050: Accelerometer and Gyroscope sensor */
#if !defined(IOTESLA_WITHOUT_MPU6050)
  #include <Wire.h>
  #include <I2Cdev.h>
  #include <MPU6050.h>
  #define IOTESLA_MPU6050_ADDRESS 0x68
#endif

/* Common to both sensors */
#if (!defined(IOTESLA_WITHOUT_BME280) || !defined(IOTESLA_WITHOUT_MPU6050))
  #define IOTESLA_USE_WIRE_H
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
    uint8_t loop(void);
    uint8_t connect(void);
    uint8_t connected(void);
    uint8_t disconnect(void);
  private:
    /* Variables */
    uint8_t _connected = 0;
    #if !defined(IOTESLA_WITHOUT_BME280)
      BME280 BME280_obj;
    #endif
    #if !defined(IOTESLA_WITHOUT_MPU6050)
      MPU6050 MPU6050_obj;
    #endif
    /* Constructor */
    /* Methods */
    void close(void);
};

#endif /* __IOTESLA_CLIENT_H */
