#include <IoTesla-client.h>

/* This allows 'ESP.getVcc()' to be used */
ADC_MODE(ADC_VCC);

#define SENSOR_PERIOD_MS 10
#define SENSOR_DATA_FILE "/sensor_data.bin"

/** ~~~
  * IoTeslaClient::IoTeslaClient(void)
  */
IoTeslaClient::IoTeslaClient(void)
{
  _connected = 0;
}

/** ~~~
  * int IoTeslaClient::begin(void)
  */
int IoTeslaClient::begin(void)
{
  Serial.printf("Starting in 10 .. ");
  delay(2000);
  Serial.printf("8 .. ");
  delay(2000);
  Serial.printf("6 .. ");
  delay(2000);
  Serial.printf("4 .. ");
  delay(2000);
  Serial.printf("2 ..\n");
  delay(2000);

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
  
  /* DO NOT INCLUDE THIS ON FINAL VERSION ! */
  SPIFFS.remove(SENSOR_DATA_FILE); /* DO NOT INCLUDE THIS ON FINAL VERSION ! */
  /* DO NOT INCLUDE THIS ON FINAL VERSION ! */

  /* Clean RAM data */
  for (int i = 0; i < IOTESLA_SDATA_RAM_COUNT; i++)
  {
    memset(&sdata[i], 0x00, sizeof(struct IoTesla_sensor_data));
  }

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
    BME280_obj.setFilter(4);
    /* Time between readings.       | 0 to 7 valid.      | See table 27. */
    BME280_obj.setStandbyTime(0);
    /* 0 disables temp sensing.     | 0 to 16 are valid. | See table 24. */
    BME280_obj.setTempOverSample(5);
    /* 0 disables pressure sensing. | 0 to 16 are valid. | See table 23. */
    BME280_obj.setPressureOverSample(5);
    /* 0 disables humidity sensing. | 0 to 16 are valid. | See table 19. */
    BME280_obj.setHumidityOverSample(5);
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

    /* By default, the library uses the following settings:
     * - setFullScaleGyroRange(MPU6050_GYRO_FS_250)
     * - setFullScaleAccelRange(MPU6050_ACCEL_FS_2)
     */

    /* Configure Accelerometer range, choose from:
     * - MPU6050_ACCEL_FS_2 ...: +/-2g
     * - MPU6050_ACCEL_FS_4 ...: +/-4g
     * - MPU6050_ACCEL_FS_8 ...: +/-8g
     * - MPU6050_ACCEL_FS_16 ..: +/-16g
     */
    MPU6050_obj.setFullScaleAccelRange(MPU6050_ACCEL_FS_16);

    /* Configure Gyroscope range, choose from:
     * - MPU6050_GYRO_FS_250 ..: +/-250 deg/sec
     * - MPU6050_GYRO_FS_500 ..: +/-500 deg/sec
     * - MPU6050_GYRO_FS_1000 .: +/-1000 deg/sec
     * - MPU6050_GYRO_FS_2000 .: +/-2000 deg/sec
     */
    MPU6050_obj.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);

    Serial.printf("- MPU6050 ... OK!\n");
  #endif

  return 0;
}

/** ~~~
 * int IoTeslaClient::read_sensors(struct IoTesla_sensor_data *sdata)
 */
int IoTeslaClient::read_sensors(struct IoTesla_sensor_data *sdata)
{
  /*
   * System Section
   */
  sdata->data_id++;
  sdata->timestamp = millis();
  sdata->supply_vcc  = 0.001 * ESP.getVcc();

  /* 
   * BME280 Section
   * Is configured to an standby of 0.5ms only, with lot of HW FIR filters
   * so it is supposed to give a coherent stable measurement ... let check it
   */
  sdata->temperature      = BME280_obj.readTempC();
  sdata->humidity         = BME280_obj.readFloatHumidity();
  sdata->pressure         = BME280_obj.readFloatPressure();
  sdata->altitude         = BME280_obj.readFloatAltitudeMeters();

  /* 
   * MPU6050 Section
   */
  MPU6050_obj.getMotion6(&sdata->accelerometer_x,
                         &sdata->accelerometer_y,
                         &sdata->accelerometer_z,
                         &sdata->gyroscope_x,
                         &sdata->gyroscope_y,
                         &sdata->gyroscope_z
  );

  /* Go without errors */
  return 0;
}

/** ~~~
 * int IoTeslaClient::save_data(struct IoTesla_sensor_data *sdata)
 */
int IoTeslaClient::save_data(struct IoTesla_sensor_data *sdata)
{
  if (!sensor_data_file)
  {
    /* File still not opened? Try to open */

    /* Mode 'a+' create if not exists:
     *  - Read from the beginning of the file
     *  - Append new data at the end
     *  * Useful for buffers ;)
     */
    sensor_data_file = SPIFFS.open(SENSOR_DATA_FILE, "a+");
    if (!sensor_data_file)
    {
      /* Oh man, this is serious */
      Serial.printf("Cannot open '" SENSOR_DATA_FILE "'. Locking now'\n");

      /* Stop here (WDT will reset at some point) */
      while(1);
    }
    Serial.printf("Opened '" SENSOR_DATA_FILE "'\n");
  }

  /* Opened, now put some (at the end of the file) */
  SPIFFS.info(spiffs_info);
  free_bytes = spiffs_info.totalBytes - spiffs_info.usedBytes;
  if (free_bytes > sizeof(struct IoTesla_sensor_data))
  {
    /* Yet free space */
    Serial.printf("Writing data %016u ts: %lu ms, %u bytes (free: %u bytes)\n",
                  sdata->data_id,
                  sdata->timestamp,
                  sizeof(struct IoTesla_sensor_data),
                  free_bytes
                  );
    sensor_data_file.write((const uint8_t *)sdata,
                           sizeof(struct IoTesla_sensor_data));
    delay(1);
  }
  else
  {
    /* Memory is full */
    Serial.printf("Memory is full\n");
    if (sensor_data_file)
    {
      sensor_data_file.close();
      Serial.printf("Closed '" SENSOR_DATA_FILE "'\n");
    }
    delay(1000);
  }

  return 0;
}

/** ~~~
  * int IoTeslaClient::loop(void)
  */
int IoTeslaClient::loop(void)
{
  /* Used to count the time */
  static unsigned long millis_now = 0;
  static unsigned long millis_last = 0;

  /* returns if not connected */
  if (!this->connected())
  {
    return 1;
  }

  /* Delayed execution without locking the CPU */
  millis_now = millis();
  if (millis_now - millis_last > SENSOR_PERIOD_MS)
  {
    /* Refresh timer */
    millis_last = millis_now;

    /* Read data */
    read_sensors(&sdata[0]);

    /* Clear screen */
    Serial.printf("\033[2J\033[H");

    /* Do! */
    Serial.printf("%2.2f [V] %2.2f [C] %2.2f [%%] %2.2f [Pa] %2.2f [m]\n"
                  "%i [gx] %i [gy] %i [gz] %i [dx] %i [dy] %i [dz]\n",
                  sdata[0].supply_vcc,
                  sdata[0].temperature,
                  sdata[0].humidity,
                  sdata[0].pressure,
                  sdata[0].altitude,
                  sdata[0].accelerometer_x,
                  sdata[0].accelerometer_y,
                  sdata[0].accelerometer_z,
                  sdata[0].gyroscope_x,
                  sdata[0].gyroscope_y,
                  sdata[0].gyroscope_z
                  );

    /* Save data */
    save_data(&sdata[0]);
  }

  /* All ok */
  return 0;
}

/** ~~~
  * int IoTeslaClient::connect(void)
  */
int IoTeslaClient::connect(void)
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
  * int IoTeslaClient::connected(void)
  */
int IoTeslaClient::connected(void)
{
  return this->_connected;
}

/** ~~~
  * int IoTeslaClient::disconnect(void)
  */
int IoTeslaClient::disconnect(void)
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
