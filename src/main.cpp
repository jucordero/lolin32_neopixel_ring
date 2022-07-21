// *********
// Neopixels
// *********
#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

const uint16_t levels = 10; // Number of stacked led rings
const uint16_t PixelCount = 20*levels; // make sure to set this to the number of pixels in your strip
const uint8_t PixelPin = 13;  // make sure to set this to the correct pin, ignored for Esp8266
uint16_t grid[levels][20];

uint8_t peak = 10;

const uint16_t animTime = 15;
uint8_t prob = 10;

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
NeoPixelAnimator animations(PixelCount, NEO_CENTISECONDS);

uint8_t loopCounter = 0, angle=0;
int8_t direction = 1;

#include <effects.h>

void clear_grid(){
  for (int16_t level = levels-1; level >= 0; level--){
    for (uint16_t column = 0; column < 20; column++)
      grid[level][column] = 0;
  }
}

void SetupAnimationSet(uint8_t effect){
  switch (effect){
    case 0: fire(); break;
    case 1: water(); break;
    case 2: rain(); break;
    case 3: timer(); break;
    case 4: lamp(); break;
    case 5: random_lights(); break;
    case 6: rings(); break;
    case 7: matrix(); break;
    case 8: off(); break;
  }
}

// *********
// Webserver
// *********
#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "SKYIRTWD";
const char* password = "tecaliente_123";

WiFiServer server(80);
String header;

boolean states[] = {false, false, false, false, false, false, false, true, false, false};
const String names[] = {"fire", "water", "rain", "time", "lamp", "rand", "ring", "worm", "matrix", "off"};
const uint8_t anims = sizeof(states);

uint8_t animation_code = 7;

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

#include <webserver.h>

// ***********
// Time Server
// ***********

#include <time.h>

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

// ***********
// Main Script
// ***********

void setup(){

  Serial.begin(9600);

  strip.Begin();
  strip.Show();
  startServer();

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop(){

  checkInconmingConnections();

  if (animations.IsAnimating()){
      // the normal loop just needs these two to run the active animations
      animations.UpdateAnimations();
      strip.Show();
  }
  else{
    SetupAnimationSet(animation_code);
  }
}