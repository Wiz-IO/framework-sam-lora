#include <Arduino.h>
#include <RF.h>
#include <RTC.h>
RTCClass rtc;

uint32_t psm_counter; // test value for rtc backup

void setup()
{
  Serial.begin(115200);
  Serial.println("\nMicrochip ATSAMR34 Xplained Pro 2019 Georgi Angelov");
  RF.end(); // put RF to sleep

  int c = get_reset_cause();
  Serial.printf("Reset %d\n", c);
  if (RESET_CAUSE_BACKUP == c)
  {
    rtc.get_backup(1 /*max 4 values*/, &psm_counter); // restore from rtc ram, after rtc.begin() value is not valid
    Serial.printf("PSM counter = %d\n", psm_counter);
  }
  else
  {
    Serial.printf("PSM counter Reset\n");
    psm_counter = 0;
  }

  rtc.begin();
  pinMode(LED_G, OUTPUT);
  digitalWrite(LED_G, LED_OFF);
}

void loop()
{
  digitalToggle(LED_G, 100); // blink led 4 seconds, then sleep for 20 seconds
  if (seconds() > 4)
  {
    digitalWrite(LED_G, LED_OFF);                      // led off
    rtc.set_backup(1 /*max 4 values*/, ++psm_counter); // save to rtc ram
    rtc.sleep_wakeup(20);                              // sleep ... wakeup after 20 seconds
  }
}
