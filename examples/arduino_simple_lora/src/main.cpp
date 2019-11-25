/*
    LoRa: Send and Receive between two boards
    More info: 
        https://github.com/sandeepmistry/arduino-LoRa/blob/master/API.md
*/

#include <Arduino.h>
#include <RF.h>
#include <LoRa.h>
#include <Timer.h>

Timer TG, TY;
void onTG(void) { digitalWrite(LED_G, LED_OFF); }
void onTY(void) { digitalWrite(LED_Y, LED_OFF); }

void onReceive(int packetSize) // ! from isr
{
    Serial.print("Received '");
    for (int i = 0; i < packetSize; i++)
        Serial.print((char)LoRa.read());
    Serial.print("' RSSI ");
    Serial.println(LoRa.packetRssi());
    digitalWrite(LED_G, LED_ON); 
    TG.start(500, onTG, TIMER_ONE_SHOT);
}

void LoRa_init()
{
    LoRa.enableTCXO();
    LoRa.setTxPower(5);
    if (!LoRa.begin(915E6)) // frequency
    {
        Serial.println("\n[ERROR] LoRa init failed");
        abort();
    }
    LoRa.onReceive(onReceive);
    LoRa.receive();
    Serial.printf("LoRa Init\n");
}

void LoRa_send()
{
    LoRa.beginPacket();
    LoRa.print("Hello World ");
    LoRa.print(seconds());
    LoRa.endPacket();
    Serial.println("Packet sent");
    LoRa.receive();
}

void setup()
{
    Serial.begin(115200);
    Serial.println("\nMicrochip SAMR34 Xplained Pro 2019 Georgi Angelov");
    pinMode(LED_Y, OUTPUT);
    digitalWrite(LED_Y, LED_OFF); 
    pinMode(LED_G, OUTPUT);
    digitalWrite(LED_G, LED_OFF);
    LoRa_init();
}

void loop()
{
    static uint32_t period = 0;
    if (seconds() - period > 10) 
    {
        LoRa_send();
        period = seconds();
        digitalWrite(LED_Y, LED_ON); 
        TY.start(500, onTY, TIMER_ONE_SHOT);
    }
}