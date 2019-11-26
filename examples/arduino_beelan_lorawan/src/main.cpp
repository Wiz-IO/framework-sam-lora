/*
    Open: 
      framework-sam-lora/arduino/libraries/Beelan-LoRaWAN/src/Config.h and enable your band

      abp.cpp and edit your settings

    Library info:
      https://github.com/BeelanMX/Beelan-LoRaWAN
*/

#include <Arduino.h>
#include <beelan-lorawan.h>

extern const char *devAddr;
extern const char *nwkSKey;
extern const char *appSKey;

const unsigned long interval = 10000; // 10 s interval to send message
unsigned long previousMillis = 0;     // will store last time message sent
unsigned int counter = 0;             // message counter

char myStr[50];
char outStr[255];
byte recvStatus = 0;

const sRFM_pins RFM_pins = {
    .CS = RF_SEL,
    .RST = RF_RST,
    .DIO0 = RF_DIO0,
    .DIO1 = RF_DIO1,
    .DIO2 = RF_DIO2,
};

void setup()
{
  Serial.begin(115200);
  Serial.println("Arduino SAMR34 LoRaWAN  2019 Georgi Angelov");
  if (!lora.init())
  {
    Serial.println("RF not detected");
    abort();
  }
  lora.setDeviceClass(CLASS_A); // Set LoRaWAN Class change CLASS_A or CLASS_C
  lora.setDataRate(SF8BW125);   // Set Data Rate
  lora.setChannel(CH0);       
  lora.setNwkSKey(nwkSKey);     // ABP
  lora.setAppSKey(appSKey);
  lora.setDevAddr(devAddr);

  lora.setTxPower(10);

  Serial.println("Ready");
}

void loop()
{
  if (millis() - previousMillis > interval)
  {
    previousMillis = millis();
    sprintf(myStr, "Hello World %04d", counter++);
    Serial.printf("Sending: %s ", myStr);
    lora.sendUplink(myStr, strlen(myStr), 0);
    Serial.println("OK");
  }

  recvStatus = lora.readData(outStr);
  if (recvStatus)
  {
    Serial.printf("Receive: %s\n", outStr);
  }
  lora.update(); // Check Lora RX
}
