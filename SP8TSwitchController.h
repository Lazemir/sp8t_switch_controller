#ifndef SP8TSWITCHCONTROLLER_H
#define SP8TSWITCHCONTROLLER_H

#include <Arduino.h>

class SP8TSwitchController {
private:
    const byte pinA;
    const byte pinB;
    const byte pinC;

public:
  SP8TSwitchController(const byte& pinA, const byte& pinB, const byte& pinC) : pinA(pinA), pinB(pinB), pinC(pinC) {
    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);
    pinMode(pinC, OUTPUT);

    setChannel(0);
  }

  // Method to set the active channel (valid range: 0 to 7)
  void setChannel(byte channelNumber) {
    if (channelNumber > 7) {
      channelNumber = 7;
    }
    digitalWrite(pinA, channelNumber & 0x01);       // Least significant bit
    digitalWrite(pinB, (channelNumber >> 1) & 0x01); // Middle bit
    digitalWrite(pinC, (channelNumber >> 2) & 0x01); // Most significant bit
  }
};

#endif