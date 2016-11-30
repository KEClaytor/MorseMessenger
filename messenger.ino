// This is a demonstration on how to use an input device to trigger changes on your neo pixels.
// You should wire a momentary push button to connect from ground to a digital IO pin.  When you
// press the button it will change to a new pixel animation.  Note that you need to press the
// button once to start the first animation!

#include <Adafruit_NeoPixel.h>
#include <Bounce2.h>

#include <Arduino.h>
#include <SPI.h>
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined(ARDUINO_ARCH_SAMD)
  #include <SoftwareSerial.h>
#endif

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

// Button debouncing see: https://blog.adafruit.com/2009/10/20/example-code-for-multi-button-checker-with-debouncing/
#define DEBOUNCE 10  // button debouncer, how many ms to debounce, 5+ ms is usually plenty

// Digital IO pin connected to the button.  This will be
// driven with a pull-up resistor so the switch should
// pull the pin to ground momentarily.  On a high -> low
// transition the button press logic will execute.

Bounce bDIT = Bounce();
Bounce bDAH = Bounce();
Bounce bBRK = Bounce();
Bounce bSND = Bounce();

// Buttons for dit and dah should be self-explanitory
//  brk is used for character break (one tap) word break (two taps)
//  or sentence break (three taps).
#define BUTTON_PIN_DIT   12
#define BUTTON_PIN_DAH   9
#define BUTTON_PIN_BRK   6
#define BUTTON_PIN_SND   10

#define NUMPIXELS    1
#define PIXEL_PIN    8    // Digital IO pin connected to the NeoPixels.

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

// BLE HID Setup
#define FACTORYRESET_ENABLE         0
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"

// Create the bluefruit object, either software serial...uncomment these lines
/* ...or hardware serial, which does not need the RTS/CTS pins. Uncomment this line */
Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

// Arrays for the Morse word and the message
int morse[6];
int mi = 0;
String message = "";
int nBrk = 0;

void setup() {
  //while (!Serial);  // required for Flora & Micro
  delay(500);

  Serial.begin(115200);
  Serial.println(F("Messenger begins..."));
  Serial.println(F("---------------------------------------"));
  
  pinMode(BUTTON_PIN_DIT, INPUT_PULLUP);
  pinMode(BUTTON_PIN_DAH, INPUT_PULLUP);
  pinMode(BUTTON_PIN_BRK, INPUT_PULLUP);
  pinMode(BUTTON_PIN_SND, INPUT_PULLUP);

  // After setting up the button, setup the object
  bDIT.attach(BUTTON_PIN_DIT);
  bDIT.interval(DEBOUNCE);
  bDAH.attach(BUTTON_PIN_DAH);
  bDAH.interval(DEBOUNCE);
  bBRK.attach(BUTTON_PIN_BRK);
  bBRK.interval(DEBOUNCE);
  bSND.attach(BUTTON_PIN_SND);
  bSND.interval(DEBOUNCE);
  
  pixels.begin();
  pixels.show(); // Initialize all pixels to 'off'

  // BLE HID Keyboard setup
  
  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  /* Change the device name to make it easier to find */
  Serial.println(F("Setting device name to 'Bluefruit Keyboard': "));
  if (! ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=Bluefruit Keyboard" )) ) {
    error(F("Could not set device name?"));
      blink(150, 0, 0);
    } else {
      blink(0, 150, 0);
  }

  /* Enable HID Service */
  Serial.println(F("Enable HID Service (including Keyboard): "));
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    if ( !ble.sendCommandCheckOK(F( "AT+BleHIDEn=On" ))) {
      error(F("Could not enable Keyboard"));
      blink(150, 0, 0);
    } else {
      blink(0, 150, 0);
    }
  }else
  {
    if (! ble.sendCommandCheckOK(F( "AT+BleKeyboardEn=On"  ))) {
      error(F("Could not enable Keyboard"));
      blink(150, 0, 0);
    } else {
      blink(0, 150, 0);
    }
  }

  /* Add or remove service requires a reset */
  Serial.println(F("Performing a SW reset (service changes require a reset): "));
  if (! ble.reset() ) {
    error(F("Couldn't reset??"));
    blink(150, 0, 0);
  } else {
    blink(0, 150, 0);
  }
}

void loop() {
  // Update the bouncer states
  bDIT.update();
  bDAH.update();
  bBRK.update();
  bSND.update();

  if (bDIT.fell()) {
    Serial.println("dit");
    addMorse(1);
    //printMorse();
    //printMorseChar();
    //printMessage();
    nBrk = 0;
    blink(0, 150, 150);
  } else if (bDAH.fell()) {
    Serial.println("dah");
    addMorse(3);
    //printMorse();
    //printMorseChar();
    //printMessage();
    nBrk = 0;
    blink(150, 0, 150);
  } else if (bBRK.fell()) {
    Serial.println("brk");
    if (nBrk == 0) {
      addCharacter(readMorse());  
    } else if (nBrk == 1) {
      addCharacter(' ');
    }
    clearMorse();
    nBrk++;
    blink(150, 75, 0);
  } else if (bSND.fell()) {
    Serial.println("snd");
    printMessage();
    clearMorse();
    clearMessage();
    nBrk = 0;
  }
}

void addCharacter(char c) {
  message += c;
}

void printMessage() {
  Serial.println(message);

  char messBuff[50];
  message.toCharArray(messBuff, 50);

  ble.print("AT+BleKeyboard=");
  ble.println(messBuff);

  if( ble.waitForOK() )
  {
    Serial.println( F("OK!") );
    blink(0, 255, 0);
  }else
  {
    Serial.println( F("FAILED!") );
    blink(255, 0, 0);
  }
}

void clearMessage() {
  message.remove(0);
}

void addMorse(int i) {
  mi = mi % 6;
  morse[mi] = i;
  mi++;
}

void clearMorse() {
  for(int ii=0; ii<6; ii++) {
    morse[ii] = 0;
  }
  mi = 0;
}

void printMorseChar() {
  char mc = readMorse();
  Serial.print(" > ");
  Serial.println(mc);
}

char readMorse() {
  int total = 0;
  char mc;
  for (int ii=0; ii<6; ii++) {
    total += ipow(morse[ii]+1, ii+1);
  }
  switch (total) {
    case 22: mc = 'A'; break;
    case 34: mc = 'B'; break;
    case 90: mc = 'C'; break;
    case 19: mc = 'D'; break;
    case 7: mc = 'E'; break;
    case 88: mc = 'F'; break;
    case 31: mc = 'G'; break;
    case 32: mc = 'H'; break;
    case 10: mc = 'I'; break;
    case 340: mc = 'J'; break;
    case 75: mc = 'K'; break;
    case 44: mc = 'L'; break;
    case 24: mc = 'M'; break;
    case 12: mc = 'N'; break;
    case 87: mc = 'O'; break;
    case 100: mc = 'P'; break;
    case 286: mc = 'Q'; break;
    case 29: mc = 'R'; break;
    case 17: mc = 'S'; break;
    case 9: mc = 'T'; break;
    case 73: mc = 'U'; break;
    case 272: mc = 'V'; break;
    case 85: mc = 'W'; break;
    case 274: mc = 'X'; break;
    case 330: mc = 'Y'; break;
    case 46: mc = 'Z'; break;
    case 1363: mc = '1'; break;
    case 1351: mc = '2'; break;
    case 1295: mc = '3'; break;
    case 1055: mc = '4'; break;
    case 63: mc = '5'; break;
    case 65: mc = '6'; break;
    case 77: mc = '7'; break;
    case 133: mc = '8'; break;
    case 373: mc = '9'; break;
    case 1365: mc = '0'; break;
    case 4410: mc = '.'; break;
    case 5164: mc = ','; break;
    case 1426: mc = '"'; break;
    case 4160: mc = '-'; break;
    case 305: mc = '/'; break;
    case 1186: mc = '@'; break;
    case 1057: mc = '='; break;
    case 4456: mc = '('; break;
    case 1130: mc = ')'; break;
    default:
      mc = '*';
      break;
  }
  return mc;
}

int ipow(int base, int expn)
{
    int result = 1;
    while (expn)
    {
        if (expn & 1)
            result *= base;
        expn >>= 1;
        base *= base;
    }

    return result;
}

// Print the raw morse string we saved
void printMorse() {
  char mm[6];
  int total = 0;
  for (int mi=0; mi<6; mi++) {
    if (morse[mi] == 1) {
      Serial.print(".");
    } else if (morse[mi] == 3) {
      Serial.print("-");
    } else {
      Serial.print("x");
    }
  }
}

void blink(int red, int green, int blue) {
  // Turn on
  for(uint8_t i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(red,green,blue));
    }
  pixels.show();

  // Wait
  delay(100);

  // Turn off
  for(uint8_t i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  pixels.show();
}

