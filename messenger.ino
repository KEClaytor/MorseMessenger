// This is a demonstration on how to use an input device to trigger changes on your neo pixels.
// You should wire a momentary push button to connect from ground to a digital IO pin.  When you
// press the button it will change to a new pixel animation.  Note that you need to press the
// button once to start the first animation!

#include <Adafruit_NeoPixel.h>
#include <Bounce2.h>

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
#define BUTTON_PIN_DAH   6
#define BUTTON_PIN_BRK   9
#define BUTTON_PIN_SND   10

#define PIXEL_PIN    8    // Digital IO pin connected to the NeoPixels.

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Arrays for the Morse word and the message
int morse[6];
int mi = 0;
int message[256];
int msi = 0;

void setup() {
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
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Update teh bouncer states
  bDIT.update();
  bDAH.update();
  bBRK.update();
  bSND.update();

  if (bDIT.fell()) {
    //morse[mi] = 1;
    //mi++;
    Serial.println("dit");
  } else if (bDAH.fell()) {
    //morse[mi] = 3;
    //mi++;
    Serial.println("dah");
  } else if (bBRK.fell()) {
    Serial.println("brk");
    printMorse();
    clearMorse();
    //if (lastPIN == BUTTON_PIN_BRK) {
      //addCharacter(' ');
    //} else {
      //addCharacter(getMorse());
      //clearMorse();
    //}
  } else if (bSND.fell()) {
    Serial.println("snd");
      //printMessage();
      //clearMessage();
  }
  
}

void clearMorse() {
  for(int ii=0; ii<6; ii++) {
    morse[ii] = 0;
  }
  mi = 0;
}

// Convert the morse code to a character
void printMorse() {
  char mm[6];
  for (int mi=0; mi<6; mi++) {
    if (morse[mi] == 1) {
      Serial.print(".");
    } else if (morse[mi] == 3) {
      Serial.print("-");
    } else {
      Serial.print("x");
    }
  }
  Serial.println("/");
  return mm;
}


void setColor(int i) {
  switch(i){
    case 0: colorWipe(strip.Color(0, 0, 0), 50);  // Black/off
            break;
    case 1: colorWipe(strip.Color(255, 0, 0), 50);      // Red
            break;
    case 2: colorWipe(strip.Color(0, 255, 0), 50);  // Green
            break;
    case 3: colorWipe(strip.Color(0, 0, 255), 50);  // Blue
            break;
    case 4: colorWipe(strip.Color(127, 127, 127), 50);  // White
            break;
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

