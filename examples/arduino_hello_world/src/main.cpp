#include <Arduino.h>

void setup()
{
    Serial.begin(115200);
    delay(1);
    Serial.println("\nMicrochip ATSAMR34 Xplained Pro 2019 Georgi Angelov");
    pinMode(LED, OUTPUT);
    Serial.println("SETUP");
}

void loop()
{
    digitalWrite(LED, 1);
    delay(100);
    digitalWrite(LED, 0);
    delay(100);

    static int cnt = 0;
    if (cnt % 20 == 0)
        Serial.printf("MILLIS: %u\n", millis());
    if (cnt % 30 == 0)
        Serial.printf("SECONDS: %u\n", seconds());
    cnt++;
}