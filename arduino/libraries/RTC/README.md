#include <Arduino.h>
#include <RTC.h>
RTCClass rtc(1);

void setup()
{
  Serial.begin(115200);
  Serial.println("\nMicrochip SAMR34 Xplained Pro 2020 Georgi Angelov");
  int cause = get_reset_cause();
  Serial.printf("Reset %d\n", cause);
  Serial.printf("WDT %08X\n", WDT->CTRLA.reg);
  Serial.printf("RTC %08X\n", RTC->MODE0.CTRLA.reg);
  pinMode(LED_G, OUTPUT);
  digitalWrite(LED_G, LED_OFF);
}

void loop()
{
  static int led = 0;
  digitalWrite(LED_G, ++led & 1);
  delay(100);

  if (seconds() > 5)
  {
    pinMode(LED_G, INPUT);
    Serial.printf("Go Sleep\n");
    rtc.sleep_wakeup(10);
  }
}
