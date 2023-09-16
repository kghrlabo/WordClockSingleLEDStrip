/*
  WordClock - for ESP32
  by kghr labo
  
  This WordClock using NeoPixel LEDs for a color shift effect.
  - The time is set automatically by NTP. (WiFi connection required)
  
  Hardware:
  - ESP32
  - NeoPixel LEDs Tape 109 (WS2812B)
  
  Wiring:
  - Solder NeoPixel 5V to ESP32 VCC(5V), GND to GND , Signal to GPIO16
  
*/
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

// include the library code:
#include <WiFi.h>
#include <FastLED.h>
#include <WebServer.h>
#include <DNSServer.h>
#include "sntp.h"
#include "SPIFFS.h"

#include "led_flat10x10.h"
#include "config.h"


void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable Brownout detector

  //Serial for debugging
  Serial.begin(115200);
  //initialize LED_BUILTIN
  pinMode(LED_BUILTIN, OUTPUT); // Lights up during Web setting function
  digitalWrite(LED_BUILTIN, LOW);  

  // initialize NeoPixel 
  initializeLEDs();
  
  //   mount filesystem
  if(!SPIFFS.begin(true)){
      Serial.println("SPIFFS Mount Failed");
      setMask(word_ERR); setMask(word_ONE);  applyMask(); // ERR ONE
      delay(60000); // hold 60sec
  } 

  // read config fileS
  if(!ReadConfigfile()) {
    Serial.println("SPIFFS read error");
    setMask(word_ERR); setMask(word_TWO);  applyMask(); // ERR TWO
    delay(2000); // hold 2sec      
    InitializeValue();
    if (!WriteConfigfile()) {
      Serial.println("SPIFFS write error");
      setMask(word_ERR); setMask(word_THREE);  applyMask(); // ERR THREE
      delay(60000); // hold 60sec      
    }
    webconfig(300L); // switch to the web config (timeout 5min = 300sec)
  }

  // Connect wifi 
  setMask(word_WIFI); applyMask(); // WIFI
  if(!connectWIFI()) {
    Serial.println("WIFI error");
    setMask(word_ERR); setMask(word_FOUR);  applyMask(); // ERR FOUR
    delay(2000); // hold 2sec    
    webconfig(60); // switch to the web config (timeout 1min = 60sec)
  }
  
  // set NTP
  if (!setNTP()) {
    Serial.println("NTP error");
    setMask(word_ERR); setMask(word_FIVE1);  applyMask(); // ERR FIVE
    delay(2000); // hold 2sec    
    webconfig(300L); // switch to the web config (timeout 5min = 300sec)
  }

  // check skipfile
  if(SPIFFS.exists(SKIPFILE)) {
    Serial.println("skipfile exists");
    SPIFFS.remove(SKIPFILE);  // remove skip file
  } else {
    Serial.println("skipfile not exists");
    webconfig(30); // switch to the web config (timeout 30sec)
  }

  Serial.println("WORDCLOCK");
  // startup sequence...
  rainbowCycle(); // Welcome illumination

}

void loop() {
  getLocalTime(&now);
  displayTime(now.tm_hour, now.tm_min, now.tm_sec);
  checkCondition(now.tm_hour);
}
