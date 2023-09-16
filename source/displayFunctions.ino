//    WORD CLOCK - for ESP32

void initializeLEDs(){
  delay(1000); // power-up safety delay
  FastLED.addLeds<LED_TYPE, NEOPIN>(leds, NUM_LEDS);
  FastLED.setBrightness(_DAYBRIGHTNESS);
  FastLED.clear(); // Initialize all pixels to 'off'
  FastLED.show();
}

void clearMask() {
  for (int i = 0; i < NUM_MAP; i++) {
    mask[i] = 0;
  }
}

void xsetMask(uint8_t word[],uint8_t len,uint8_t sw) {
  //len = sizeof(word)/sizeof(word[0]);
  for (int i = 0; i < len; i++) {
    mask[LED[word[i]]] = sw;
  }
}

// show colorshift through the phrase mask. for each NeoPixel either show a color or show nothing!
void applyMask() {
  for (int i = 0; i < NUM_MAP; i++) {
    if (mask[LED[i]] == 0) {
      leds[LED[i]] = CRGB( 0, 0, 0);
    } else {
      if (colormode == "Rainbow") {
         leds[LED[i]]=CRGB(col(i+r_count),col(i+r_count+256),col(i+r_count+512));
        continue;
      } 
      if (colormode == "RGB") {
        leds[LED[i]]=CRGB(colorG,colorR,colorB);
        continue;
      } 
      leds[LED[i]]=CRGB(_COLORG,_COLORR,_COLORB);
    }  
  }
  FastLED.show(); // show it!
  clearMask(); // reset mask for next time

  if (colormode == "Rainbow") {
    if ((millis()-r_millis) > interval) {
      r_count = (r_count+1) % 768; //(256 * 8); 
      r_millis = millis();
    }
  }
}

// change brightness
void adjustBrightness(int hour) {

  //set the daytime brightness
  if (morningcutoff < nightcutoff) {
      if(hour>=morningcutoff && hour<nightcutoff) {
        FastLED.setBrightness(daybrightness); //day time
        return;
        }
  } else {
      if(hour>=morningcutoff || hour<=nightcutoff) {
        FastLED.setBrightness(daybrightness); //day time
        return;
      }
  }  
  //set the nighttime brightness
  FastLED.setBrightness(nightbrightness);
}

// Lights all LEDs in rainbow colors
void rainbowCycle() {
 FastLED.setBrightness(ALLLED_BRIGHTNESS);
  int cycles = 7; // 7 cycles of all colors on wheel
  for (int j = 0; j < 256 * cycles; j++) { 
    for (int i = 0; i < NUM_MAP; i++) {
      leds[LED[i]] = CRGB(col(i+j),col(i+j+256),col(i+j+512));
    }
    FastLED.show(); // show it!
    delay(2);
  }
}

// Input a value 0 to 767 to get a color value.
int col(int pos) {
  pos = pos % 768;
  if ( pos <= 255) return(pos);
  if ( pos >= 512) return(0);
  return(255-(pos-256));
}
