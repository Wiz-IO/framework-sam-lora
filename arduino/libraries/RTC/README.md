
Example

```c
#include <Arduino.h>
#include <RTC.h>
RTCClass rtc(1); // enable RTC, auto-start

void setup()
{
  Serial.begin(115200);
  Serial.println("\nMicrochip SAMR34 Xplained Pro 2020 Georgi Angelov");
  int cause = get_reset_cause();
  Serial.printf("Reset %d\n", cause); // 128 = from RTC
  pinMode(LED_G, OUTPUT);
  digitalWrite(LED_G, LED_OFF);
}

void loop()
{
  static int led = 0;
  digitalWrite(LED_G, ++led & 1);
  delay(100);

  if (seconds() > 5) // blink 5 seconds and goto sleep-mode
  {
    pinMode(LED_G, INPUT);
    Serial.println("Go Sleep");
    rtc.sleep_wakeup(10); // sleep ... wakeup after 10 seconds
  }
}
```
