#include <Keyboard.h>

const signed char ACL=2;
const signed char BRK=3;

const signed char REV=4;
const signed char NEU=7;
const signed char FWD=6;

const short MAX=680;

const char ACLI='a';
const char ACLD='d';

const char BRKI=';';
const char BRKD='\'';

const char REVI='w';
const char REVD='s';

short ACLN=10;
short BRKN=10;

short ACLL=0;
short BRKL=0;

short ACLC=0;
short BRKC=0;

short REVP=0;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting to print metrics");

  pinMode(REV, INPUT);
  pinMode(NEU, INPUT);
  pinMode(FWD, INPUT);

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
//  printALine(
//    ACLC,
//    SACL-ACLL,
//    BRKC,
//    SBRK-BRKL,
//    REVN,
//    NEUN,
//    FWDN,
//    NREVP-REVP
//   );

  sendAllKeys(SACL-ACLL, SBRK-BRKL, NREVP-REVP);
  
  ACLL=SACL;
  BRKL=SBRK;
  REVP=NREVP;
  delay(1000);
}

void sendAllKeys(short acls, short brks, short revps) {
  sendKeys(ACLI, ACLD, acls);
  sendKeys(BRKI, BRKD, brks);
  sendKeys(REVI, REVD, revps);
}

void printALine(short acl, short acls, short brk, short brks, short rev, short neu, short fwd, short revpp){
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
  
  
}
void sendKeys(char pos, char neg, short num) {
  boolean isNeg = num < 0;
  if(isNeg) num = num * -1;
  for(short i = 0; i < num; i ++) {
      if(isNeg)
        sendKey(neg);
      else
        sendKey(pos);
      delay(250);
  }
}

void sendKey(char key){
  Serial.print("Sending ");
  Serial.println(key);
  Keyboard.press(key);
  delay(250);
  Keyboard.releaseAll();
}

short findRevPos(boolean rev, boolean neu, boolean fwd, short last){
  if(rev) return 1;
  if(neu) return 2;
  if(fwd) return 3;
  return 0;
}

signed short steps(short current, short steps, short range){
    signed short diff = current / (range / steps);
    return diff ; 
}


