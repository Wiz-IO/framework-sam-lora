## LoRaLib

More info and examples : https://github.com/jgromes/LoRaLib

## Example

```c
#include <Arduino.h>
#include <LoRaLib.h>

SX1276 lora = new LoRa;

void lora_init()
{
  // initialize SX1278 with default settings
  Serial.print(F("Initializing ... "));
  // carrier frequency:           434.0 MHz
  // bandwidth:                   125.0 kHz
  // spreading factor:            9
  // coding rate:                 7
  // sync word:                   0x12
  // output power:                17 dBm
  // current limit:               100 mA
  // preamble length:             8 symbols
  // amplifier gain:              0 (automatic gain control)
  int state = lora.begin();
  if (state == ERR_NONE)
  {
    Serial.println(F("success!"));
  }
  else
  {
    Serial.print(F("failed, code "));
    Serial.println(state);
    abort();
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("\nMicrochip SAMR34 Xplained Pro 2019 Georgi Angelov");
  lora_init();
  Serial.print(F("Waiting for incoming transmission ... "));
}

void loop()
{
  // you can receive data as an Arduino String
  // NOTE: receive() is a blocking method!
  //       See example ReceiveInterrupt for details
  //       on non-blocking reception method.
  // NOTE: for spreading factor 6, the packet length
  //       must be known in advance, and provided to
  //       receive() method!
  String str;
  int state = lora.receive(str);

  // you can also receive data as byte array
  /*
    size_t len = 8;
    byte byteArr[len];
    int state = lora.receive(byteArr, len);
  */

  if (state == ERR_NONE)
  {
    // packet was successfully received
    Serial.println(F("success!"));

    // print data of the packet
    Serial.print(F("Data:\t\t\t"));
    Serial.println(str);

    // print RSSI (Received Signal Strength Indicator) of the last received packet
    Serial.print(F("RSSI:\t\t\t"));
    Serial.print(lora.getRSSI());
    Serial.println(F(" dBm"));

    // print SNR (Signal-to-Noise Ratio) of the last received packet
    Serial.print(F("SNR:\t\t\t"));
    Serial.print(lora.getSNR());
    Serial.println(F(" dB"));

    // print frequency error of the last received packet
    Serial.print(F("Frequency error:\t"));
    Serial.print(lora.getFrequencyError());
    Serial.println(F(" Hz"));
  }
  else if (state == ERR_RX_TIMEOUT)
  {
    // timeout occurred while waiting for a packet
    Serial.print("."); //timeout
  }
  else if (state == ERR_CRC_MISMATCH)
  {
    // packet was received, but is malformed
    Serial.println(F("CRC error!"));
  }
}
```