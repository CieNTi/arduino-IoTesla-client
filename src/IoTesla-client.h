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

/* Amount of data to keep saved on RAM */
#define IOTESLA_SDATA_RAM_COUNT 1

/* Amount of data to save on a single file */
#define IOTESLA_DATA_PER_FILE 5000

/* Max size for a command */
#define IOTESLA_COMMAND_SIZE 16

/* Sensor data structure */
struct IoTesla_sensor_data
{
  /* System related */
  unsigned long timestamp;
  unsigned int data_id;
  float supply_vcc;
  /* BME280 related*/
  float temperature;
  float humidity;
  float pressure;
  float altitude;
  /* MPU6050 related */
  int16_t accelerometer_x;
  int16_t accelerometer_y;
  int16_t accelerometer_z;
  int16_t gyroscope_x;
  int16_t gyroscope_y;
  int16_t gyroscope_z;
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
    /* Data methods */
    int read_sensors(struct IoTesla_sensor_data *sdata);
    int save_data(struct IoTesla_sensor_data *sdata);
    /* Control via console methods */
    int read_console(void);

  private:
    /* Tracks connection status */
    int _connected = 0;
    /* Holds File for saving data */
    File sensor_data_file;
    /* To check if more data can be saved */
    struct FSInfo spiffs_info;
    size_t free_bytes;
    /* Holds sensor data (RAM) */
    struct IoTesla_sensor_data sdata[IOTESLA_SDATA_RAM_COUNT];
    /* Available commands */
    char *commands[4] =
    {
      #define IOTESLA_CMD_PRINT 0
      "print",
      #define IOTESLA_CMD_DELETE 1
      "delete",
      #define IOTESLA_CMD_STATUS 2
      "status",
      NULL
    };
    /* Holds latest command, valid or not */
    char last_command[IOTESLA_COMMAND_SIZE] = { 0x00 };
    /* Chars read so far inside last_command */
    int last_command_size = 0;
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
