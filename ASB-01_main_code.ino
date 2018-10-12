#include <LiquidCrystal.h>
LiquidCrystal lcd(11, 12, 5, 4, 3, 2);

//Debug variables
boolean debug = false;
int minMapPot = 40;
int maxMapPot = 970;

//Gameplay Variables
int gameDuration = 0;
int beginCountdown = 10;
int preplant = 0;
int bombtimer = 0;
int owTeam = 0;
int bluPts = 0;
int redPts = 0;
int gamemode = 0;

//Ports VariablesL
const int valSw = A0;
const int pot = A1;
const int keyPin = A3;
const int debugPin = A4;
const int redLed = 9;
const int bluLed = 8;
const int switchA = 6;
const int switchB = 7;
const int greLed = 10;
const int warnOut = 48; //Buzzer
const int hpOut = 49;

void setup() {
  pinMode(redLed, OUTPUT);
  pinMode(bluLed, OUTPUT);
  pinMode(greLed, OUTPUT);
  pinMode(warnOut, OUTPUT);
  pinMode(hpOut, OUTPUT);
  pinMode(switchA, INPUT);
  pinMode(switchB, INPUT);
  pinMode(valSw, INPUT);
  pinMode(keyPin, INPUT);
  pinMode(debugPin, INPUT);
  pinMode(pot, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);

  if(digitalRead(debugPin)==true) debug=true;

  Serial.println("Welcome ! Press Validate to start setting the game up\n");
  if(debug==true) Serial.println("### DEBUG ENABLED ! SPECIAL FEATURES INCLUDED ###");
  lcd.print("Press validate");
  lcd.setCursor(0,1);
  lcd.print("to start");
  digitalWrite(warnOut, HIGH);
  delay(100);
  digitalWrite(warnOut, LOW);
  delay(100);
  digitalWrite(warnOut, HIGH);
  delay(100);
  digitalWrite(warnOut, LOW);
  if(debug==true) lcd.print(" DEBUG=1");
  while(digitalRead(valSw)==false);
  if(debug==false) check(750);
  else {
    digitalWrite(warnOut, HIGH);
    delay(50);
    digitalWrite(warnOut, LOW);
  }
  delay(500);
  while(digitalRead(valSw)==true);

}

void loop() {
  lcd.clear();
  Serial.println("Select your gamemode :\n");
  Serial.println(gamemodeName(gamemode));
  lcd.clear();
  lcd.print("Gamemode :");
  lcd.setCursor(0,1);
  lcd.print(gamemodeName(gamemode));
  gamemode = gamemodeSelection();
  digitalWrite(warnOut, HIGH);
  delay(50);
  digitalWrite(warnOut, LOW);
  while(digitalRead(valSw)==true);

  switch(gamemode) {
    case 0:
      st_kingofthehill();
      break;
    case 1:
      st_bombscenario();
      break;
    case 2:
      st_freetime();
      break;
  }

  lcd.clear();

  digitalWrite(warnOut, HIGH);
  delay(100);
  digitalWrite(warnOut, LOW);

  Serial.println("");
  Serial.println("Will initialize game soon, please disable the switches");
  lcd.print("Will start soon");
  lcd.setCursor(0,1);
  lcd.print("Disable switches");
  delay(500);
  Serial.println("Parameters : ");
  Serial.print("  Gamemode : ");
  Serial.println(gamemodeName(gamemode));
  Serial.print("  Game duration : ");
  Serial.println(secsToDigit(gameDuration));
  Serial.print("  Countdown before start : ");
  Serial.println(secsToDigit(beginCountdown));
  Serial.print("  Pre-plant time : ");
  Serial.println(secsToDigit(preplant));
  Serial.print("  Bomb timer : ");
  Serial.println(secsToDigit(bombtimer));
  Serial.println("");
  digitalWrite(warnOut, HIGH);
  delay(100);
  digitalWrite(warnOut, LOW);
  delay(100);
  digitalWrite(warnOut, HIGH);
  delay(100);
  digitalWrite(warnOut, LOW);
  delay(1000);
  while(countdown(beginCountdown)==false) {
    Serial.println("Please disable switches or remove key !");
    lcd.clear();
    lcd.print("Cntdwn stopped");
    lcd.setCursor(0,1);
    lcd.print("Disable switches");
    delay(250);
  }
  Serial.println("Go !");
  lcd.clear();
  lcd.print("Let's go !");
  digitalWrite(warnOut, HIGH);
  digitalWrite(hpOut, HIGH);
  delay(1000);
  digitalWrite(warnOut, LOW);
  digitalWrite(hpOut, LOW);

  switch(gamemode) {
    default:
      gm_kingofthehill();
      break;
    case 0:
      gm_kingofthehill();
      break;
    case 1:
      gm_bombscenario();
      break;
    case 2:
      gm_freetime();
      break;
  }
  gameDuration = 0;
  beginCountdown = 10;
  preplant = 0;
  bombtimer = 0;
  owTeam = 0;
  bluPts = 0;
  redPts = 0;
  gamemode = 0;
  lcd.clear();
  lcd.print("Setting up new");
  lcd.setCursor(0,1);
  lcd.print("game...");
  digitalWrite(warnOut, HIGH);
  delay(100);
  digitalWrite(warnOut, LOW);
  delay(100);
  digitalWrite(warnOut, HIGH);
  delay(100);
  digitalWrite(warnOut, LOW);
  while(digitalRead(valSw) == true);
  delay(1000);

}

void gm_kingofthehill() {
  for(int i=gameDuration;i>=0;i--)
  {
    boolean swASt = digitalRead(switchA);
    boolean swBSt = digitalRead(switchB);

    if(swASt == swBSt) owTeam = 0;
    else if(swASt == true && swBSt == false) owTeam = 1;
    else if(swASt == false && swBSt == true) owTeam = 2;

    lcd.clear();
    switch(owTeam)
    {
      case 1:
        Serial.println("Current owning team is blue");
        lcd.print("Blue hold");
        bluPts = bluPts + 1;
        digitalWrite(bluLed, HIGH);
        digitalWrite(redLed, LOW);
        digitalWrite(greLed, LOW);
        break;
      case 2:
        lcd.print("Red hold");
        Serial.println("Current owning team is red");
        redPts = redPts + 1;
        digitalWrite(redLed, HIGH);
        digitalWrite(bluLed, LOW);
        digitalWrite(greLed, LOW);
        break;
      default:
        lcd.print("No team");
        Serial.println("No team is currently holding the site");
        digitalWrite(greLed, HIGH);
        digitalWrite(redLed, LOW);
        digitalWrite(bluLed, LOW);
        break;
    }
    lcd.setCursor(11,0);
    lcd.print(secsToDigit(i));
    lcd.setCursor(0,1);
    lcd.print("R: ");
    lcd.print(redPts);
    lcd.setCursor(8,1);
    lcd.print("B: ");
    lcd.print(bluPts);

    Serial.print("B: ");
    Serial.print(bluPts);
    Serial.print(" pts R: ");
    Serial.print(redPts);
    Serial.print(" pts Time : ");
    Serial.println(secsToDigit(i));
    Serial.println("");
    delay(1000);
  }
  endGame(bluPts, redPts, gameDuration);
}

void gm_bombscenario() {
  boolean exploded = true;
  uint8_t square[8] = {0x00,0x3f,0x3f,0x00,0x3f,0x3f,0x00};
  uint8_t arr[8] = {0x01,0x03,0x07,0x0f,0x07,0x03,0x01};
  lcd.createChar(0, square);
  lcd.createChar(1, arr);
  int tokenGoal = 10;
  int plantToken = 0;
  for(int i=preplant; i>=0;i--) {
    if(digitalRead(keyPin)==false) {
      if(digitalRead(valSw)==true ) {
        plantToken++;
      }
      lcd.clear();
      Serial.println(secsToDigit(i));
      lcd.print("Arm bomb");
      lcd.setCursor(9,0);
      lcd.print(secsToDigit(i));
      lcd.setCursor(0,1);
      lcd.print(">");
      for(int i=plantToken;i>0;i--) lcd.write(byte(0));
      lcd.setCursor(11,1);
      lcd.print("<");
      lcd.print(plantToken*10);
      lcd.print("%");
    }
    else {
      lcd.clear();
      Serial.println(secsToDigit(i));
      lcd.print("Arm bomb");
      lcd.setCursor(9,0);
      lcd.print(secsToDigit(i));
      lcd.setCursor(0,1);
      lcd.write(byte(1));
      lcd.print(" Plug in key !");
    }
    delay(1000);
    if(plantToken>=tokenGoal) break;
  }
  if(plantToken<tokenGoal) { //Bomb not planted before time
    endGameBomb(1,0,0,tokenGoal);
  }
  else { //Bomb planted successfully
    Serial.println("Bomb planted ! Explosion in " + secsToDigit(bombtimer));
    int defuseGoal = 10;
    int defuseToken = 0;
    digitalWrite(warnOut, HIGH);
    digitalWrite(hpOut, HIGH);
    delay(1000);
    digitalWrite(warnOut, LOW);
    digitalWrite(hpOut, LOW);
    for(int i=bombtimer;i>=0;i--) {
      if(digitalRead(valSw) == false){
        Serial.println("Explosion in " + secsToDigit(i));
        lcd.clear();
        lcd.print("Bomb planted !");
        lcd.setCursor(0,1);
        lcd.print(secsToDigit(i));
        lcd.setCursor(12,1);
        lcd.print(defuseToken*10);
        lcd.print("%");
      }
      else {
        defuseToken++;
        lcd.clear();
        lcd.print("Defusing   " + secsToDigit(i));
        lcd.setCursor(0,1);
        lcd.print(">");
        for(int i=defuseToken;i>0;i--) lcd.write(byte(0));
        lcd.setCursor(11,1);
        lcd.print("<");
        lcd.print(defuseToken*10);
        lcd.print("%");
        if(defuseToken>=defuseGoal) {
          endGameBomb(1,1,i,0);
          exploded = false;
          break;
        }
      }
      if(i>=10) {
        digitalWrite(warnOut, HIGH);
        digitalWrite(hpOut, HIGH);
        digitalWrite(redLed, HIGH);
        delay(50);
        digitalWrite(warnOut, LOW);
        digitalWrite(hpOut, LOW);
        digitalWrite(redLed, LOW);
        delay(950);
      }
      else if(i<10 and i>=5){
        for(int j = 0;j<2;j++) {
          digitalWrite(warnOut, HIGH);
          digitalWrite(hpOut, HIGH);
          digitalWrite(redLed, HIGH);
          delay(50);
          digitalWrite(warnOut, LOW);
          digitalWrite(hpOut, LOW);
          digitalWrite(redLed, LOW);
          delay(450);
        }
      }
      else if(i<5 and i>=1){
        for(int j = 0;j<4;j++) {
          digitalWrite(warnOut, HIGH);
          digitalWrite(hpOut, HIGH);
          digitalWrite(redLed, HIGH);
          delay(50);
          digitalWrite(warnOut, LOW);
          digitalWrite(hpOut, LOW);
          digitalWrite(redLed, LOW);
          delay(200);
        }
      }
      else {
        for(int j = 0;j<8;j++) {
          digitalWrite(warnOut, HIGH);
          digitalWrite(hpOut, HIGH);
          digitalWrite(redLed, HIGH);
          delay(50);
          digitalWrite(warnOut, LOW);
          digitalWrite(hpOut, LOW);
          digitalWrite(redLed, LOW);
          delay(75);
        }
      }
    }
    //Bomb not defused, bomb exploded
    if(exploded == true) endGameBomb(0,0,0,defuseToken);
  }
}

void gm_freetime () {
  for(int i=gameDuration;i>=0;i--)
  {
    lcd.clear();
    lcd.print("Free Time");
    lcd.setCursor(0,1);
    lcd.print(secsToDigit(i));
    Serial.println(secsToDigit(i));
    delay(1000);
  }
  lcd.clear();
  lcd.print("Game finished");
  lcd.setCursor(0,1);
  lcd.print("Time elapsed");
  digitalWrite(redLed, LOW);
  digitalWrite(greLed, LOW);
  digitalWrite(bluLed, LOW);
  digitalWrite(warnOut, HIGH);
  delay(1000);
  digitalWrite(warnOut, LOW);
  delay(500);
  digitalWrite(warnOut, HIGH);
  delay(1000);
  digitalWrite(warnOut, LOW);
  delay(500);
  digitalWrite(warnOut, HIGH);
  delay(2000);
  digitalWrite(warnOut, LOW);
  while(digitalRead(valSw)==false) delay(1000);
}

void st_kingofthehill() {
  Serial.println("Select the game settings");
  gameDuration = durationSelection();
  digitalWrite(warnOut, HIGH);
  delay(50);
  digitalWrite(warnOut, LOW);
  while(digitalRead(valSw)==true);
  lcd.clear();
  beginCountdown = countdownSelection();
  digitalWrite(warnOut, HIGH);
  delay(50);
  digitalWrite(warnOut, LOW);
  while(digitalRead(valSw)==true);
  lcd.clear();
  delay(200);

  Serial.println("");
  Serial.println("We're all set ! Here's a recap :");
  Serial.print("  Game duration : ");
  Serial.println(secsToDigit(gameDuration));
  Serial.print("  Countdown before start : ");
  Serial.println(secsToDigit(beginCountdown));
  Serial.println("\nPress Validate to start the game !");
  Serial.println("Press Reset to cancel");
  lcd.print("Duration : " + secsToDigit(gameDuration));
  lcd.setCursor(0,1);
  lcd.print("Countdown: " + secsToDigit(beginCountdown));
  while(digitalRead(valSw)==false);
}

void st_bombscenario () {
  Serial.println("Select the game settings");
  preplant = prePlantSelection();
  digitalWrite(warnOut, HIGH);
  delay(50);
  digitalWrite(warnOut, LOW);
  while(digitalRead(valSw)==true);
  lcd.clear();
  bombtimer = bombtimerSelection();
  digitalWrite(warnOut, HIGH);
  delay(50);
  digitalWrite(warnOut, LOW);
  while(digitalRead(valSw)==true);
  lcd.clear();
  beginCountdown = countdownSelection();
  digitalWrite(warnOut, HIGH);
  delay(50);
  digitalWrite(warnOut, LOW);
  while(digitalRead(valSw)==true);
  lcd.clear();
  delay(200);

  Serial.println("");
  Serial.println("We're all set ! Here's a recap :");
  Serial.print("  Pre-plant time : ");
  Serial.println(secsToDigit(preplant));
  Serial.print("  Bomb timer : ");
  Serial.println(secsToDigit(bombtimer));
  Serial.print("  Countdown before start : ");
  Serial.println(secsToDigit(beginCountdown));
  Serial.println("\nPress Validate to start the game !");
  Serial.println("Press Reset to cancel");
  lcd.print("Pre-plant: " + secsToDigit(preplant));
  lcd.setCursor(0,1);
  lcd.print("Bomb timer:" + secsToDigit(bombtimer));
  while(digitalRead(valSw)==false);
  digitalWrite(warnOut, HIGH);
  delay(50);
  digitalWrite(warnOut, LOW);
  lcd.clear();
  lcd.print("Bomb timer:" + secsToDigit(bombtimer));
  lcd.setCursor(0,1);
  lcd.print("Countdown: " + secsToDigit(beginCountdown));
  delay(200);
  while(digitalRead(valSw)==true);
  while(digitalRead(valSw)==false);
}

void st_freetime () {
  Serial.println("Select the game settings");
  gameDuration = durationSelection();
  digitalWrite(warnOut, HIGH);
  delay(50);
  digitalWrite(warnOut, LOW);
  while(digitalRead(valSw)==true);
  lcd.clear();
  beginCountdown = countdownSelection();
  digitalWrite(warnOut, HIGH);
  delay(50);
  digitalWrite(warnOut, LOW);
  while(digitalRead(valSw)==true);
  lcd.clear();
  delay(200);

  Serial.println("");
  Serial.println("We're all set ! Here's a recap :");
  Serial.print("  Game duration : ");
  Serial.println(secsToDigit(gameDuration));
  Serial.print("  Countdown before start : ");
  Serial.println(secsToDigit(beginCountdown));
  Serial.println("\nPress Validate to start the game !");
  Serial.println("Press Reset to cancel");
  lcd.print("Duration : " + secsToDigit(gameDuration));
  lcd.setCursor(0,1);
  lcd.print("Countdown: " + secsToDigit(beginCountdown));
  while(digitalRead(valSw)==false);
}

void check(int delays) {
  //Executed at launch to test each component
  Serial.println("Executing components check...");
  lcd.clear();
  lcd.print("Checking comp");

  Serial.println("CHECK : WARNING OUT");
  digitalWrite(warnOut, HIGH);
  delay(100);
  digitalWrite(warnOut, LOW);
  delay(100);
  digitalWrite(warnOut, HIGH);
  delay(100);
  digitalWrite(warnOut, LOW);
  Serial.println("CHECK : RED LED");
  digitalWrite(redLed, HIGH);
  lcd.print(".");
  delay(delays);
  digitalWrite(redLed, LOW);
  Serial.println("CHECK : GREEN LED");
  digitalWrite(greLed, HIGH);
  lcd.print(".");
  delay(delays);
  digitalWrite(greLed, LOW);
  Serial.println("CHECK : BLUE LED");
  digitalWrite(bluLed, HIGH);
  lcd.print(".");
  delay(delays);
  digitalWrite(bluLed, LOW);
  Serial.println("CHECK : SPEAKERS OUT");
  digitalWrite(hpOut, HIGH);
  lcd.print(".");
  delay(delays);
  digitalWrite(hpOut, LOW);
  Serial.println("Components check complete ! Ready to go !\n");
  lcd.clear();
  lcd.print("Check complete");
  lcd.setCursor(0,1);
  lcd.print("Let's go !");
  delay(1000);
}

void endGame(int bluPts, int redPts, int duration) {
  int team = 0;
  lcd.clear();
  lcd.print("Game finished");
  lcd.setCursor(0,1);
  Serial.println("");
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  Serial.println("      Game finished !");
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  if(bluPts > redPts) { //BLUE WINS
    lcd.print("Blue Team wins !");
    Serial.print("Blue Team wins the game with ");
    Serial.print(bluPts);
    Serial.println(" points !");
    Serial.print("Red Team scored ");
    Serial.print(redPts);
    Serial.println(" points.");
    team = 1;
  }
  else if(bluPts < redPts) { //RED WINS
    lcd.print("Red Team wins !");
    Serial.print("Red Team wins the game with ");
    Serial.print(redPts);
    Serial.println(" points !");
    Serial.print("Blue Team scored ");
    Serial.print(bluPts);
    Serial.println(" points.");
    team = 2;
  }
  else { //NONE WINS
    lcd.print("Nobody wins");
    Serial.println("Game drawn, no team won.");
    Serial.print("Red Team scored ");
    Serial.print(redPts);
    Serial.println(" points.");
    Serial.print("Blue Team scored ");
    Serial.print(bluPts);
    Serial.println(" points.");
  }
  int percentBlu = (bluPts*100)/duration;
  int percentRed = (redPts*100)/duration;
  int percentIdle = 100 - percentBlu - percentRed;

  Serial.print("The game lasted ");
  Serial.print(secsToDigit(duration));
  Serial.println(" minutes.");

  Serial.println("\nPercentages :");
  Serial.print("Blue Team : ");
  Serial.print(percentBlu);
  Serial.println("%");

  Serial.print("Red Team : ");
  Serial.print(percentRed);
  Serial.println("%");

  Serial.print("Idle time : ");
  Serial.print(percentIdle);
  Serial.println("%");

  Serial.println("\nReset the box to play again.");
  digitalWrite(redLed, LOW);
  digitalWrite(greLed, LOW);
  digitalWrite(bluLed, LOW);
  digitalWrite(warnOut, HIGH);
  digitalWrite(hpOut, HIGH);
  delay(1000);
  digitalWrite(warnOut, LOW);
  digitalWrite(hpOut, LOW);
  delay(500);
  digitalWrite(warnOut, HIGH);
  digitalWrite(hpOut, HIGH);
  delay(1000);
  digitalWrite(warnOut, LOW);
  digitalWrite(hpOut, LOW);
  delay(500);
  digitalWrite(warnOut, HIGH);
  digitalWrite(hpOut, HIGH);
  delay(2000);
  digitalWrite(warnOut, LOW);
  digitalWrite(hpOut, LOW);
  while(digitalRead(valSw)==false) {
    switch(team) {
      case 1:
        digitalWrite(bluLed, HIGH);
        lcd.clear();
        lcd.print("Game finished");
        lcd.setCursor(0,1);
        lcd.print("Blue Team wins !");
        delay(2000);
        digitalWrite(bluLed, LOW);
        lcd.clear();
        lcd.print("Hold Validate");
        lcd.setCursor(0,1);
        lcd.print("to play again");
        break;
      case 2:
        digitalWrite(redLed, HIGH);
        lcd.clear();
        lcd.print("Game finished");
        lcd.setCursor(0,1);
        lcd.print("Red Team wins !");
        delay(2000);
        digitalWrite(redLed, LOW);
        lcd.clear();
        lcd.print("Hold Validate");
        lcd.setCursor(0,1);
        lcd.print("to play again");
        break;
      default:
        digitalWrite(greLed, HIGH);
        lcd.clear();
        lcd.print("Game finished");
        lcd.setCursor(0,1);
        lcd.print("Nobody wins");
        delay(2000);
        digitalWrite(greLed, LOW);
        lcd.clear();
        lcd.print("Hold Validate");
        lcd.setCursor(0,1);
        lcd.print("to play again");
        break;
    }
    delay(2000);
  }
}

void endGameBomb(int team, int winKey, int tim, int token) {
  lcd.clear();
  lcd.print("Game finished");
  digitalWrite(warnOut, HIGH);
  digitalWrite(hpOut, HIGH);
  delay(1000);
  digitalWrite(warnOut, LOW);
  digitalWrite(hpOut, LOW);
  delay(500);
  digitalWrite(warnOut, HIGH);
  digitalWrite(hpOut, HIGH);
  delay(1000);
  digitalWrite(warnOut, LOW);
  digitalWrite(hpOut, LOW);
  delay(500);
  digitalWrite(warnOut, HIGH);
  digitalWrite(hpOut, HIGH);
  delay(2000);
  digitalWrite(warnOut, LOW);
  digitalWrite(hpOut, LOW);
  Serial.println("");
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  Serial.println("      Game finished !");
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  if(team == 1 && winKey == 0){
    Serial.println("Defenders win !");
    Serial.println("Attackers did not plant the bomb");
    Serial.println("Time remaining : " + secsToDigit(tim));
    Serial.println("Percentage planted : " + token);
    Serial.println("The game lasted : " + secsToDigit(gameDuration));
  }
  if(team == 1 && winKey == 1){
    Serial.println("Defenders win !");
    Serial.println("Attackers defused the bomb !");
    Serial.println("Time remaining : " + secsToDigit(tim));
    Serial.println("The game lasted : " + secsToDigit(gameDuration));
  }
  if(team == 0){
    Serial.println("Attackers win !");
    Serial.println("The bomb has exloded !");
    Serial.println("Percentage defused: " + token);
    Serial.println("The game lasted : " + secsToDigit(gameDuration));
  }
  while(1) {
    if(team == 1 && winKey == 0){
      digitalWrite(bluLed, HIGH);
      lcd.clear();
      lcd.print("Defenders win !");
      lcd.setCursor(0,1);
      lcd.print("Bomb not planted");
      delay(2000);
      digitalWrite(bluLed, LOW);
      lcd.clear();
      lcd.print("Hold Validate");
      lcd.setCursor(0,1);
      lcd.print("to play again");
      if(digitalRead(valSw) == true) break;
    }
    if(team == 1 && winKey == 1){
      digitalWrite(bluLed, HIGH);
      lcd.clear();
      lcd.print("Defenders win !");
      lcd.setCursor(0,1);
      lcd.print("Bomb defused !");
      delay(2000);
      digitalWrite(bluLed, LOW);
      lcd.clear();
      lcd.print("Hold Validate");
      lcd.setCursor(0,1);
      lcd.print("to play again");
      if(digitalRead(valSw) == true) break;
    }
    if(team == 0){
      digitalWrite(redLed, HIGH);
      lcd.clear();
      lcd.print("Attackers win !");
      lcd.setCursor(0,1);
      lcd.print("Bomb exploded !");
      delay(2000);
      digitalWrite(redLed, LOW);
      lcd.clear();
      lcd.print("Hold Validate");
      lcd.setCursor(0,1);
      lcd.print("to play again");
      if(digitalRead(valSw) == true) break;
    }
    delay(2000);
  }
}

boolean countdown(int cd) {
  for(int i= cd; i>0; i--)
  {
    if(digitalRead(switchA)==false && digitalRead(switchB)==false) {
      lcd.clear();
      lcd.print("The game begins");
      lcd.setCursor(0,1);
      lcd.print("in " + secsToDigit(i));
      Serial.print("The game begins in ");
      Serial.println(secsToDigit(i));
      digitalWrite(warnOut, HIGH);
      digitalWrite(hpOut, HIGH);
      delay(50);
      digitalWrite(warnOut, LOW);
      digitalWrite(hpOut, LOW);
      delay(950);
    }
    else {
      return(false);
      break;
    }

  }
  return(true);
}

int durationSelection() {
  int duration = 900;
  int previousDuration = 0;
  while(digitalRead(valSw)==false) {
    int potVal = analogRead(pot);
    int mapVal = 0;
    if(debug==true) mapVal = map(potVal,minMapPot,maxMapPot,-1,6);
    else mapVal = map(potVal,minMapPot,maxMapPot,0,6);
    switch(mapVal) {
      case 0:
        duration = 900;
        break;
      case 1:
        duration = 1200;
        break;
      case 2:
        duration = 1800;
        break;
      case 3:
        duration = 2700;
        break;
      case 4:
        duration = 3600;
        break;
      case 5:
        duration = 5400;
        break;
      case 6:
        duration = 7200;
        break;
      default:
        duration = 5;
        break;
    }
    if(duration<previousDuration || duration>previousDuration) {
      Serial.print("Game length : ");
      lcd.clear();
      lcd.print("Game length :");
      lcd.setCursor(0,1);
      lcd.print(secsToDigit(duration));
      Serial.println(secsToDigit(duration));
      previousDuration = duration;
    }
    delay(100);
  }
  return(duration);
}

int gamemodeSelection() {
  int gamemode = 0;
  int previousGamemode = 0;
  while(digitalRead(valSw)==false) {
    int potVal = analogRead(pot);
    int mapVal = map(potVal,minMapPot,maxMapPot,0,2);
    switch(mapVal) {
      case 0:
        gamemode = 0;
        break;
      case 1:
        gamemode = 1;
        break;
      case 2:
        gamemode = 2;
        break;
      default:
        gamemode = 0;
        break;
    }
    if(gamemode<previousGamemode || gamemode>previousGamemode) {
      Serial.print("Gamemode : ");
      lcd.clear();
      lcd.print("Gamemode :");
      lcd.setCursor(0,1);
      lcd.print(gamemodeName(gamemode));
      Serial.println(gamemodeName(gamemode));
      previousGamemode = gamemode;
    }
    delay(100);
  }
  return(gamemode);
}

String gamemodeName(int gm) {
  String gmName = "None";
  switch(gm) {
    default:
      gmName = "King of the Hill";
      break;
    case 0:
      gmName = "King of the Hill";
      break;
    case 1:
      gmName = "Bomb Scenario";
      break;
    case 2:
      gmName = "Free Time";
      break;
  }
  return(gmName);
}

int countdownSelection() {
  int countdown = 25;
  int previousCountdown = 0;
  while(digitalRead(valSw)==false) {
    int potVal = analogRead(pot);
    int mapVal = 0;
    if(debug==true) mapVal = map(potVal,minMapPot,maxMapPot,-1,5);
    else mapVal = map(potVal,minMapPot,maxMapPot,0,5);
    switch(mapVal) {
      case 0:
        countdown = 25;
        break;
      case 1:
        countdown = 40;
        break;
      case 2:
        countdown = 60;
        break;
      case 3:
        countdown = 120;
        break;
      case 4:
        countdown = 180;
        break;
      case 5:
        countdown = 240;
        break;
      default: //For debug and dev uses
        countdown = 5;
        break;
    }
    if(countdown<previousCountdown || countdown>previousCountdown) {
      Serial.print("Countdown duration before game start : ");
      Serial.println(secsToDigit(countdown));
      lcd.clear();
      lcd.print("Countdown :");
      lcd.setCursor(0,1);
      lcd.print(secsToDigit(countdown));
      previousCountdown = countdown;
    }
    delay(100);
  }
  return(countdown);
}

int prePlantSelection() {
  int preplant = 25;
  int previousPreplant = 0;
  while(digitalRead(valSw)==false) {
    int potVal = analogRead(pot);
    int mapVal = 0;
    if(debug==true) mapVal = map(potVal,minMapPot,maxMapPot,-1,3);
    else mapVal = map(potVal,minMapPot,maxMapPot,0,3);
    switch(mapVal) {
      case 0:
        preplant = 300;
        break;
      case 1:
        preplant = 480;
        break;
      case 2:
        preplant = 720;
        break;
      case 3:
        preplant = 960;
        break;
      default: //For debug and dev uses
        preplant = 20;
        break;
    }
    if(preplant<previousPreplant || preplant>previousPreplant) {
      Serial.print("Pre-plant time : ");
      Serial.println(secsToDigit(preplant));
      lcd.clear();
      lcd.print("Pre-plant time :");
      lcd.setCursor(0,1);
      lcd.print(secsToDigit(preplant));
      previousPreplant = preplant;
    }
    delay(100);
  }
  return(preplant);
}

int bombtimerSelection() {
  int bombtimer = 25;
  int previousBombtimer = 0;
  while(digitalRead(valSw)==false) {
    int potVal = analogRead(pot);
    int mapVal = 0;
    if(debug==true) mapVal = map(potVal,minMapPot,maxMapPot,-1,3);
    else mapVal = map(potVal,minMapPot,maxMapPot,0,3);
    switch(mapVal) {
      case 0:
        bombtimer = 180;
        break;
      case 1:
        bombtimer = 300;
        break;
      case 2:
        bombtimer = 480;
        break;
      case 3:
        bombtimer = 600;
        break;
      default: //For debug and dev uses
        bombtimer = 20;
        break;
    }
    if(bombtimer<previousBombtimer || bombtimer>previousBombtimer) {
      Serial.print("Bomb timer : ");
      Serial.println(secsToDigit(bombtimer));
      lcd.clear();
      lcd.print("Bomb timer :");
      lcd.setCursor(0,1);
      lcd.print(secsToDigit(bombtimer));
      previousBombtimer = bombtimer;
    }
    delay(100);
  }
  return(bombtimer);
}

String secsToDigit(int t) {
  float flTime = t/60;
  int minutes = flTime;
  int seconds = t-60*minutes;
  String mins;
  String secs;
  if(minutes<10) mins = "0" + String(minutes);
  else mins = String(minutes);
  if(seconds<10) secs = "0" + String(seconds);
  else secs = String(seconds);

  String finalTime = mins + ":" + secs;
  return(finalTime);
}
