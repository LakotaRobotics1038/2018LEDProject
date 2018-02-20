#include <Adafruit_NeoPixel.h>
#include "WS2812_Definitions.h"

#define PIN 4
#define LED_COUNT 135

#define LEFT_TOWER_START 0
#define LEFT_TOWER_COUNT 40
#define RIGHT_TOWER_START 40
#define RIGHT_TOWER_COUNT 40

#define NUMBER_TOP_START 80
#define NUMBER_TOP_COUNT 4              //may need to be switched
#define NUMBER_BOTTOM_START 84
#define NUMBER_BOTTOM_COUNT 4

#define LEFT_BASE_START 88
#define LEFT_BASE_COUNT 14
#define NAME_BOTTOM_START 102
#define NAME_BOTTOM_COUNT 9
#define BLINKER_START 111
#define BLINKER_COUNT 1
#define NAME_TOP_START 112
#define NAME_TOP_COUNT 9
#define RIGHT_BASE_START 121
#define RIGHT_BASE_COUNT 14





boolean highGear = false;
boolean redTeam = true;
boolean nameBlinkOn = false;
boolean towerBlinkOn = false;
int tower = 0;
int ani = 0;
int mode = 0;

Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);
void setup()
{
  leds.begin();
  Serial.begin(9600);
  while (!Serial)
  {
  }
}

void loop()
{
  /*FUTURE LED KID: use a switch case to set the
    variables. We were too lazy to change it this
    year. Or come up with something better. Honestly,
    this code sucks.*/
  for (int wait = 0; wait < 50; wait++)       //delay loop (wait 10 : ani 1)
  {
    byte var = Serial.read();                      //read serial
    if (var == 'H')                 //gear
    {
      highGear = true;
    }
    else if (var == 'L')
    {
      highGear = false;
    }
    else if (var == 'R')            //team and comp mode
    {
      redTeam = true;
      mode = 1;
    }
    else if (var == 'B')
    {
      redTeam = false;
      mode = 1;
    }
    else if (var == 's')            //tower
    {
      tower = 0;
    }
    else if (var == 'f')
    {
      tower = 1;
    }
    else if (var == 'b')
    {
      tower = 2;
    }
    else if (var == 't')
    {
      tower = 3;
    }
    else if (var == 'm')
    {
      tower = 4;
    }
    else if (var == 'p')
    {
      tower = 5;
    }
    else if (var == 'c')
    {
      tower = 6;
    }
    else if (var == 'D')          //mode
    {
      mode = 0;
    }
    else if (var == 'E')
    {
      mode = 2;
    }
    delay (1);
  }

  ++ani;              //updates all animations

  if (ani > 193)      //resets the amimation value so that is does not slow down
  {
    ani = 0;
  }

  if (ani % 10 > 1)    //necessaary for blinking code
  {
    towerBlinkOn = false;
  }
  else
  {
    towerBlinkOn = true;
  }

  switch (mode)                             //mode
  {
    case 0:                     //demo mode (our team colors)
      rotate (BLUE, PURPLE, 5);
      break;
    case 1:                     //comp mode (the important stuff)
      for (int i = 0; i < LED_COUNT; i++)
      {
        if (i == BLINKER_START)
        {
          blinker(i);
        }
        if (i < RIGHT_TOWER_START + RIGHT_TOWER_COUNT)
        {
          towerLights(i);
        }
        if (i >= LEFT_BASE_START && i < LEFT_BASE_START + LEFT_BASE_COUNT || i >= RIGHT_BASE_START && i < RIGHT_BASE_START + RIGHT_BASE_COUNT)
        {
          wheelWells(i);
        }
        if (i >= NAME_TOP_START && i < NAME_TOP_START + NAME_TOP_COUNT || i >= NAME_BOTTOM_START && i < NAME_BOTTOM_START + NAME_BOTTOM_COUNT)
        {
          nameNumber(i);
        }
        if (i >= NUMBER_TOP_START && i < NUMBER_TOP_START + NUMBER_TOP_COUNT || i >= NUMBER_BOTTOM_START && i < NUMBER_BOTTOM_START + NUMBER_BOTTOM_COUNT)
        {
          nameNumber(i);
        }
      }
      break;
    case 2:                     //E-stop mode (RAINBOW)
      rainbow (ani);
      break;
  }
  leds.show();
}



void nameNumber(int ledNum)
{
  if (highGear)
  {
    leds.setPixelColor(ledNum, GREEN);
  }
  else
  {
    leds.setPixelColor(ledNum, PURPLE);
  }
}
void wheelWells(int ledNum)
{
  if (redTeam)
  {
    leds.setPixelColor(ledNum, RED);
  }
  else
  {
    leds.setPixelColor(ledNum, BLUE);
  }
}

void towerLights(int ledNum)
{
  switch (tower)
  {
    case 0:   // does not see a cube: blinking yellow
      if (!towerBlinkOn)
      {
        leds.setPixelColor(ledNum, ORANGE);
      }
      else
      {
        leds.setPixelColor(ledNum, BLACK);
      }
      break;
    case 1:   // found cube:solid yellow
      leds.setPixelColor(ledNum, ORANGE);
      break;
    case 2:   //button pressed:black
      leds.setPixelColor(ledNum, BLACK);
      break;
    case 3:    //robot turns to face block: lower tower is red
      leds.setPixelColor(ledNum, BLACK);
      lowerThird (ledNum);
      break;
    case 4:   //robot moves towards block: lower tower is still red, middle tower is now yellow
      leds.setPixelColor(ledNum, BLACK);
      middleThird (ledNum);
      lowerThird (ledNum);
      break;
    case 5:   //robot pulls block in: lower is still red, middle is still yellow, top is now green
      leds.setPixelColor(ledNum, BLACK);
      topThird (ledNum);
      middleThird (ledNum);
      lowerThird(ledNum);
      break;
    case 6:   //robot closes around bock: the whole tower flashes green
      leds.setPixelColor(ledNum, GREEN);
      break;
  }
}

void blinker(int ledNum)                                 //blinking cursor
{
  if (!nameBlinkOn && ani % 10 == 1)
  {
    leds.setPixelColor(ledNum, DEEPPINK);
    nameBlinkOn = true;
  }
  else
  {
    leds.setPixelColor(ledNum, BLACK);
    nameBlinkOn = false;
  }
}

void rainbow(byte startPosition)                          //rainbow code
{
  /*I origionally found this code on google, so if its
    bad, it's not entirely my fault. If you want to see
    the original pseudocode look in the WS2812_Breakout_Example
    file.*/
  int rainbowScale = 192 / (.25 * LED_COUNT);        //this sets the scale of the rainbow. smaller LED_COUNT = more rainbows. larger LED_COUNT = longer rainbows

  for (int i = 0; i < LED_COUNT; i++)
  {
    int orderNum = (rainbowScale * (i + startPosition)) % 192;  //a number between 0 and 191 that determines where on the rainbow the LED is

    //use this to trouble shoot if the annimation glitches
    /*Serial.print(startPosition);
      Serial.print (" : ");
      Serial.println(orderNum);*/

    leds.setPixelColor(i, rainbowOrder(orderNum));
  }
}

uint32_t rainbowOrder(byte position)
{
  if (position < 31)                      //0-30 => red-yellow
  {
    return leds.Color(0xFF, position * 8, 0);
  }
  else if (position < 63)                 //30-62 => yellow-green
  {
    position -= 31;
    return leds.Color(0xFF - position * 8, 0xFF, 0);
  }
  else if (position < 95)                 //63-94 => green-aqua
  {
    position -= 63;
    return leds.Color(0, 0xFF, position * 8);
  }
  else if (position < 127)                //95-126 => aqua-blue
  {
    position -= 95;
    return leds.Color(0, 0xFF - position * 8, 0xFF);
  }
  else if (position < 159)                //127-158 => blue-purple
  {
    position -= 127;
    return leds.Color(position * 8, 0, 0xFF);
  }
  else if (position < 191)                //159-190 => purple-red
  {
    position -= 159;
    return leds.Color(0xFF, 0x00, 0xFF - position * 8);
  }
  /*when 191 goes into the above statement, it
    makes the LED purple and not red, so we set it
    manually to prevent a "runner"*/
  else                                   //191 => red
  {
    return leds.Color(0xFF, 0x00, 0x00);
  }
}

void rotate (unsigned long colora, unsigned long colorb, byte longshort)    //rotate code
/* I'm sorry this code is poorly written, but it works
  so I'm not going to try to make it more efficent. Sorry*/
{
  for (int i = ani; i < LED_COUNT; i = i + longshort * 2)         //sets LEDs in front of ani pixle
  {
    for (int j = 0; j < longshort; j++)                 //sets first color
    {
      leds.setPixelColor(i + j, colora);
    }
    for (int j = longshort; j < longshort * 2; j++)     //sets second color
    {
      leds.setPixelColor(i + j, colorb);
    }
  }
  for (int i = ani; i > 0; i = i - longshort * 2)                 //sets LEDs behind ani pixle
  {
    for (int j = 0; j < longshort; j++)                //sets first color
    {
      leds.setPixelColor(i - j - 1, colorb);
    }
    for (int j = longshort; j < longshort * 2; j++)   //sets second color
    {
      leds.setPixelColor(i - j - 1, colora);
    }
  }
}

// control tower progress bar

void lowerThird (int ledNum)                  // lower third red
{
  if (ledNum < (LEFT_TOWER_COUNT / 3) || RIGHT_TOWER_START + (RIGHT_TOWER_COUNT * 2) / 3 < ledNum)
  {
    leds.setPixelColor(ledNum, RED);
  }
}

void middleThird (int ledNum)                 //middle third yellow
{
  if ((LEFT_TOWER_COUNT / 3) <= ledNum && ledNum < (LEFT_TOWER_COUNT * 2) / 3)
  {
    leds.setPixelColor(ledNum, ORANGE);
  }
  if ( RIGHT_TOWER_START + (RIGHT_TOWER_COUNT / 3) < ledNum && ledNum <= RIGHT_TOWER_START + (RIGHT_TOWER_COUNT * 2) / 3)
  {
    leds.setPixelColor(ledNum, ORANGE);
  }
}

void topThird (int ledNum)                    //top third green
{
  if ((LEFT_TOWER_COUNT * 2 / 3) <= ledNum < LEFT_TOWER_COUNT || RIGHT_TOWER_START <= ledNum <= RIGHT_TOWER_START + RIGHT_TOWER_COUNT / 3)
  {
    leds.setPixelColor(ledNum, GREEN);
  }
}

