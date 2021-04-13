#include <iarduino_RTC.h>
iarduino_RTC watch(RTC_DS1302, 8, 6, 7);

#include <FastLED.h>
#define NUM_LEDS 60
#define DATA_PIN 4
CRGB LEDs[NUM_LEDS];
CRGB colorHour = CRGB::Red;
CRGB colorMinute = CRGB::White;
CRGB colorSecond = CRGB::Purple;
CRGB colorHourMinute = CRGB::Yellow;
CRGB colorHourSecond = CRGB::Magenta;
CRGB colorMinuteSecond = CRGB::Blue;
CRGB colorAll = CRGB::Green;

#define USE_LED_MOVE_BETWEEN_HOURS false
#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


void setup() {
  delay(300);
  Serial.begin(9600);
  watch.begin();
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(LEDs, NUM_LEDS);  
  FastLED.setBrightness(255);
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
}

void loop() {
  readTheTime();
  if(watch.minutes == 0)
  {
    ChangePalettePeriodically();
    
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    
    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
  }
  else{
  for (int i=0; i<NUM_LEDS; i++) 
      LEDs[i] = CRGB::Black;

    int second = watch.seconds;
    int minute = watch.minutes;
    int hour = getLEDHour(watch.hours, watch.minutes);
    Serial.print(second);
    Serial.print(minute);
    Serial.println(hour);
    // Set "Hands"
    LEDs[second] = colorSecond;
    LEDs[minute] = colorMinute;  
    LEDs[hour] = colorHour;  

    // Hour and min are on same spot
    if ( hour == minute)
      LEDs[hour] = colorHourMinute;

    // Hour and sec are on same spot
    if ( hour == second)
      LEDs[hour] = colorHourSecond;

    // Min and sec are on same spot
    if ( minute == second)
      LEDs[minute] = colorMinuteSecond;

    // All are on same spot
    if ( minute == second && minute == hour)
      LEDs[minute] = colorAll;
    FastLED.setBrightness(255);
    FastLED.show();
    delay(1000);
  }
}

void readTheTime(){
  // Ask the clock for the data.
  watch.gettime();
}


byte getLEDHour(byte hours, byte minutes) {
  if (hours > 12)
    hours = hours - 12;

  byte hourLED;
  if (hours <= 5) 
    hourLED = (hours * 5) + 30;
  else
    hourLED = (hours * 5) - 30;

  if (USE_LED_MOVE_BETWEEN_HOURS == true) {
    if        (minutes >= 12 && minutes < 24) {
      hourLED += 1;
    } else if (minutes >= 24 && minutes < 36) {
      hourLED += 2;
    } else if (minutes >= 36 && minutes < 48) {
      hourLED += 3;
    } else if (minutes >= 48) {
      hourLED += 4;
    }
  }

  return hourLED;  
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        LEDs[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};
