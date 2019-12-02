#include <Arduino.h>
#include <RF.h>
#include <RTC.h>
RTCClass rtc(1);

void setup()
{
  Serial.begin(115200);
  Serial.println("\nMicrochip ATSAMR34 Xplained Pro 2019 Georgi Angelov");
  RF.end(); // put RF to sleep
  Serial.printf("Reset %d\n", get_reset_cause());
  pinMode(LED_G, OUTPUT);
  digitalWrite(LED_G, LED_OFF);
}

void loop()
{
  // blink led 4 seconds, then sleep for 20 seconds
  digitalToggle(LED_G, 100);
  if (seconds() > 4)
  {
    digitalWrite(LED_G, LED_OFF);
    rtc.sleep_wakeup(20); // sleep ... wakeup after 20 seconds
  }
}