/* Include required headers and/or libraries */
#include <IoTesla-client.h>

/* This block allows to be configured outside this code, but will use
 * the following information as default in case is not */
#if !defined(STASSID_0)
  #define STASSID_0 "IoTesla"
  #define STAPSK_0  "euroavia2019"
#endif

/* Instantiate a IoTeslaClient object called IoTesla */
IoTeslaClient IoTesla;

/*
 * Single-pass function to configure the app
 */
void setup()
{
  /* Start serial for output */
  Serial.begin(115200);

  /* Welcome message! Useful as a control point */
  Serial.printf("Ahoy! ESP8266 here!\n"
                "---\n"
                );

  /* Check communication before continue */
  if (IoTesla.begin())
  {
    Serial.printf("IoTesla Client failed to initialize. Stopping here\n");
    
    /* Stop here (WDT will reset at some point) */
    while(1) {};
  }
}

/*
 * Recurrent task, called forever
 */
void loop()
{
  /* Test IoTesla Client */
  if (IoTesla.loop())
  {
    Serial.printf("IoTesla Client loop not connected. Connecting ...\n");
    if (IoTesla.connect())
    {
      Serial.printf("Unable to connect. Retrying in 1 second ...\n");
      delay(1000);
    }
    else
    {
      Serial.printf("Connected!\n");
    }
  }

  /* Ensure not to flood with a huge amount of fast data */
  delay(10);
}
