# IoTesla Client library for Arduino platform

This library was made for the C and Arduino Workshop for Euroavia Sevilla, to ease the development process, integration and usage of sensors, mqtt broker and frontend, using ESP8266 WiFi.

This library will need the following libraries to be installed prior to use it:

- D1 Mini board
    + Board support: [ESP8266 Arduino](https://github.com/esp8266/Arduino)
- MPU6050 (GY-521) board
    + Library: [MPU6050](https://github.com/electroniccats/mpu6050)
- BME280 board
    + Library: [SparkFun BME280](https://github.com/sparkfun/SparkFun_BME280_Arduino_Library)
- MQTT Client:
    + Will use: [PubSubClient library](https://github.com/knolleary/pubsubclient) (QoS=0, No SSL, lot of information found on google)
    + Also tested with: [MQTT library](https://github.com/256dpi/arduino-mqtt) (Any QoS, SSL, less information)

Please install them all before continue.
