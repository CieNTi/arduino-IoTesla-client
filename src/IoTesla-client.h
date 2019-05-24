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

/* Sensor data structure */
struct IoTesla_sensor_data
{
  /* System related */
  float supply_vcc;
  /* BME280 related*/
  float temperature;
  float humidity;
  float pressure;
  float altitude;
};

/* IoTesla Client Class */
class IoTeslaClient
{
  public:
    /* Constructor */
    IoTeslaClient(void);
    /* Basic methods */
    int begin(void);
    int loop(void);
    int connect(void);
    int connected(void);
    int disconnect(void);
    /* Sensor methods */
    int read_sensors(struct IoTesla_sensor_data *sdata);

  private:
    /* Tracks connection status */
    int _connected = 0;
    /* Holds sensor data */
    struct IoTesla_sensor_data sdata[8];
    /* BME280 instance to work with */
    #if !defined(IOTESLA_WITHOUT_BME280)
      BME280 BME280_obj;
    #endif
    /* MPU6050 instance to work with */
    #if !defined(IOTESLA_WITHOUT_MPU6050)
      MPU6050 MPU6050_obj;
    #endif
    /* Methods */
    void close(void);
};

#endif /* __IOTESLA_CLIENT_H */
