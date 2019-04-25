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
  * uint8_t IoTeslaClient::begin()
  */
uint8_t IoTeslaClient::begin()
{
  return 0;
}

/** ~~~
  * uint8_t IoTeslaClient::loop()
  */
uint8_t IoTeslaClient::loop()
{
  /* returns if not connected */
  if (!this->connected())
  {
    return 1;
  }
  return 0;
}

/** ~~~
  * uint8_t IoTeslaClient::connect()
  */
uint8_t IoTeslaClient::connect()
{
  /* Close connection if connected */
  if (this->connected())
  {
    this->close();
  }

  /* Set flag */
  this->_connected = 1;

  return 1;
}

/** ~~~
  * uint8_t IoTeslaClient::connected()
  */
uint8_t IoTeslaClient::connected()
{
  return this->_connected;
}

/** ~~~
  * uint8_t IoTeslaClient::disconnect()
  */
uint8_t IoTeslaClient::disconnect()
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
  * void IoTeslaClient::close()
  */
void IoTeslaClient::close()
{
  /* Set flag */
  this->_connected = 0;
}
