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
