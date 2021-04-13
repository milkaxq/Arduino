#include <iarduino_RTC.h>
iarduino_RTC watch(RTC_DS1302, 8, 6, 7);

#include <FastLED.h>
#define NUM_LEDS 60
#define DATA_PIN 4
CRGB LEDs[NUM_LEDS];
CRGB colorHour = CRGB::Red;
CRGB colorMinute = CRGB::Green;
CRGB colorSecond = CRGB::Blue;
CRGB colorHourMinute = CRGB::Yellow;
CRGB colorHourSecond = CRGB::Magenta;
CRGB colorMinuteSecond = CRGB::Cyan;
CRGB colorAll = CRGB::White;

#define USE_LED_MOVE_BETWEEN_HOURS true

void setup() {
  delay(300);
  Serial.begin(9600);
  watch.begin();
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(LEDs, NUM_LEDS);  
  FastLED.setBrightness(255);
}

void loop() {
  readTheTime();
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
