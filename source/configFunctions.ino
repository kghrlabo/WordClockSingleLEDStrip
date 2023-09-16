//    WORD CLOCK - for ESP32
//
//*******************************************

void InitializeValue() {
  ssid = _SSID;
  passwd = _PASSWD;
  timezone = _TIMEZONE;
  showampm = _SHOWAMPM;
  showseconds = _SHOWSECONDS;
  showminutes = _SHOWMINUTES;
  colormode = _COLORMODE;
  interval = _INTERVAL;
  colorR = _COLORR;
  colorG = _COLORG;
  colorB = _COLORB;
  daybrightness = _DAYBRIGHTNESS;
  nightbrightness = _NIGHTBRIGHTNESS;
  morningcutoff = _MORNINGCUTOFF;
  nightcutoff = _NIGHTCUTOFF;
}

//*******************************************
bool WriteConfigfile(){
  Serial.printf("SPIFFS writing file: %s\n", CONFIGFILE);
  File fp = SPIFFS.open(CONFIGFILE, "w");
  if (!fp) return(false);

  fp.println(ssid);
  fp.println(passwd);
  fp.println(timezone);
  fp.println(showampm);
  fp.println(showseconds);
  fp.println(showminutes);
  fp.println(colormode);
  fp.println(String(interval));
  fp.println(String(colorR));
  fp.println(String(colorG));
  fp.println(String(colorB));
  fp.println(String(daybrightness));
  fp.println(String(nightbrightness));
  fp.println(String(morningcutoff));
  fp.println(String(nightcutoff));
  fp.close();

  return(true);
}

//*******************************************
bool ReadConfigfile(){
  String numstr;
  File fp = SPIFFS.open(CONFIGFILE, "r");
  if (!fp) return(false);

  ssid = fp.readStringUntil('\n');  
  if (ssid == "") return(false);
  passwd = fp.readStringUntil('\n');
  if (passwd == "" && ssid == _SSID) passwd = _PASSWD;
  timezone = fp.readStringUntil('\n');
  if (timezone == "" )  return(false);
  showampm =  fp.readStringUntil('\n');
  if (showampm == "")  return(false);
  showseconds =  fp.readStringUntil('\n');
  if (showseconds == "")  return(false);
  showminutes =  fp.readStringUntil('\n');
  if (showminutes == "")  return(false);

  colormode =  fp.readStringUntil('\n');
  if (colormode== "")  return(false);
  numstr =  fp.readStringUntil('\n');
  if (numstr == "")  return(false);   else interval = numstr.toInt(); 
  numstr =  fp.readStringUntil('\n');
  if (numstr == "")  return(false);   else colorR = numstr.toInt();
  numstr =  fp.readStringUntil('\n');
  if (numstr == "")  return(false);   else colorG = numstr.toInt();
  numstr =  fp.readStringUntil('\n');
  if (numstr == "")  return(false);   else colorB = numstr.toInt();

  numstr =  fp.readStringUntil('\n');
  if (numstr == "")  return(false);   else daybrightness = numstr.toInt();
  numstr =  fp.readStringUntil('\n');
  if (numstr == "")  return(false);   else nightbrightness = numstr.toInt();
  numstr =  fp.readStringUntil('\n');
  if (numstr == "")  return(false);   else morningcutoff = numstr.toInt();
  numstr =  fp.readStringUntil('\n');
  if (numstr == "")  return(false);   else nightcutoff = numstr.toInt();

  fp.close();

  ssid.trim(); 
  passwd.trim(); 
  timezone.trim(); 
  showampm.trim(); 
  showseconds.trim(); 
  showminutes.trim(); 
  colormode.trim(); 

  return(true);
}

void timeavailable(struct timeval *t ){
  Serial.println("SNTP_SYNC_STATUS_COMPLETED");
}

bool connectWIFI() {

  int retry = 0;
 
  WiFi.begin(ssid.c_str(), passwd.c_str());
  Serial.println("WiFi.begin");  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    retry ++;
    delay(200); 
    if (retry > WIFIRETRY) return(false); 
  }

  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  return(true);
}

void checkCondition(int hour) {
  // Check every hour
  if (hour == exhour) return; 
  exhour = hour;

  // Reconnect if WIFI is disconnected.
  if (WiFi.status() != WL_CONNECTED) connectWIFI();

  // check brightness
  adjustBrightness(hour);

}

bool setNTP() {
  gmtOffset_sec = timezone.toFloat()*3600L; // defallt JST +9
  configTime(gmtOffset_sec, 0, NTP_SERVER);
  delay(1000); // hold 1sec

  // Get local time
  if(!getLocalTime(&now)) return(false);
  Serial.println(&now, "%A, %B %d %Y %H:%M:%S");

  // set notification sync callback for debug
  sntp_set_time_sync_notification_cb(timeavailable);
  return(true);
}
