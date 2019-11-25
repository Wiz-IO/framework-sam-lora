/*
    Not tested in LoRaWAN network ( I not have coordinator )

    Library info:
      https://github.com/BeelanMX/Beelan-LoRaWAN
*/


#include <Arduino.h>
#include <beelan-lorawan.h>

//ABP Credentials
const char *devAddr = "00000000";
const char *nwkSKey = "00000000000000000000000000000000";
const char *appSKey = "00000000000000000000000000000000";

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
    .DIO5 = RF_DIO5,
};

void setup()
{
  Serial.begin(115200);
  Serial.println("SAMR34 Arduino 2019 Georgi Angelov");
  delay(1000);
  if (!lora.init())
  {
    Serial.println("RF not detected");
    abort();
  }
  Serial.println("LoRa init");

  // Set LoRaWAN Class change CLASS_A or CLASS_C
  lora.setDeviceClass(CLASS_A);

  // Set Data Rate
  lora.setDataRate(SF8BW125);

  // set channel to random
  lora.setChannel(MULTI);

  // Put ABP Key and DevAddress here
  lora.setNwkSKey(nwkSKey);
  lora.setAppSKey(appSKey);
  lora.setDevAddr(devAddr);
}

void loop()
{
  if (millis() - previousMillis > interval)
  {
    previousMillis = millis();
    sprintf(myStr, "Counter-%d", counter);
    Serial.print("Sending: ");
    Serial.println(myStr);
    lora.sendUplink(myStr, strlen(myStr), 0);
    counter++;
  }

  recvStatus = lora.readData(outStr);
  if (recvStatus)
    Serial.println(outStr);
  lora.update(); // Check Lora RX
}