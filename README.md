# IoTesla Client library for Arduino platform

This library is created for the C and Arduino Workshop for Euroavia Sevilla, to ease the development process, integration and usage of sensors, MQTT broker and frontend, using ESP8266 WiFi.

This library will need the following libraries to be installed prior to use it:

- D1 Mini board
    + Board support: [ESP8266 Arduino](https://github.com/esp8266/Arduino)
- Only one of the following MQTT Client libraries:
    + [PubSubClient library](https://github.com/knolleary/pubsubclient) (QoS=0, No SSL, lot of information found on google). Will be used by default
    + [MQTT library](https://github.com/256dpi/arduino-mqtt) (Any QoS, SSL, less information). Define `IOTESLA_WITHOUT_PUBSUB` before `IoTesla-client.h` include to disable PubSub, which will enable this one as replacement.

IoTesla nodes has both BME280 and MPU6050 sensor boards attached to them, so the following libraries are encouraged to be installed in order to use all node and library capabilities.

Both are expected by default, so a compilation will throw error as-is. In order to disable one or both, the following flags have to be defined before `IoTesla-client.h` include.

```C
/* Removes BME280 support and requirements */
#define IOTESLA_WITHOUT_BME280

/* Removes MPU6050 support and requirements */
#define IOTESLA_WITHOUT_MPU6050
```

- BME280 board
    + Library: [SparkFun BME280](https://github.com/sparkfun/SparkFun_BME280_Arduino_Library)
- MPU6050 (GY-521) board
    + Library: [MPU6050](https://github.com/electroniccats/mpu6050)

# Frontend and control panel for IoTesla nodes

In order to easily interact with IoTesla nodes, a controller is provided in form of a Javascript class and static site.

Web technologies and Javascript are choosen because the massive compatibility they has, so it can be accessed using PC, Smartphones ...

The static site is proudly generated using `mkDocs`, and can be both built or simply served inside the development server it provides (see: `mkdocs serve`). This method integrates very well with git providers as it uses just valid markdown files and related media.

To install mkDocs and related requirements:

```shell
$ pip3 install mkdocs
$ pip3 install mkdocs-awesome-pages-plugin
$ pip3 install mkdocs-material
```

Then, execute the following from this repository folder to start serving the site at [http://127.0.0.1:8000](http://127.0.0.1:8000):

```shell
$ mkdocs serve
```

# IoTesla client usage and remote capabilities

IoTesla client will correctly configure the node and sensors once `.begin()` method is called, and will expect to be called inside the main loop.

> **NOTE:** The main loop cannot contain time-wasting delays, as it can lead to failure and MQTT data loss

Example loop with delayed execution while everything is working fine:

```C
/* Used to count the time */
unsigned long millis_now = 0;
unsigned long millis_last = 0;
#define A_DELAY_MS 2500

/*
 * Recurrent task, called forever
 */
void loop()
{
  /* Update time */
  millis_now = millis();

  /* If more than A_DELAY_MS ms, do some task */
  if (millis_now - millis_last > A_DELAY_MS)
  {
    /* Rearm timer */
    millis_last = millis_now;

    // SOME DELAYED TASK HERE
  }
}
```

## MQTT broker access and topics convention

## Node Information API

## Sensor API

## Last words

Enjoy!

```
            ======
          /========\
         /..........\
        /.............\
       /...............\
      /.................\
     /.................\\\_____
    /..................\\\\\\\\\________
   |...............................\\\\\\______
   |.......................................======\____
    \..............................................===\_______
     \........................................................\_
       \........................................................\
         \.......................................................\
            \.................................._====_._............\
               \........................... ../        *-.._........\
                 \..........................|                \_____/*
                   \........................|                 \----\
                     \......................|                  |....\
                       \=\..................|                  |.....\
                        \==\.................\                 /......|
                         \==\\.................\              /.......|
                          \=\.\\.................=.._________/......./
                            \=..\.................................../
                             \=\..\................................/
                               \=...\............................./
                                 \=\..\\\________________________.
                                  \..\\__.............\.
                                   \..\\..---__..........\.
                                    \..\\.......\............\.
                                      \..\\.......\\..............\.
                                        \..\\.......\  \\._...........\.
                                         \...\\.......\     --\._........\
                                          \...| \.......\         \_._.....\
                                           \..|   \.......\            \....|
                                            \/       \......\             \/
                                                        \.....\
                                                          \.../
                                                            \/        
```

 :Credits: https://pastebin.com/8xNsnAhQ
