#include <Arduino.h>
#include <FastLED.h>

#define DATA_PIN_1 D3
//#define SIGNAL_PIN D4
#define POWER_PIN D10
#define NUM_LEDS_TRACK 226
CRGB leds_track[NUM_LEDS_TRACK]; // Actual definition - storage allocated here
uint8_t BRIGHTNESS = 25;
//uint8_t cBrightness = 200; // scale 0 = black ; 255 = full brightness

bool powerOn = true;
//bool signalOn = true;
bool trackOn = true;
bool steamerOn = true;
bool stockCarOn = true;
bool lightCarOn = true;
bool cabooseOn = true;

#include "bleControl.h"
#include "espnowControl.h"
#include "patterns.h"

// ***************************************************************************************

void setup() {

  Serial.begin(115200);
  delay(1000);

  //pinMode(SIGNAL_PIN, OUTPUT);
  pinMode(POWER_PIN, OUTPUT);

  FastLED.addLeds<WS2812B, DATA_PIN_1, GRB>(leds_track, NUM_LEDS_TRACK);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  
  bleSetup();
  espnowSetup();
  
}


// ***************************************************************************************

void loop() {

  // BLE Control ---------------------------------
	// upon BLE disconnect
	if (!deviceConnected && wasConnected) {
		if (debug) {Serial.println("Device disconnected.");}
		delay(500); // give the bluetooth stack the chance to get things ready
		pServer->startAdvertising();
		if (debug) {Serial.println("Start advertising");}
		wasConnected = false;
	}

  /*
  digitalWrite(POWER_PIN,HIGH);
  Serial.println("High...");
  delay(3000);
  digitalWrite(POWER_PIN,LOW);
  Serial.println("Low...");
  delay(3000);
  */


  // System Power Control -----------------------------
  powerOn? digitalWrite(POWER_PIN, LOW) : digitalWrite(POWER_PIN, HIGH);
  
  // Ferature control -----------------------------------
  if (powerOn) {
  
    // Sub Module Control ------------------------------- 

    // Direct feature control --------------------------

      // Track lights ---
    
        if (trackOn) { 
          christmasSparkles();
          //Patterns[currentPatternNumber]();
        } else {
          FastLED.clear();
          FastLED.show();
        }
      
        /*EVERY_N_SECONDS(SECONDS_PER_PATTERN) {
          if (rotatePatterns) {
            FastLED.clear();
            FastLED.show();
            nextPattern();
          } 
        }*/
      
      // Crossing signal ---
        /*
        EVERY_N_MILLISECONDS_RANDOM(5000,15000){
          signalOn = !signalOn;
        }
        signalOn ? digitalWrite(SIGNAL_PIN, HIGH) : digitalWrite(SIGNAL_PIN, LOW);
        */ 
  } // if powerOn  
} // loop