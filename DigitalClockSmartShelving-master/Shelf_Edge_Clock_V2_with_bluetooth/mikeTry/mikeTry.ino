#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#endif

#include <iarduino_RTC.h>
iarduino_RTC watch(RTC_DS1302, 8, 6, 7);

#include <FastLED.h>

#define LED_PIN     4
#define NUM_LEDS    276
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

// Which pin on the Arduino is connected to the NeoPixels?
#define LEDCLOCK_PIN    4
#define LEDDOWNLIGHT_PIN    5
#define downled_pin 3

// How many NeoPixels are attached to the Arduino?
#define LEDCLOCK_COUNT 276
#define LEDDOWNLIGHT_COUNT 42
#define led 3

  //(red * 65536) + (green * 256) + blue ->for 32-bit merged colour value so 16777215 equals white
int clockMinuteColour = 1677; //1677
int clockHourColour = 1677; //7712
int state = 1;
int clockFaceBrightness = 0;

// Declare our NeoPixel objects:
Adafruit_NeoPixel stripClock(LEDCLOCK_COUNT, LEDCLOCK_PIN, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel stripDownlighter(LEDDOWNLIGHT_COUNT, LEDDOWNLIGHT_PIN, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel stripDownlighter2(led, downled_pin, NEO_RGB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

int state_select = 0;

void setup() {
  delay(300);
  Serial.begin(9600);
  watch.begin();

  stripClock.begin();           // INITIALIZE NeoPixel stripClock object (REQUIRED)
  stripClock.show();            // Turn OFF all pixels ASAP
  stripClock.setBrightness(255); // Set inital BRIGHTNESS (max = 255)
 

  stripDownlighter.begin();           // INITIALIZE NeoPixel stripClock object (REQUIRED)
  stripDownlighter.show();            // Turn OFF all pixels ASAP
  stripDownlighter.setBrightness(150); // Set BRIGHTNESS (max = 255)

  stripDownlighter2.begin();           // INITIALIZE NeoPixel stripClock object (REQUIRED)
  stripDownlighter2.show();            // Turn OFF all pixels ASAP
  stripDownlighter2.setBrightness(150); // Set BRIGHTNESS (max = 255)
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;

}

void loop() {
  readTheTime();
  // if (watch.minutes == 0){

  // }

  switch (state_select){
    case 1:
      flashClock();
      break;
    case 0:
      showClock();
      break;
  }

  if (Serial.available()!=0){
    String command = Serial.readStringUntil('/n');
    Serial.println(command.length());
    command.trim();
    if (command.length() > 0){
      if (command == "foo"){
        state_select = 1;
      }
      if (command == "bar"){
        state_select = 0;
      }
    }
  }
}

void flashClock(){
  
  stripClock.clear();
  stripDownlighter.setBrightness(0); // Turn OFF all pixels ASAP
  stripDownlighter2.setBrightness(0);
  stripDownlighter2.show();
  stripDownlighter.show();
  stripClock.clear();
  ChangePalettePeriodically();  
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */  
  FillLEDsFromPaletteColors( startIndex);  
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void showClock(){
  stripClock.clear();
  stripDownlighter.setBrightness(150); // Turn OFF all pixels ASAP
  stripDownlighter2.setBrightness(150);
  stripDownlighter2.show();
  stripDownlighter.show();
  stripClock.clear(); 
  //display the time on the LEDs
  displayTheTime();
  stripClock.show();
   //(red * 65536) + (green * 256) + blue ->for 32-bit merged colour value so 16777215 equals white
  stripDownlighter.fill(170000, 0, LEDDOWNLIGHT_COUNT);
  stripDownlighter2.fill(170000, 0, led);
  stripDownlighter.show();
  stripDownlighter2.show();
  // delay(5000);
}

void readTheTime(){
  // Ask the clock for the data.
  watch.gettime();
  // And use it
//  Serial.println("");
//  Serial.print("Time is: ");   Serial.print(watch.Hours);
//  Serial.print(":"); Serial.print(watch.minutes);
//  Serial.print(":"); Serial.println(watch.seconds);
//  Serial.print("Date is: 20");   Serial.print(watch.year);
//  Serial.print(":");  Serial.print(watch.month);
//  Serial.print(":");    Serial.println(watch.day);
}

void displayTheTime(){

  stripClock.clear(); //clear the clock face 

  
  int firstMinuteDigit = watch.minutes % 10; //work out the value of the first digit and then display it
  displayNumber(firstMinuteDigit, 0, clockMinuteColour);

  
  int secondMinuteDigit = floor(watch.minutes / 10); //work out the value for the second digit and then display it
  displayNumber(secondMinuteDigit, 84, clockMinuteColour);  


  int firstHourDigit = watch.Hours; //work out the value for the third digit and then display it
  if (firstHourDigit >= 12){
    firstHourDigit = firstHourDigit - 12;
  }
 
 // Comment out the following three lines if you want midnight to be shown as 12:00 instead of 0:00
//  if (firstHourDigit == 0){
//    firstHourDigit = 12;
//  }
 
  firstHourDigit = firstHourDigit % 10;
  displayNumber(firstHourDigit, 168, clockHourColour);


  int secondHourDigit = watch.Hours; //work out the value for the fourth digit and then display it

// Comment out the following three lines if you want midnight to be shwon as 12:00 instead of 0:00
//  if (secondHourDigit == 0){
//    secondHourDigit = 12;
//  }
 
 if (secondHourDigit >= 12){
    secondHourDigit = secondHourDigit - 12;
  }
    if (secondHourDigit > 9){
      stripClock.fill(clockHourColour, 252, 24); 
    }

  }


void displayNumber(int digitToDisplay, int offsetBy, int colourToUse){
    switch (digitToDisplay){
    case 0:
    digitZero(offsetBy,colourToUse);
      break;
    case 1:
      digitOne(offsetBy,colourToUse);
      break;
    case 2:
    digitTwo(offsetBy,colourToUse);
      break;
    case 3:
    digitThree(offsetBy,colourToUse);
      break;
    case 4:
    digitFour(offsetBy,colourToUse);
      break;
    case 5:
    digitFive(offsetBy,colourToUse);
      break;
    case 6:
    digitSix(offsetBy,colourToUse);
      break;
    case 7:
    digitSeven(offsetBy,colourToUse);
      break;
    case 8:
    digitEight(offsetBy,colourToUse);
      break;
    case 9:
    digitNine(offsetBy,colourToUse);
      break;
    default:
     break;
  }
}
void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
        if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
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
