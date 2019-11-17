#include <Arduino.h>
#include <AESClass.h>
AESClass aes;

char key[16] = {0x03, 0x13, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73, 0x83, 0x93, 0xA3, 0xB3, 0xC3, 0xD3, 0xE3, 0xF3};
char message[] = "0123456789ABCDEF";

// http://aes.online-domain-tools.com/
// key:    03132333435363738393A3B3C3D3E3F3
// answer: CC13DAC0BD8A34D570F90A5F62CF4C65

void setup()
{
  Serial.begin(115200);
  Serial.println("\nMicrochip ATSAMR34 Xplained Pro 2019 Georgi Angelov");
  Serial.printf("MESSAGE    %s\n", message);

  uint8_t *p;
  aes.begin();

  p = aes.encode(message, key);
  Serial.print("AES ENCODE ");
  for (int i = 0; i < 16; i++)
    Serial.printf("%02X", (int)p[i]);
  Serial.println();

  p = aes.decode(p, key);
  Serial.print("AES DECODE ");
  for (int i = 0; i < 16; i++)
    Serial.printf("%c", p[i]);
  Serial.println();

  aes.end();
}

void loop()
{
  delay(1000);
  Serial.print(".");
}