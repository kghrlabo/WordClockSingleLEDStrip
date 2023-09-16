//    WORD CLOCK - for ESP32

void webconfig(unsigned long timeoutsec) {

  Serial.println("Start Web server for config: ");
  StartWebServer(); 

  // skip the webconfig on next startup
  SPIFFS.open(SKIPFILE, "w");

  timeoutmillis =  millis() + (timeoutsec * 1000L); // convert sec to millsec
  configblink = true;

  while (millis() < timeoutmillis) {

    dnsServer.processNextRequest();
    webServer.handleClient();

    // blink LED while web server is active
    if (millis() % 1000L < 500L) {
      digitalWrite(LED_BUILTIN, HIGH); 
      setMask(word_WIFI);  setMask(word_WORD);  setMask(word_CLOCK); // LED "WIFI WORD CLOCK"
      applyMask();
    } else {
       if (configblink==true) {
        digitalWrite(LED_BUILTIN, LOW);
        clearMask();
        applyMask();
      }
    }
    
  }
  // timeout web config
  Serial.println("timeout webconfig");

  _reboot();
}

void StartWebServer() {
  // disconnect WiFi 
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  delay(200); //hold 200msec

  // Create a WiFi access point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(APSSID); // no password required
  delay(200); // Important! This delay is mandatory. []
  WiFi.softAPConfig(apIP,apIP,apSubnet); 
  // set dns
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", apIP);
  delay(200); //hold 200msec
  // set website
  webServer.on("/", configtop);
  webServer.on("/wifi", HTTP_GET, configwifi);
  webServer.on("/setwifi", HTTP_GET,setwifi);
  webServer.on("/parm", HTTP_GET, configparm);
  webServer.on("/setparm", HTTP_GET,setparm);
  webServer.on("/reboot", HTTP_GET, reboot);
  webServer.on("/_reboot", HTTP_GET,_reboot);
  webServer.on("/resetparm", HTTP_GET, resetparm);
  webServer.on("/_resetparm", HTTP_GET, _resetparm);
  webServer.onNotFound(configtop);
  // start web server
  webServer.begin();
  delay(200); //hold 200msec
}

String maskpasswd (String passwd){
  String maskpasswd = "***************...";
  if (passwd.length() == 0) {
    maskpasswd = "(null)";
  } else if (passwd.length() < 15 ) { 
      maskpasswd = maskpasswd.substring(1, passwd.length());
  }
  if ( ssid == _SSID && maskpasswd == _PASSWD) maskpasswd == _PASSWD; // check this logic
  return maskpasswd;
}


//*******************************************
void configtop() {
  timeoutmillis = millis() + 180000L; // reset timeout : 3min x 60sec x 1000
  configblink = false;

  String html = Headder_str();
  html += "<a href='/wifi'>WIFI</a> <a href='/parm'>Parameter</a> <a href='/resetparm'>Reset</a>";
  html += "<hr>";
  html += "<h3>Current Settings</h3>";
  html += "<p>SSID: " + ssid + "<br>";
  html += "Password: " + maskpasswd(passwd) + "<br>";
  html += "Timezone: " + timezone + "<br>";
  html += "Color mode: " + colormode + "<br>";  
  if (colormode=="Rainbow"){
    html += "Interval (msec) : " + String(interval) + "<br>"; 
  }
  if (colormode=="RGB"){
    html += "(R,G,B)= (" + String(colorR) + ","+String(colorG) + ","+ String(colorB) + ")<br>"; 
  }
  html += "Daybrightness: " + String(daybrightness) + "<br>";  
  html += "Nightbrightness: " + String(nightbrightness) + "<br>";  
  html += "Morningcutoff: " + String(morningcutoff) + "<br>";  
  html += "Nightcutoff: " + String(nightcutoff) + "<br>";
  html += "Show AM/PM: " + showampm + "<br>";
  html += "Show Seconds: " + showseconds + "<br>";
  html += "Show Minutes: " + showminutes + "<br>";
  html += "</p><hr>";
  html += "<a href='/reboot'>Reboot</a>";
  html += "</body></html>";
  webServer.send(200, "text/html", html);
}

//*******************************************
String Headder_str() {
  String html = " \
  <!DOCTYPE html> \
  <html> \
  <head> \
    <meta name='viewport' content='width=device-width, initial-scale=1.3'> \
    <meta http-equiv='Pragma' content='no-cache'> \
    <meta http-equiv='Cache-Control' content='no-cache'> \
  </head> \
  <meta http-equiv='Expires' content='0'> \
  <style> \
      a:link, a:visited { background-color: #009900; color: white; padding: 5px 15px; \
      text-align: center; text-decoration: none;  display: inline-block;} \
      a:hover, a:active { background-color: #00FF00;} \
      input[type=button], input[type=submit], input[type=reset] { \
      background-color: #FF0000;  border: none;  color: white;  padding: 5px 15px; \
      text-align: center; text-decoration: none;  display: inline-block; font-size: 1em;} \
  </style> \
  <body> \
  <h2>Wordclock v1.0</h2> \
  ";
  return html;
}

//*******************************************
void reboot() {
  String html = Headder_str();
  html += " \
  <hr> \
  <h3>reboot confirmation</h3> \
  <p>Are you sure to reboot?</p> \
  <a href='/_reboot'>YES</a> <a href='/'>no</a> \
  <hr> \
  </body> \
  </html> \
  ";
  webServer.send(200, "text/html", html);
}

void _reboot() {
  // reboot ESP32
  Serial.println("reboot esp32 now.");

  clearMask();
  applyMask(); 

  delay(1000); // hold 1 sec

  // restart ESP32
  ESP.restart(); 
}

//*******************************************
void resetparm() {
  String html = Headder_str();
  html += " \
  <hr> \
  <h3>Reset settings</h3> \
  <p>Are you sure to reset parameters to the default value?</p> \
  <a href='/_resetparm'>YES</a> <a href='/'>no</a> \
  <hr> \
  </body></html> \
  ";
  webServer.send(200, "text/html", html);
}

void _resetparm() {

  InitializeValue();
  WriteConfigfile();

  // 「/」に転送
  webServer.sendHeader("Location", String("/"), true);
  webServer.send(302, "text/plain", "");

}

//*******************************************
void configwifi() {
  int ssid_num = WiFi.scanNetworks();
  if (ssid_num > SSIDMAX) ssid_num = SSIDMAX;
  String html = Headder_str();
  html += " \
  <a href='/'>TOP</a> \
  <hr> \
  <h3>WiFi Settings</h3> \
  <p><form action='/setwifi' method='get'> \
    SSID:<br> \
    <select name='ssid1' id='ssid_id'>";
    for(int i=0; i<ssid_num; i++) {
      html += "<option value=" + WiFi.SSID(i) +">" + WiFi.SSID(i) + "</option>\r\n";
    }
    html +=" \
    </select><br> \
    Password:<br><input type='password' name='passwd1'><br> \
    <p><input type='submit' value='Save'></p> \
  </form><p> \
  <p><span style='font-size:80%;'>(Current SSID: %ssid% )</span><br> \
  <span style='font-size:80%;'>(password: %passwd% )</span></p> \
  <hr><a href='/'>Cancel</a> \
  <script> \
    document.getElementById('ssid_id').value = '%ssid%'; \
  </script> \
  </body></html> \
  ";
  html.replace("%ssid%",ssid);
  html.replace("%passwd%",maskpasswd(passwd));
  webServer.send(200, "text/html", html);
}

void setwifi(){
  ssid = webServer.arg("ssid1");
  passwd = webServer.arg("passwd1");

  WriteConfigfile();
  
  // 「/」に転送
  webServer.sendHeader("Location", String("/"), true);
  webServer.send(302, "text/plain", "");
}

//*******************************************
void configparm () {
  String html = Headder_str();
  html += " \
  <a href='/'>TOP</a> \
  <hr> \
  <h3>Parameter Settings</h3> \
  <p><form action='/setparm' method='get'> \
    <b>Color mode:</b>  \
    <select name='colmode' id='col_id'> \
      <option value='Rainbow'>Rainbow</option> \
      <option value='RGB'>RGB</option> \
    </select><br> \
    Rainbow interval (msec): <input type='number' name='ival' size='2' min='0' max='500' step='1' value='%interval%'><br> \
    RGB: (<input type='number' name='cstr' size='2' min='0' max='255' step='1' value='%colorR%'>, \
    <input type='number' name='cstg' size='2' min='0' max='255' step='1' id='cstg' value='%colorG%'>, \
    <input type='number' name='cstb' size='2' min='0' max='255' step='1' id='cstb' value='%colorB%'>)<br> \
    <b>Timezone:</b> \
    <select name='timez' id='timez_id'> \
      <option value='-12'>(GMT-12:00)</option> \
      <option value='-11'>(GMT-11:00)</option> \
      <option value='-10'>(GMT-10:00)</option> \
      <option value='-9'>(GMT-09:00)</option> \
      <option value='-8'>(GMT-08:00)</option> \
      <option value='-7'>(GMT-07:00)</option> \
      <option value='-6'>(GMT-06:00)</option> \
      <option value='-5'>(GMT-05:00)</option> \
      <option value='-4'>(GMT-04:00)</option> \
      <option value='-3.5'>(GMT-03:30)</option> \
      <option value='-3'>(GMT-03:00)</option> \
      <option value='-2'>(GMT-02:00)</option> \
      <option value='-1'>(GMT-01:00)</option> \
      <option value='0'>(GMT+00:00)</option> \
      <option value='1'>(GMT+01:00)</option> \
      <option value='2'>(GMT+02:00)</option> \
      <option value='3'>(GMT+03:00)</option> \
      <option value='3.5'>(GMT+03:30)</option> \
      <option value='4'>(GMT+04:00)</option> \
      <option value='4.5'>(GMT+04:30)</option> \
      <option value='5'>(GMT+05:00)</option> \
      <option value='5.5'>(GMT+05:30) </option> \
      <option value='5.75'>(GMT+05:45)</option> \
      <option value='6'>(GMT+06:00)</option> \
      <option value='6.5'>(GMT+06:30)</option> \
      <option value='7'>(GMT+07:00)</option> \
      <option value='8'>(GMT+08:00)</option> \
      <option value='9'>(GMT+09:00) Japan</option> \
      <option value='9.5'>(GMT+09:30)</option> \
      <option value='10'>(GMT+10:00)</option> \
      <option value='11'>(GMT+11:00)</option> \
      <option value='12'>(GMT+12:00)</option> \
      <option value='13'>(GMT+13:00)</option> \
    </select><br> \
    <b>LED Brightness:</b><br> \
    Daybrightness(0-255): <input type='number' name='dayb' min='0' max='255' step='1' size='2' value='%daybrightness%'><br> \
    Nightbrightness(0-255): <input type='number' name='nightb' min='0' max='255' step='1' size='2' value='%nightbrightness%'><br> \
    Day start(0-23): <input type='number' name='moff' min='0' max='23' step='1' size='2' value='%morningcutoff%'><br> \
    Night start(0-23): <input type='number' name='noff' min='0' max='23' step='1' size='2' value='%nightcutoff%'><br> \
    <b>Show AM/PM:</b>  \
    <select name='ampm' id='ampm_id'> \
      <option value='enable'>enable</option> \
      <option value='disable'>disable</option> \
    </select><br> \
    <b>Show Seconds:</b>  \
    <select name='sec' id='sec_id'> \
      <option value='enable'>enable</option> \
      <option value='disable'>disable</option> \
    </select><br> \
    <b>Show Minutes:</b>  \
    <select name='min' id='min_id'> \
      <option value='enable'>enable</option> \
      <option value='disable'>disable</option> \
    </select><br> \
    <p><input type='submit' value='Save'></p> \
  </form></p> \
  <hr><a href='/'>Cancel</a> \
  <script> \
    document.getElementById('col_id').value = '%colormode%'; \
    document.getElementById('timez_id').value = '%timezone%'; \
    document.getElementById('ampm_id').value = '%showampm%'; \
    document.getElementById('sec_id').value = '%showseconds%'; \
    document.getElementById('min_id').value = '%showminutes%'; \
  </script> \
  </body></html> \
  ";

  html.replace("%colormode%",colormode );
  html.replace("%interval%",String(interval) );
  html.replace("%timezone%",timezone );
  html.replace("%colorR%",String(colorR) );
  html.replace("%colorG%",String(colorG) );
  html.replace("%colorB%",String(colorB) );
  html.replace("%daybrightness%",String(daybrightness) );
  html.replace("%nightbrightness%",String(nightbrightness) );
  html.replace("%morningcutoff%",String(morningcutoff) );
  html.replace("%nightcutoff%",String(nightcutoff) );
  html.replace("%showampm%",showampm );
  html.replace("%showseconds%",showseconds );
  html.replace("%showminutes%",showminutes );

  webServer.send(200, "text/html", html);
}

void setparm(){
  String numstr;

  timezone = webServer.arg("timez");
  showampm = webServer.arg("ampm"); 
  showseconds = webServer.arg("sec"); 
  showminutes = webServer.arg("min"); 
  colormode = webServer.arg("colmode");
  interval = webServer.arg("ival").toInt();
  colorR = webServer.arg("cstr").toInt();
  colorG = webServer.arg("cstg").toInt();
  colorB = webServer.arg("cstb").toInt();
  daybrightness = webServer.arg("dayb").toInt();
  nightbrightness = webServer.arg("nightb").toInt();
  morningcutoff = webServer.arg("moff").toInt();
  nightcutoff = webServer.arg("noff").toInt();

  WriteConfigfile();

  webServer.sendHeader("Location", String("/"), true);
  webServer.send(302, "text/plain", "");
}
