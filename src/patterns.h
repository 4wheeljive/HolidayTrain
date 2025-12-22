#ifndef PATTERNS_H
#define PATTERNS_H

#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS_TRACK 226  // Redeclare the constant
extern CRGB leds_track[NUM_LEDS_TRACK];  // extern declaration - no storage allocated

//#include "C:\Users\Jeff\Documents\Coding\PlatformIO\Projects\Train\Common\twinkle.h"
//#include "C:\Users\Jeff\Documents\Coding\PlatformIO\Projects\Train\Common\palettes.h"

// COLOR SETS *****************************************************************************
/*
// For gradient palettes ============================================
extern const TProgmemRGBGradientPaletteRef gradientPalettes[]; 
extern const uint8_t gradientPaletteCount;
//uint8_t currentPaletteNumber;
//uint8_t targetPaletteNumber;
CRGBPalette16 currentPalette = bhw2xmas_gp;
//CRGBPalette16 targetPalette;

// Color nicknames
CRGB red = CRGB::DarkRed;
CRGB green = CRGB::DarkGreen;
CRGB black = CRGB::Black;
CRGB gold = CRGB::Gold;
*/


// Pulses =================================================================================
/*
CRGB pulseColor = red;
CRGB pulseTailColor = green;
uint16_t pulseRate = 250;  // lower is faster [in milliseconds]
uint8_t travelSpeed = 75;  // lower is faster [range: 0-255]
uint8_t fadeRate = 100;  // lower is shorter tail [range: 0-255]

void pulses() {

  cBrightness = 150; 
  uint8_t wave = beatsin8( 3, 0, 255); // slowly cycle between 0-255
  pulseRate = map(wave,0,255,500,1000);  // cycle between a pulseRate of 120 to 1000

  EVERY_N_MILLISECONDS(travelSpeed) {
    // move pulses down the strip
    for (int16_t i = NUM_LEDS_TRACK-1; i >= 0; i--) {
      if (leds_track[i] == pulseColor) {
        if (i == NUM_LEDS_TRACK-1) {
          leds_track[i] = pulseTailColor; // add a trail
        } else {
          leds_track[i+1] = pulseColor;
          leds_track[i] = pulseTailColor; // add a trail
        }
      }
    }

    // fade leds for tail effect
    for(int i = 0; i < NUM_LEDS_TRACK; i++) {
      if (leds_track[i] != pulseColor) {
        leds_track[i].nscale8(fadeRate);  // only fades tail pixels
      }
    }
  }

  EVERY_N_MILLISECONDS_I(timingObj,1) {
    // time to start a new pulse
    leds_track[0] = pulseColor;
    timingObj.setPeriod(pulseRate);  // use the current pulseRate
  }

  //nscale8(leds_track, NUM_LEDS_TRACK, cBrightness); 
  FastLED.show();
  
} // Pulses


// Twinkle =======================================================================

fl::TwinkleFox twinkleTrack(NUM_LEDS_TRACK);

void twinkle() {

    cBrightness = 200;
    
    EVERY_N_SECONDS(SECONDS_PER_PALETTE) {
      twinkleTrack.chooseNextColorPalette(twinkleTrack.targetPalette);
    }
    //twinkleTrack.targetPalette = twinkleTrack.ActivePaletteList[1];
    twinkleTrack.draw(fl::Fx::DrawContext(millis(), leds_track));

    //nscale8(leds_track, NUM_LEDS_TRACK, cBrightness); 
    FastLED.show();

} // runTwinkle

// Waves =================================================

void waves() {

    cBrightness = 200;

    uint16_t hueIncMax = 3000;
    CRGB newcolor = CRGB::Black;
    uint8_t blendFract = 64;
    float waveSpeed = 1.5f;

		static uint16_t sPseudotime = 0;
		static uint16_t sLastMillis = 0;
		static uint16_t sHue16 = 0;
	
		uint8_t sat8 = beatsin88( 87, 230, 255); 
		uint8_t brightdepth = beatsin88( 341, 96, 224);
		uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
		uint8_t msmultiplier = beatsin88(147, 23, 60);
	
		uint16_t hue16 = sHue16; 
		uint16_t hueinc16 = beatsin88(113, 1, hueIncMax);
		uint16_t ms = millis();  
		uint16_t deltams = ms - sLastMillis ;
		sLastMillis  = ms;     
		sPseudotime += deltams * msmultiplier * waveSpeed;
		sHue16 += deltams * beatsin88( 400, 5,9);  
		uint16_t brightnesstheta16 = sPseudotime;

		for ( uint16_t i = 0 ; i < NUM_LEDS_TRACK; i++ ) {
    
			hue16 += hueinc16;
			uint8_t hue8 = hue16 / 256;

			brightnesstheta16 += brightnessthetainc16;
			uint16_t b16 = sin16( brightnesstheta16 ) + 32768;

			uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
			uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
			bri8 += (255 - brightdepth);
		
			uint8_t index = hue8;
			index = scale8( index, 230); 
      newcolor = ColorFromPalette( currentPalette, index, bri8);

      uint16_t pixelnumber = i;
      pixelnumber = (NUM_LEDS_TRACK-1) - i;  // comment/uncomment this line to reverse apparent direction of LED progression   
  		nblend( leds_track[pixelnumber], newcolor, blendFract);

    } // strip loop

    //nscale8(leds_track, NUM_LEDS_TRACK, cBrightness);
    FastLED.show();

} // waves()
*/
// Christmas Sparkles ==========================================================

uint16_t ledsData[NUM_LEDS_TRACK][4];  // array to store color data and an extra value
uint16_t pick;  // stores a temporary pixel number

//---------------------------------------------------------------
void christmasSparkles() {

  //cBrightness = 150;
  //"Background" color for non-sparkling pixels.
  CRGB sparkleBgColor = CHSV(0, 0, 0);  // black
  //CRGB sparkleBgColor = CHSV(50, 30, 40);  // dim white
  //CRGB sparkleBgColor = CHSV(96, 200, 30);  // dim green
  
  EVERY_N_MILLISECONDS(40){
    if( random8() < 60 ) {  // How much to sparkle!  Higher number is more.
      pick = random16(NUM_LEDS_TRACK);
      if (ledsData[pick][3] == 0 ) {
        ledsData[pick][3] = 35;  // Used to tag pixel as sparkling
        uint8_t randomPick = random8(5);
        if (randomPick == 0) {
          ledsData[pick][0] = 178;   // sparkle hue (blue)
          ledsData[pick][1] = 244;  // sparkle saturation
          ledsData[pick][2] = 180;  // sparkle value (was 210)
        }
        if (randomPick == 1) {
          ledsData[pick][0] = 10;  // sparkle hue (red)
          ledsData[pick][1] = 255;  // sparkle saturation
          ledsData[pick][2] = 210;  // sparkle value (was 240)
        }
        if (randomPick == 2) {
          ledsData[pick][0] = 0;  // sparkle hue (white-ish)
          ledsData[pick][1] = 25;  // sparkle saturation
          ledsData[pick][2] = 235;  // sparkle value (was 255)
        }
        if (randomPick == 3) {
          ledsData[pick][0] = 35;   // sparkle hue (orange)
          ledsData[pick][1] = 235;  // sparkle saturation
          ledsData[pick][2] = 225;  // sparkle value (was 245)
        }
        if (randomPick == 4) {
          ledsData[pick][0] = 190;  // sparkle hue (purple)
          ledsData[pick][1] = 255;  // sparkle saturation
          ledsData[pick][2] = 215;  // sparkle value (was 238)
        }
        leds_track[pick] = CHSV(ledsData[pick][0], ledsData[pick][1], ledsData[pick][2]);
      }
    }
    for (uint16_t i=0; i < NUM_LEDS_TRACK; i++) {
      if (ledsData[i][3] == 0) {  // if not sparkling, set to "back ground" color
        leds_track[i] = sparkleBgColor;
      } else {
        CHSV hsv = rgb2hsv_approximate(leds_track[i]);  // Used to get approximate Hue
        EVERY_N_MILLISECONDS(38) { ledsData[i][0] = hsv.hue - 1; }  // slightly shift hue
        ledsData[i][2] = scale8(ledsData[i][2], 245);  // slowly darken
        leds_track[i] = CHSV(ledsData[i][0], ledsData[i][1], ledsData[i][2]);
        ledsData[i][3] = ledsData[i][3] - 1;  // countdown sparkle tag
      }
    }
  }

  //nscale8(leds_track, NUM_LEDS_TRACK, cBrightness ); // fadeAmount scale 0 = black ; 255 = full brightness
  FastLED.show();

}//end christmasSparkles


//---------------------------------------------------------------
/*
void christmasSparklesRG() {  // Red and Green only
 
  cBrightness = 150;
  //"Background" color for non-sparkling pixels.  Can be set to black for no bg color.
  CRGB sparkleBgColor = CHSV(0, 0, 0);  // black
  //CRGB sparkleBgColor = CHSV(50, 30, 30);  // dim white
 
  EVERY_N_MILLISECONDS(40){
    if( random8() < 110 ) {  // How much to sparkle!  Higher number is more.
      pick = random16(NUM_LEDS_TRACK);
      if (ledsData[pick][3] == 0 ) {
        ledsData[pick][3] = 65;  // Used to tag pixel as sparkling
        uint8_t randomPick = random8(2);
        if (randomPick == 0) {
          ledsData[pick][0] = 0;  // sparkle hue (red)
          ledsData[pick][1] = 253;  // sparkle saturation
          ledsData[pick][2] = 242;  // sparkle value
        }
        if (randomPick == 1) {
          ledsData[pick][0] = 96;   // sparkle hue (green)
          ledsData[pick][1] = 230;  // sparkle saturation
          ledsData[pick][2] = 255;  // sparkle value
        }
        leds_track[pick] = CHSV(ledsData[pick][0], ledsData[pick][1], ledsData[pick][2]);
      }
    }
    for (uint16_t i=0; i < NUM_LEDS_TRACK; i++) {
      if (ledsData[i][3] == 0) {  // if not sparkling, set to "back ground" color
        leds_track[i] = sparkleBgColor;
      } else {
        CHSV hsv = rgb2hsv_approximate(leds_track[i]);  // Used to get approximate Hue
        EVERY_N_MILLISECONDS(50) { ledsData[i][0] = hsv.hue - 1; }  // slightly shift hue
        ledsData[i][2] = scale8(ledsData[i][2], 253);  // slowly darken
        leds_track[i] = CHSV(ledsData[i][0], ledsData[i][1], ledsData[i][2]);
        ledsData[i][3] = ledsData[i][3] - 1;  // countdown sparkle tag
      }
    }
  }
  //nscale8(leds_track, NUM_LEDS_TRACK, cBrightness);
  FastLED.show();
}//end christmasSparklesRG

// Candy Cane =====================================================================================

void candyCane(){
  
  cBrightness = 100;
  CRGB color1 = CRGB::Gray50;  // color used between color 2 (and 3 if used)
  CRGB color2 = CRGB::DarkRed;
  CRGB color3 = CRGB::DarkGreen;  //optional 3rd color
  const uint16_t travelSpeed = 150;
  int shiftBy = -1;  //shiftBy can be positive or negative (to change direction)
  static uint8_t numColors = 3;  // Can be either 2 or 3
  static uint8_t stripeLength = 5;  //number of pixels per color
  static int offset;

  
  EVERY_N_SECONDS(3) {
    //numColors = random8(2,4);  //picks either 2 or 3
    stripeLength = random8(3,6);  //picks random length
  }
  

  EVERY_N_MILLISECONDS(travelSpeed) {
    if (numColors==2) {
      for (uint16_t i=0; i<NUM_LEDS_TRACK; i++){
        if ( (i+offset)%((numColors)*stripeLength)<stripeLength ) {
          leds_track[i] = color2;
        } else {
          leds_track[i] = color1;
        }
      }
    }

    if (numColors==3) {
      for (uint16_t i=0; i<NUM_LEDS_TRACK; i++){
        if ( (i+offset)%((numColors+1)*stripeLength)<stripeLength ) {
          leds_track[i] = color2;
        }
        else if ( (i+offset+(2*stripeLength))%((numColors+1)*stripeLength)<stripeLength ) {
          leds_track[i] = color3;
        } else {
          leds_track[i] = color1;
        }
      }
    }

    nscale8(leds_track, NUM_LEDS_TRACK, cBrightness);
    FastLED.show();

    offset = offset + shiftBy;
    if (shiftBy>0) {  //for positive shiftBy
      if (offset>=NUM_LEDS_TRACK) offset = 0;
    } else {  //for negitive shiftBy
      if (offset<0) offset = NUM_LEDS_TRACK;
    }

  } // EVERY_N
} // candyCane

// Moving Colors =================================================================================

// Set initial start position of each color
int16_t positionA = NUM_LEDS_TRACK*2/3;
int16_t positionB = NUM_LEDS_TRACK/3;
int16_t positionC = 0;
const uint16_t holdTime = 120;  // Adjusts speed of travel
int8_t delta = 1;  // 1 or -1.  Sets travel direction

void movingColors() {
  
  cBrightness = 150;
  EVERY_N_MILLISECONDS(holdTime) {

    // Fading tail effect.  Comment out for solid colors
    fadeToBlackBy( leds_track, NUM_LEDS_TRACK, 3); // was 10
    
    // assign pixel colors
    leds_track[positionA] = CRGB::DarkRed;
    leds_track[positionB] = CRGB::Grey;  // Using grey so not as bright
    leds_track[positionC] = CRGB::DarkGreen;
  
    //nscale8(leds_track, NUM_LEDS_TRACK, cBrightness);
    FastLED.show();  // Show the pixels
   
    // Advance position based on delta, and rollover if needed.
    positionA = ((positionA + delta + NUM_LEDS_TRACK) % NUM_LEDS_TRACK);
    positionB = ((positionB + delta + NUM_LEDS_TRACK) % NUM_LEDS_TRACK);
    positionC = ((positionC + delta + NUM_LEDS_TRACK) % NUM_LEDS_TRACK);

  }//end every_n
  
} // movingColors
*/
// ***************************************************************************************
/*
// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*PatternList[])();
PatternList Patterns = { christmasSparkles }; // , christmasSparklesRG, pulses, twinkle, waves, candyCane, movingColors,
uint8_t currentPatternNumber = 0; // Index number of which pattern is current
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
bool rotatePatterns = true;
#define SECONDS_PER_PATTERN 20
void nextPattern() {
  currentPatternNumber = (currentPatternNumber + 1) % ARRAY_SIZE(Patterns);
}
*/

#endif // PATTERNS_H