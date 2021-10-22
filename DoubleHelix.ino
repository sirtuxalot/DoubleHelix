#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN             2
#define LED_TYPE       WS2812B
#define COLOR_ORDER        GRB
#define NUM_LEDS            28
CRGB leds[NUM_LEDS];
#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120
#define VOLTS                5
#define MAX_AMPS           500

void setup() {
  // 3 second delay for recovery
  delay(3000);
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set power settings
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS,MAX_AMPS);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, confetti, sinelon, rainbowWithGlitter, pride, WeAre, WeAreWithGlitter, chaser, pridewithGlitter, juggle, bpm, GoBlue, GoBlueWithGlitter };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  
void loop() {
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();
  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 
  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 30 ) { nextPattern(); } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern() {
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
  FastLED.setBrightness(BRIGHTNESS);
}

void rainbow() {
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() {
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm() {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

// chaser, GoBlue, and WeAre (latter two are breathing effect) added from http://lifemeetlightning.com/all-the-led-lights/fastled-tutorial-code/
// with some tweaks
void chaser() {
for (int i=0; i<NUM_LEDS; i++) {
    leds[i] = CRGB(0,255-4*i,4*i);
    FastLED.show();
    delay(10); //even shorter delay this time
  }
  for (int i=NUM_LEDS-1; i>=0; i--) {
    leds[i] = CRGB(4*i,0,255-4*i);
    FastLED.show();
    delay(10); //even shorter delay this time
  }
}

void GoBlue() {
for (int i=0; i<NUM_LEDS; i++) {
    leds[i] = CRGB::Yellow; 
  }
  for (int i=0; i<BRIGHTNESS; i++) {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(10);
  }
  for (int i=BRIGHTNESS; i>=0; i--) {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(10);
  }
  for (int i=NUM_LEDS-1; i>=0; i--) { 
    leds[i] = CRGB::DarkBlue; 
  }
  for (int i=0; i<BRIGHTNESS; i++) {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(10);
  }
  for (int i=BRIGHTNESS; i>=0; i--) {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(10);
  }
}

void GoBlueWithGlitter() {
  GoBlue();
  addGlitter(80);
}

void WeAre() {
for (int i=0; i<NUM_LEDS; i++) {
    leds[i] = CRGB::Yellow; 
  }
  for (int i=0; i<BRIGHTNESS; i++) {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(10);
  }
  for (int i=BRIGHTNESS; i>=0; i--) {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(10);
  }
  for (int i=NUM_LEDS-1; i>=0; i--) { 
    leds[i] = CRGB::DarkBlue; 
  }
  for (int i=0; i<BRIGHTNESS; i++) {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(10);
  }
  for (int i=BRIGHTNESS; i>=0; i--) {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(10);
  }
}

void WeAreWithGlitter() {
  WeAre();
  addGlitter(80);
}

// inserted from the FastLED Pride2015 example sketch
void pride() {
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    CRGB newcolor = CHSV( hue8, sat8, bri8);
    
    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS-1) - pixelnumber;
    
    nblend( leds[pixelnumber], newcolor, 64);
  }
}

void pridewithGlitter () {
  pride();
  addGlitter(80);
}
