/*
    Libraries:
        Adafruit GFX 
        Adafruit SSD1306 128x64

    OLED: 
        https://store.comet.bg/Catalogue/Product/51127/

*/

#include <Arduino.h>
#include <Device.h>

String SN;
#include <Timer.h>
Timer T;

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display;
#define SLAVE_ADDRESS 0x78 >> 1
#define imageWidth 78
#define imageHeight 64

void oled_setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, SLAVE_ADDRESS);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("  Arduino");
  display.setTextColor(BLACK, WHITE);
  display.println(" ATSAMR34 ");
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.println();
  display.println("WizIO 2019 Microchip");
  display.display();
}

void onTimer(void)
{
  static int led = 0;
  digitalWrite(LED, ++led & 1);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("ATSAMR34 Arduino 2019 Georgi Angelov");
  Serial.printf("RESET FROM: %d\n", get_reset_cause());
  dev.getSerialNumber(SN);
  Serial.print("SERIAL NUMBER: ");
  Serial.println(SN);
  oled_setup();
  pinMode(LED, OUTPUT);
  T.start(50, onTimer);
}

void loop()
{

}
