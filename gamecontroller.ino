#include <LiquidCrystal.h>

#include <Keyboard.h>


//DEFINE PINS
const signed char ACL=3;
const signed char BRK=2;

const signed char REV=4;
const signed char NEU=7;
const signed char FWD=6;

//MAX POTENITIOMETER VALUE
const short MAX=680;

//DEFINE KEYBOARD CHARACTERS
const char ACLI='a';
const char ACLD='d';

const char BRKI=';';
const char BRKD='\'';

const char REVI='w';
const char REVD='s';

const char AWS='q';
const char LOAD='T';
const char HORN=' ';

//DEFINE NUMBER OF STEPS
short ACLN=10;
short BRKN=10;

//LAST STEP VALUES READ
short ACLL=0;
short BRKL=0;

//CURRENT STEP VALUES
short ACLC=0;
short BRKC=0;

//CURRENT REVERSER POSITION
short REVP=0;

//SHOW DEBUG OUTPUT
bool DEBUG=false;

LiquidCrystal lcd(13,12,8,9,10,11);


void setup() {
  if(DEBUG) {
    Serial.begin(9600);
    Serial.println("Starting to print metrics");
  }
  pinMode(REV, INPUT);
  pinMode(NEU, INPUT);
  pinMode(FWD, INPUT);

// set up the LCD's number of columns and rows:
   lcd.begin(20,4);
   // Print a message to the LCD.
   lcd.print("hello, world!");
  Keyboard.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  ACLC=analogRead(ACL);
  BRKC=analogRead(BRK);
  short SACL=steps(ACLC, ACLN, MAX);
  short SBRK=steps(BRKC, BRKN, MAX);
  short REVN=digitalRead(REV);
  short NEUN=digitalRead(NEU);
  short FWDN=digitalRead(FWD);

  short NREVP=findRevPos(REVN,NEUN,FWDN,REVP);
  if(DEBUG){
    printALine(
      ACLC,
      SACL-ACLL,
      BRKC,
      SBRK-BRKL,
      REVN,
      NEUN,
      FWDN,
      NREVP-REVP
     );
  }
  display(SACL, SBRK, NREVP);
  sendAllKeys(SACL-ACLL, SBRK-BRKL, NREVP-REVP);

  
  ACLL=SACL;
  BRKL=SBRK;
  REVP=NREVP;
  delay(1000);
}

void sendAllKeys(short acls, short brks, short revps) {
  sendKeys(ACLI, ACLD, acls, 250, 250);
  sendKeys(BRKI, BRKD, brks, 250, 250);
  sendKeys(REVI, REVD, revps, 500, 250);
}

void printALine(short acl, short acls, short brk, short brks, short rev, short neu, short fwd, short revpp){
  if(DEBUG){
    Serial.println("ACL STEP BRK STEP REV NEU FWD, REVP");
    Serial.println("===================================");
    Serial.print(acl);
    Serial.print(" ");
    Serial.print(acls);
    Serial.print(" ");
    Serial.print(brk);
    Serial.print(" ");
    Serial.print(brks);
    Serial.print("   ");
    Serial.print(rev);
    Serial.print("   ");
    Serial.print(neu);
    Serial.print("   ");
    Serial.print(fwd);
    Serial.print("   ");
    Serial.println(revpp);

    lcd.setCursor(0,3);
    lcd.print("ACL");
    lcd.print(acl);
    lcd.print("-");
    lcd.print(acls);
    lcd.print("BRK");
    lcd.print(brk);
    lcd.print("-");
    lcd.print(brks);
    lcd.print("REV");
    if(rev) lcd.print("F");
    else if(neu) lcd.print("N");
    else if(rev) lcd.print("R");
    else lcd.print("-");

  }
}

void sendKeys(char pos, char neg, short num, short keypr, short del) {
  boolean isNeg = num < 0;
  if(isNeg) num = num * -1;
  for(short i = 0; i < num; i ++) {
      if(isNeg)
        sendKey(neg, keypr);
      else
        sendKey(pos, keypr);
      delay(del);
  }
}

void sendKey(char key, short del){
  if(DEBUG){
    Serial.print("Sending ");
    Serial.println(key);
  }
  Keyboard.press(key);
  delay(del);
  Keyboard.releaseAll();
}

void display(short acl, short brk, short rev){

  lcd.clear();
  char b = 'a' + 158;
 
  lcd.setCursor(0,0);
  lcd.print("ACL ");
  for(short i=0; i<acl; i++) lcd.print(b);
  lcd.setCursor(0,1);
  lcd.print("BRK ");
  for(short i=0; i<brk; i++) lcd.print(b);
  lcd.setCursor(0,2);
  lcd.print("Reverser ");
  if(rev == 0) lcd.print("Off");
  else if(rev == 1) lcd.print("Reverse");
  else if(rev == 2) lcd.print("Neutral");
  else if(rev == 3) lcd.print("Forward");
 
}

short findRevPos(boolean rev, boolean neu, boolean fwd, short last){
  if(rev) return 1;
  if(neu) return 2;
  if(fwd) return 3;
  return 0;
}

inline signed short steps(short current, short steps, short range){
    signed short diff = current / (range / steps);
    return diff ; 
}


