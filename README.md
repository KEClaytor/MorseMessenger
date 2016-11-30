# MorseMessenger
Send "morse" code via bluetooth and a wearable glove.
See video at: https://youtu.be/ZiRGSabL8Xk

## Configuration
You'll need the Adafruit Neopixel and Bluefruit libraries and the bounce2 library. These can be downloaded from within the arduino IDE.

## Hardware
Hardware pins are set with;
#define BUTTON_PIN_DIT   12
#define BUTTON_PIN_DAH   9
#define BUTTON_PIN_BRK   6
#define BUTTON_PIN_SND   10

These are pulled up with the internal resistor and a connection to ground (gnd) will pull them down and trigger the button press.

To configure the bluefruit in HID keyboard mode see here:
https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend/hidkeyboard

Sew some conductive thread from the Flora pins to the fingertips, making sure to not cross ground with any of your signal threads.
