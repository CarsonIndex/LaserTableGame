#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_IS31FL3731.h>
#include "Adafruit_LEDBackpack.h"
#include<stdio.h>
#include <string.h>

//Defines pin number for respective parts
#define leftPot1 A0
#define leftPot2 A1
#define leftPot3 A2
#define rightPot3 A3
#define rightPot2 A4
#define rightPot1 A5

#define speakerPin 2
#define leftPhotoPin A6
#define rightPhotoPin A7

int leftTeamScore;
int rightTeamScore;

//Declares servo objects
Servo leftRow1;
Servo leftRow2;
Servo leftRow3;
Servo laserServo;
Servo rightRow3;
Servo rightRow2;
Servo rightRow1;

//Declares LED Matrices
Adafruit_IS31FL3731_Wing redMatrix = Adafruit_IS31FL3731_Wing();
Adafruit_IS31FL3731_Wing blueMatrix = Adafruit_IS31FL3731_Wing();

//Declares AlphNum Displays
Adafruit_AlphaNum4 redDisplay = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 blueDisplay = Adafruit_AlphaNum4();

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  //Attaches servo motors to respective pins
  leftRow1.attach(13);
  leftRow2.attach(12);
  leftRow3.attach(11);
  laserServo.attach(10);
  rightRow3.attach(9);
  rightRow2.attach(8);
  rightRow1.attach(7);

  //Declares potentiometer pins as inputs
  pinMode(leftPot1, INPUT);
  pinMode(leftPot2, INPUT);
  pinMode(leftPot3, INPUT);
  pinMode(rightPot3, INPUT);
  pinMode(rightPot2, INPUT);
  pinMode(rightPot1, INPUT);

  //Declares photoresistor and buzzer pins
  pinMode(leftPhotoPin, INPUT);
  pinMode(rightPhotoPin, INPUT);
  pinMode(speakerPin, OUTPUT);

  //Instantiates LED Matrices
  redMatrix.begin(0x77);
  blueMatrix.begin();

  //Instantiates AlphaNum Displays
  redDisplay.begin(0x71);
  setDisplay();
  //Will need to change address after soldering address bridge pin
  blueDisplay.begin(0x71);
  leftTeamScore = 0;
  rightTeamScore = 0;

}

void moveServos(){
  //Reads potentiometer values
  int leftLoc1 = analogRead(leftPot1);
  int leftLoc2 = analogRead(leftPot2);
  int leftLoc3 = analogRead(leftPot3);
  int rightLoc3 = analogRead(rightPot3);
  int rightLoc2 = analogRead(rightPot2);
  int rightLoc1 = analogRead(rightPot1);

  //Maps analog values to degrees
  leftLoc1 = map(leftLoc1, 0, 1023, 0 , 180);
  leftLoc2 = map(leftLoc2, 0, 1023, 0 , 180);
  leftLoc3 = map(leftLoc3, 0, 1023, 0 , 180);
  rightLoc3 = map(rightLoc3, 0, 1023, 0 , 180);
  rightLoc2 = map(rightLoc2, 0, 1023, 0 , 180);
  rightLoc1 = map(rightLoc1, 0, 1023, 0 , 180);

  //Moves Servos to new locations
  leftRow1.write(leftLoc1);
  leftRow2.write(leftLoc2);
  leftRow3.write(leftLoc3);
  rightRow3.write(rightLoc3);
  rightRow2.write(rightLoc2);
  rightRow1.write(rightLoc1);
}

//Moves laser for gameplay
//TODO: Create better algorithm for laser movement
void moveLaser(){
  int laserLoc = rand() % 181;
  laserServo.write(laserLoc);
}

//Checks if light is over threshold
//Will need to change a little bit for actual photoresistors
void checkLight(){
  int leftLevel = analogRead(leftPhotoPin);
  int rightLevel = analogRead(rightPhotoPin);
  Serial.print(leftLevel);
  Serial.print(" ");
  Serial.println(rightLevel);
  if(leftLevel < 511){
    celebrationSequence("left");
  }
  if(rightLevel < 511){
    celebrationSequence("right");
  }
}

//Calls relevant celebration functions
void celebrationSequence(String teamName){
  incrementScore(teamName);
  playSound();
  ledAnimation(teamName);
}

//Increments score on Display
//TODO: Write code to change score on Display.
//Check if Team Score > 10
void incrementScore(String teamName){
  if(teamName == "left")
    {
      //Increases Score Integer for left/red team
      leftTeamScore++;
      //Sets all values of red AlphaNum Display to new values and writes to display
      redDisplay.writeDigitAscii(0, 'P');
      redDisplay.writeDigitAscii(1, '1');
      redDisplay.writeDigitAscii(2, '0');
      redDisplay.writeDigitAscii(3, '0' + leftTeamScore);
      redDisplay.writeDisplay();
      delay(100);
    }
  if(teamName == "right")
    {
      //Increases Score Integer for left/red team
      rightTeamScore++;
      //Sets all values of red AlphaNum Display to new values and writes to display
      blueDisplay.writeDigitAscii(0, 'P');
      blueDisplay.writeDigitAscii(1, '2');
      blueDisplay.writeDigitAscii(2, '0');
      blueDisplay.writeDigitAscii(3, '0' + rightTeamScore);
      blueDisplay.writeDisplay();
      delay(100);

    }
}

void setDisplay(){
  redDisplay.writeDigitAscii(0, 'P');
  redDisplay.writeDigitAscii(1, '1');
  redDisplay.writeDigitAscii(2, '0');
  redDisplay.writeDigitAscii(3, '0' + leftTeamScore);
  redDisplay.writeDisplay();
  blueDisplay.writeDigitAscii(0, 'P');
  blueDisplay.writeDigitAscii(1, '2');
  blueDisplay.writeDigitAscii(2, '0');
  blueDisplay.writeDigitAscii(3, '0' + rightTeamScore);
  blueDisplay.writeDisplay();
  delay(100);


}
//Flashes LEDS
//Write code to make leds write out which team scores
void ledAnimation(String teamName){
  if(teamName == "red")
    {
      //Sets parameters of red LED board
      redMatrix.setTextSize(1);
      redMatrix.setTextWrap(false);
      redMatrix.setTextColor(100);
      //Prints Red Scored and scrolls across Screen
      for (int8_t x=0; x>=-32; x--){
        redMatrix.clear();
        redMatrix.setCursor(x,0);
        redMatrix.print("Red Scored");
        delay(100);
      }
      delay(20);
    }
  else
    {
      //Sets parameters of blue LED board
      blueMatrix.setTextSize(1);
      blueMatrix.setTextWrap(false);
      blueMatrix.setTextColor(100);
      //Prints Blue Scored and scrolls across Screen
      for (int8_t x=0; x>=-32; x--){
        blueMatrix.clear();
        blueMatrix.setCursor(x,0);
        blueMatrix.print("Blue Scored");
        delay(100);
      }
      delay(20);      
    }
}

//Flashes entire led matrices
void flashLights(String teamName){
  if(teamName == "red"){
      //redTeam full matrix flash
      for(int i = 0; i < 3; i++)
        {
          redMatrix.drawRect(0,0, redMatrix.width(), redMatrix.height(), 255);
          redMatrix.fillRect(0,0, redMatrix.width(), redMatrix.height(), 255);
          delay(500);
          redMatrix.clear();
          delay(500);
        }
    }
  else if(teamName == "blue"){

    //blueTeam full matrix flash
    for(int i = 0; i < 3; i++)
        {
          blueMatrix.drawRect(0,0, blueMatrix.width(), blueMatrix.height(), 255);
          blueMatrix.fillRect(0,0, blueMatrix.width(), blueMatrix.height(), 255);
          delay(500);
          blueMatrix.clear();
          delay(500);
        }
  }
  else if(teamName == "both"){

    //Both sides full matrix flash
    for(int i = 0; i < 3; i++)
      {
          redMatrix.drawRect(0,0, redMatrix.width(), redMatrix.height(), 255);
          redMatrix.fillRect(0,0, redMatrix.width(), redMatrix.height(), 255);
          blueMatrix.drawRect(0,0, blueMatrix.width(), blueMatrix.height(), 255);
          blueMatrix.fillRect(0,0, blueMatrix.width(), blueMatrix.height(), 255);
          delay(500);
          redMatrix.clear();
          blueMatrix.clear();
          delay(500);
      }
  }
      
}

//Counts down from 3 on led matrices
void countDown(){
    //Sets parameters for led matrices
    blueMatrix.setTextSize(1);
    blueMatrix.setTextWrap(false);
    blueMatrix.setTextColor(100);
    blueMatrix.print("3");
    redMatrix.setTextSize(1);
    redMatrix.setTextWrap(false);
    redMatrix.setTextColor(100);

    //Flashes countdown on led matrices
    for(int i = 3; i>= 0; i--)
      {
        redMatrix.setCursor(5,0);
        blueMatrix.setCursor(5,0);
        String intString = String(i);
        redMatrix.print(intString);
        blueMatrix.print(intString);
        delay(500);
        redMatrix.clear();
        blueMatrix.clear();
        delay(500);
      }
    
}
//Plays celebration sound
//TODO possibly create more sounds, or a game winning sound
void playSound(){
  //Declares notes and durations
  int notes[] = {262, 294, 330, 349, 392, 440, 494, 523};
  int durations[] = {100, 100, 100, 100, 100, 100, 100, 100};
  //Plays notes for declared durations
  for (int i = 0; i < 8; i++) {
    tone(speakerPin, notes[i]);
    delay(durations[i]);
  }
  // Stop playing the tune
  noTone(speakerPin);
  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
  //moveServos();
  //moveLaser();
  //checkLight();
  setDisplay();
}
