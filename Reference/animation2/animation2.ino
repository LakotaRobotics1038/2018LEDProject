#include <Adafruit_NeoPixel.h>
#include "WS2812_Definitions.h"

#define PIN 4
#define LED_COUNT 30
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);
void setup()
{
  // put your setup code here, to run once:
  leds.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:
  rotate (GOLD, BLUE, 30, 50);
}

void rotate (unsigned long colora, unsigned long colorb, byte longshort, unsigned long wait)
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    clearLEDs();
    for (int j = i; j < LED_COUNT; j = j + longshort * 2)       // in front
    {
      for (int k = 0; k < longshort; k++)                       //color a
      {
        leds.setPixelColor(j + k, colora);
      }
      for (int m = longshort; m < longshort * 2; m++)
      {
        leds.setPixelColor(j + m, colorb);
      }
    }
    for (int n = i; n > 0; n = n - longshort * 2)
    {
      for (int k = 0; k < longshort; k++)
      {
        leds.setPixelColor(n - k - 1, colorb);
      }
      for (int m = longshort; m < longshort * 2; m++)
      {
        leds.setPixelColor(n - m - 1, colora);
      }
    }
    leds.show();
    delay (wait);
  }
}
void clearLEDs()
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    leds.setPixelColor(i, 0);
  }
}
