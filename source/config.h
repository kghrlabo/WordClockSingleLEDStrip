//    WORD CLOCK - for ESP32

#define CONFIGFILE "/wc_contig.txt"
#define SKIPFILE "/skip.txt"

// define pins
#define NEOPIN 16 // connect to DIN on NeoPixel
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

// wifi setting
#define _SSID "yourssid" // SSID for internet connect
#define _PASSWD "12345678" // SSID password  for internet connect
#define WIFIRETRY 20 // WIFI connection retry count
#define APSSID "WordClock" // SSID for WebConfig (no password required)
#define SSIDMAX 20 // Maximum SSID display 

// Timezone
#define _TIMEZONE "9" // it is string value
#define NTP_SERVER "pool.ntp.org"

// LED color setting
#define _COLORMODE "Rainbow" // Rainbow or RGB
#define _COLORR 44 // 0-255
#define _COLORG 255 // 0-255
#define _COLORB 59 // 0-255
#define _INTERVAL 100 // controls rainbow color shifting speed ms

// brightness based on time of day
#define _DAYBRIGHTNESS 180 // 0-200 
#define _NIGHTBRIGHTNESS 90 // 0-200

// cutoff times for day / night brightness. feel free to modify.
#define _MORNINGCUTOFF 7 // when does daybrightness begin? 7am
#define _NIGHTCUTOFF 22 // when does nightbrightness begin? 10pm

// show AMPM
#define _SHOWAMPM "disable" // "enable" or "disable"

// show seconds
#define _SHOWSECONDS "enable" // "enable" or "disable"

// show minuts
#define _SHOWMINUTES "enable" // "enable" or "disable"

#define setMask(word) xsetMask(word,sizeof(word),1) // set mask 1
#define resetMask(word) xsetMask(word,sizeof(word),0) // set mask 0


// SPIFFS record parameter
String ssid = _SSID;
String passwd = _PASSWD;
String timezone = _TIMEZONE;
String showampm = _SHOWAMPM;
String colormode = _COLORMODE;
String showseconds = _SHOWSECONDS;
String showminutes = _SHOWMINUTES;
uint8_t interval = _INTERVAL;
uint8_t colorR = _COLORR;
uint8_t colorG = _COLORG;
uint8_t colorB = _COLORB;
uint8_t daybrightness = _DAYBRIGHTNESS;
uint8_t nightbrightness = _NIGHTBRIGHTNESS;
uint8_t morningcutoff = _MORNINGCUTOFF;
uint8_t nightcutoff = _NIGHTCUTOFF;

// global variables
CRGB leds[NUM_LEDS];
const uint8_t NUM_MAP = sizeof(LED)/sizeof(LED[0]); // number of mapping elements
uint8_t mask[NUM_MAP]; // LEDs ON/OFF
unsigned long timeoutmillis; // 5min x 60sec x 1000
struct tm now; // Holds current clock time
uint8_t exhour = 0; 
unsigned long extouch = 0; //
uint r_count = 0; // an integer for the color shifting effect
unsigned long r_millis = millis(); // 
long gmtOffset_sec = 0; // initial Time zone : (GMT+00:00) Greenwich Mean Time
bool configblink = true;

// Web config 
IPAddress apIP(192,168,1,100);
IPAddress apSubnet(255,255,255,0);
WebServer webServer(80);
DNSServer dnsServer;
