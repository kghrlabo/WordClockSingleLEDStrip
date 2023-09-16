//    WORD CLOCK - for ESP32

void displayTime(int hour,int minute,int sec) {

  setMask(word_ITIS); // IT IS;

  int leftover = minute % 5;
  minute = minute - leftover;

  if (minute >= 35) {
    hour = (hour + 1) % 24; // Switch to "TO" minutes the next hour
  }

  switch(minute) {
    // time we display the appropriate minute counter
    case 0: 
      setMask(word_OCLOCK);
      break; 
    case 5:
      setMask(word_FIVE1);
      setMask(word_PAST);
      break;
    case 10:
      setMask(word_TEN1);
      setMask(word_PAST);
      break;
    case 15:
      setMask(word_AQUARTER);
      setMask(word_PAST);
       break;
    case 20:
      setMask(word_TWENTY);
      setMask(word_PAST);
      break;
    case 25:
      setMask(word_TWENTY);
      setMask(word_FIVE1);
      setMask(word_PAST);
      break;
    case 30:
      setMask(word_HALF);
      setMask(word_PAST);
      break;
    case 35:
      setMask(word_TWENTY);
      setMask(word_FIVE1);
      setMask(word_TO);
      break;
    case 40:
      setMask(word_TWENTY);
      setMask(word_TO);
      break;
    case 45:
      setMask(word_AQUARTER);
      setMask(word_TO);
      break;
    case 50:
      setMask(word_TEN1);
      setMask(word_TO);
      break;
    case 55:
      setMask(word_FIVE1);
      setMask(word_TO);
      break;
    default:
      setMask(word_ERR);
  }

  switch (hour) {
    case 0:
    case 12:
      setMask(word_TWELVE);
      break;
    case 1:
    case 13:
      setMask(word_ONE);
      break;
    case 2:
    case 14:
      setMask(word_TWO);
      break;
    case 3:
    case 15:
      setMask(word_THREE);
      break;
    case 4:
    case 16:
      setMask(word_FOUR);
      break;
    case 5:
    case 17:
      setMask(word_FIVE2);
      break;
    case 6:
    case 18:
      setMask(word_SIX);
      break;
    case 7:
    case 19:
      setMask(word_SEVEN);
      break;
    case 8:
    case 20:
      setMask(word_EIGHT);
      break;
    case 9:
    case 21:
      setMask(word_NINE);
      break;
    case 10:
    case 22:
      setMask(word_TEN2);
      break;
    case 11:
    case 23:
      setMask(word_ELEVEN);
      break;
   default:
      setMask(word_ERR);
  }

  // show AM or PM
  if (showampm=="enable") {
    if (hour < 13) setMask(word_AM); // AM;
    else setMask(word_PM); // PM;    
  }

  // show "Seconds"
  if (showseconds=="enable") {
    if (sec % 2) resetMask(word_SECONDS); // second blink;
    else setMask(word_SECONDS); // second blink;
  }

  // show "Minutes"
  if (showminutes=="enable") {
    switch(leftover) {
      case 4:
        setMask(word_MIN4);
      case 3:
        setMask(word_MIN3);
      case 2:
        setMask(word_MIN2);
      case 1:
        setMask(word_MIN1);
      case 0:
        break;
    }
  }

  // apply phrase mask to colorshift function
  applyMask(); 

}
