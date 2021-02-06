
// Code for meals reminder alarm clock
// Phyoe Thu
#include <LiquidCrystal.h>
#include <Wire.h>
#include <RTClib.h>
#include <EEPROM.h>

RTC_DS3231 rtc;

const int rs = 12, en = 11, d4 = 6, d5 = 5, d6 = 4, d7 = 3; // LCD pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int buzz = 13; // Buzzer
int addr = 17;

const int buttonPin1 = A0;
const int buttonPin2 = 9; // Breakfast
const int buttonPin3 = 8; // Lunch
const int buttonPin4 = 7; // Dininer

int val2 = 0;
int val3 = 0;
int pushVal;

int bS1 = 0; // Current state of the button
int lBS1 = 0; // Previous state of the button

int bS2 = 0; // Current state of the button
int lBS2 = 0; // Previous state of the button

int bS3 = 0; // Current state of the button
int lBS3 = 0; // Previous state of the button

int bS4 = 0; // Current state of the button
int lBS4 = 0; // Previous state of the button  

// Set the alarms reminder in 24 hours format (up to 3 alarms)
int buzz8amHH = 8;          //    HH - hours         
int buzz8amMM = 00;          //    MM - Minute
int buzz8amSS = 00;          //    SS - Seconds

int buzz1pmHH = 13;          //    HH - hours
int buzz1pmMM = 00;          //    MM - Minute
int buzz1pmSS = 00;          //    SS - Seconds

int buzz6pmHH = 18;          //    HH - hours
int buzz6pmMM = 00;          //    MM - Minute
int buzz6pmSS = 00;          //    SS - Seconds

int nowHr, nowMin, nowSec; 

void introMessage() { // Display intro message (first message)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome To Our");                            
  lcd.setCursor(0, 1);
  lcd.print("Meals Reminder!");
}

void shortMessage() { // Display short message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("All You Need Is"); 
  lcd.setCursor(0, 1);
  lcd.print("Food...");
}

void helpScreen() { // Display 1st screen in LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press Buttons");
  lcd.setCursor(0, 1);
  lcd.print("for Reminder...!");
 }

void displayOne() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press Button 1");
  lcd.setCursor(0, 1);
  lcd.print("for Breakfast!");
}

void displayTwo() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press Button 2");
  lcd.setCursor(0, 1);
  lcd.print("for Lunch!");
}

void displayThree() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press Button 3");
  lcd.setCursor(0, 1);
  lcd.print("for Dinner!");
}

void timeScreen() { // Display time and date on LCD
  DateTime now = rtc.now(); 

  // Display time
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time:");
  lcd.setCursor(6, 0);
  lcd.print(nowHr = now.hour(), DEC);
  lcd.print(":");
  lcd.print(nowMin = now.minute(), DEC);
  lcd.print(":");
  lcd.print(nowSec = now.second(), DEC);

  // Display date
  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  lcd.print(now.month(), DEC);
  lcd.print("/");
  lcd.print(now.day(), DEC);
  lcd.print("/");
  lcd.print(now.year(), DEC);
  delay(1000);
}


// ----------------------------------------------------------------------------------
// Start of the program
// ----------------------------------------------------------------------------------
void setup()
{
  Wire.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));           

  lcd.begin(16, 2);
  
  introMessage();
  delay(3000);
  
  shortMessage();
  delay(3000);
  
  helpScreen();
  delay(3000);

  displayOne();
  delay(1500);

  displayTwo();
  delay(1500);

  displayThree();
  delay(1500);
  
  timeScreen();
  delay(1000);
  
  lcd.clear();
  
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  pinMode(buzz, OUTPUT);
  
  Serial.begin(9600);
}

void ValSet(){
  Serial.print("addr ");
  Serial.println(EEPROM.read(addr));
  val2 = EEPROM.read(addr); // Read previosuly saved value of push button to start from where it was left previously
  
  switch (val2) {
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Breakfast");
      lcd.setCursor(0, 1);
      lcd.print("at ");
      lcd.print(buzz8amHH);
      lcd.print(":");
      lcd.print(buzz8amMM);
      lcd.print(":"); 
      lcd.print(buzz8amSS);
      pushVal = 1;
      delay(1000);
      break;
      
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Lunch reminder");
      lcd.setCursor(0, 1);
      lcd.print("at ");
      lcd.print(buzz1pmHH);
      lcd.print(":");
      lcd.print(buzz1pmMM);
      lcd.print(":"); 
      lcd.print(buzz1pmSS);
      pushVal = 2;
      delay(1000);
      break;
      
    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Dinner reminder");
      lcd.setCursor(0, 1);
      lcd.print("at ");
      lcd.print(buzz6pmHH);
      lcd.print(":");
      lcd.print(buzz6pmMM);
      lcd.print(":"); 
      lcd.print(buzz6pmSS);
      pushVal = 3;
      delay(1000);
      break;
  }
}

// Main loop will constantly check to see if the button has been pressed
// If it has been pressed, a counter is incremented, and then some action can be taken
void loop() 
{                                     
  if (pushVal == 1) { // If push button 1 pressed then remind at 8 am
    at8am(); // Function to start buzzing at 8 am 
  }
  
  else if (pushVal == 2) { // If push button 2 pressed then remind at 1 pm
   // at8am();
    at1pm(); // Function to start buzzing at 1 pm
  }
  
  else if (pushVal == 3) { // If push button 3 pressed then remind at 6 pm
   // at8am();
  //  at2pm(); 
    at6pm(); // Function to start buzzing at 6 pm 
  }
  
  // Read the state of the button
  bS1 = digitalRead(buttonPin1);
  bS2 = digitalRead(buttonPin2);
  bS3 = digitalRead(buttonPin3);
  bS4 = digitalRead(buttonPin4);


// Check to see if it different than the last time we checked

if (bS2 != lBS2) {
    // Either the button was just pressed or just released
    if (bS2 == LOW) {
      // It was just pressed
      Serial.println("no2");
      EEPROM.write(17,1);
      push1();
      delay(1000);
    }
  }
  lBS2 = bS2;

if (bS3 != lBS3) {
    // Either the button was just pressed or just released
    if (bS3 == LOW) {
      // It was just pressed
      Serial.println("no3");
      EEPROM.write(17,2);
      push2();
      delay(1000);
    }
  }
  lBS3 = bS3;


if (bS4 != lBS4) {
    // Either the button was just pressed or just released
    if (bS4 == LOW) {
      // It was just pressed
      Serial.println("no4");
      EEPROM.write(17,3);
      push3();
      delay(1000);
    }
  }
  lBS4 = bS4;
  
if (bS1 != lBS1) {
    // Either the button was just pressed or just released
    if (bS1 == LOW) {
      val3=EEPROM.read(addr);
      EEPROM.write(17,0);
      digitalWrite(buzz, LOW);
      pinstop();
      EEPROM.write(17,val3);
    }
  }
  lBS1 = bS1;

  timeScreen();
  ValSet();
}

void push1() { // Function to display breakfast reminder message                           
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reminder set ");
    lcd.setCursor(0, 1);
    lcd.print("for breakfast!");
    delay(3000);
    lcd.clear();
  }

void push2() { // Function to display lunch reminder message
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reminder set ");
    lcd.setCursor(0, 1);
    lcd.print("for lunch!");
    delay(3000);
    lcd.clear();
}

void push3() { // Function to display dinner reminder message
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reminder set ");
    lcd.setCursor(0, 1);
    lcd.print("for dinner!");
    delay(3000);
    lcd.clear();
}

void pinstop() { // Function to stop buzzing when user pushes fourth push button
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enjoy your food!");
    lcd.setCursor(0, 1);
    lcd.print("Have a nice day!");
    delay(5000);
    lcd.clear();
}
  
void at8am() { // Function to start buzzing at 8 am
  DateTime t = rtc.now();
  
  if (int(t.hour()) == buzz8amHH && int(t.minute()) == buzz8amMM  && (int(t.second()) == buzz8amSS || int(t.second()) < buzz8amSS+10)) {
    digitalWrite(buzz,HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Time to eat your");
    lcd.setCursor(0, 1);
    lcd.print("breakfast! :)");
    delay(5000);
  }
}

void at1pm() { // Function to start buzzing at 1 pm
  DateTime t = rtc.now();
  
  if (int(t.hour()) == buzz1pmHH && int(t.minute()) == buzz1pmMM  && (int(t.second()) == buzz1pmSS || int(t.second()) < buzz1pmSS+10)) {
    digitalWrite(buzz,HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Time to eat your");
    lcd.setCursor(0, 1);
    lcd.print("lunch! :)");
    delay(5000);
  }
}

void at6pm() { // Function to start buzzing at 6 pm
  DateTime t = rtc.now();
  if (int(t.hour()) == buzz6pmHH && int(t.minute()) == buzz6pmMM  && (int(t.second()) == buzz6pmSS || int(t.second()) < buzz6pmSS+10)) {
    digitalWrite(buzz,HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Time to eat your");
    lcd.setCursor(0, 1);
    lcd.print("dinner! :)");
    delay(5000);
  }
}
