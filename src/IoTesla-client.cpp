#include <IoTesla-client.h>

/* This allows 'ESP.getVcc()' to be used */
ADC_MODE(ADC_VCC);

/** ~~~
  * IoTeslaClient::IoTeslaClient(void)
  */
IoTeslaClient::IoTeslaClient(void)
{
  _connected = 0;
}

/** ~~~
  * uint8_t IoTeslaClient::begin(void)
  */
uint8_t IoTeslaClient::begin(void)
{
  Serial.printf("Configuring IoTesla Client\n");

  /* Initialize the file system */
  Serial.printf("- Initializing SPIFFS\n");
  if (SPIFFS.begin() == false)
  {
    Serial.printf("! SPIFFS cannot be initialized\n");

    /* Stop here (WDT will reset at some point) */
    while(1) {};
  }
  uint32_t SPIFFS_real_size = ESP.getFlashChipRealSize();
  uint32_t SPIFFS_IDE_size = ESP.getFlashChipSize();

  Serial.printf("- Flash Chip ID ....: %08X\n", ESP.getFlashChipId());
  Serial.printf("- Flash ide size ...: %u bytes\n", SPIFFS_IDE_size);
  Serial.printf("- Flash real size ..: %u bytes\n", SPIFFS_real_size);

  if (SPIFFS_IDE_size != SPIFFS_real_size)
  {
    Serial.println("! Wrong SPIFFS configuration. Please check IDE\n");

    /* Stop here (WDT will reset at some point) */
    while(1) {};
  }
  Serial.printf("- SPIFFS ... OK!\n");

  /* Join I2C bus and set it to 400 kHz */
  #if defined(IOTESLA_USE_WIRE_H)
    Serial.printf("- Setting up I2C bus\n");
    #if defined(IOTESLA_STD_I2C_PINS)
      Wire.begin();
    #else
      Wire.begin(0, 2);
    #endif
    Wire.setClock(400000);
  #endif

  /* Configure BME280 sensor */
  #if !defined(IOTESLA_WITHOUT_BME280)
    Serial.printf("- Setting up BME280 sensor\n");
    BME280_obj.setI2CAddress(IOTESLA_BME280_ADDRESS);

    /* Check communication before continue */
    if (BME280_obj.beginI2C(Wire) == false)
    {
      Serial.printf("! BME280 did not respond. Please check wiring\n");

      /* Stop here (WDT will reset at some point) */
      while(1);
    }
    Serial.printf("- BME280 ... OK!\n");

    /* Filter coefficient.          | 0 to 4 is valid.   | See 3.4.4     */
    BME280_obj.setFilter(2);
    /* Time between readings.       | 0 to 7 valid.      | See table 27. */
    BME280_obj.setStandbyTime(1);
    /* 0 disables temp sensing.     | 0 to 16 are valid. | See table 24. */
    BME280_obj.setTempOverSample(8);
    /* 0 disables pressure sensing. | 0 to 16 are valid. | See table 23. */
    BME280_obj.setPressureOverSample(8);
    /* 0 disables humidity sensing. | 0 to 16 are valid. | See table 19. */
    BME280_obj.setHumidityOverSample(8);
    /* MODE_SLEEP, MODE_FORCED, MODE_NORMAL is valid.    | See 3.3       */
    BME280_obj.setMode(MODE_NORMAL);
  #endif

  /* Configure MPU6050 sensor */
  #if !defined(IOTESLA_WITHOUT_MPU6050)
    Serial.printf("- Setting up MPU6050 sensor\n");
    /* Initialize the sensor */
    MPU6050_obj = MPU6050(IOTESLA_MPU6050_ADDRESS);
    MPU6050_obj.initialize();

    /* Check communication before continue */
    if (MPU6050_obj.testConnection() == false)
    {
      Serial.printf("! MPU6050 did not respond. Please check wiring\n");

      /* Stop here (WDT will reset at some point) */
      while(1);
    }
    Serial.printf("- MPU6050 ... OK!\n");
  #endif

  return 0;
}

/** ~~~
  * uint8_t IoTeslaClient::loop(void)
  */
uint8_t IoTeslaClient::loop(void)
{
  /* returns if not connected */
  if (!this->connected())
  {
    return 1;
  }
  return 0;
}

/** ~~~
  * uint8_t IoTeslaClient::connect(void)
  */
uint8_t IoTeslaClient::connect(void)
{
  /* Close connection if connected */
  if (this->connected())
  {
    this->close();
  }

  /* Set flag */
  this->_connected = 1;

  return 0;
}

/** ~~~
  * uint8_t IoTeslaClient::connected(void)
  */
uint8_t IoTeslaClient::connected(void)
{
  return this->_connected;
}

/** ~~~
  * uint8_t IoTeslaClient::disconnect(void)
  */
uint8_t IoTeslaClient::disconnect(void)
{
  /* returns if not connected */
  if (!this->connected())
  {
    return 1;
  }

  /* Close */
  this->close();

  return 0;
}

/** ~~~
  * void IoTeslaClient::close(void)
  */
void IoTeslaClient::close(void)
{
  /* Set flag */
  this->_connected = 0;
}
