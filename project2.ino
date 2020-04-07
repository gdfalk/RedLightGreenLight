#include <analogWrite.h>

// states for state machine
int state=1;
int stateRedLight=1;
int stateGreenLight=2;
int stateWin=3;
int stateLose=4;

//variables for clock
unsigned long interval=5000;
unsigned long prevMillis=0;

//pins
int internalLED=13;
int redLED=12;
int greenLED=27;
int blueLED=33;
int button=A0;
int ldr=A1;

//light level checker variables
int lightbool=0;
int lightlevel=0;

void setup() {
Serial.begin(9600);
//Outputs
pinMode(internalLED,OUTPUT);
pinMode(redLED,OUTPUT);
pinMode(greenLED,OUTPUT);
pinMode(blueLED,OUTPUT);

//Inputs
pinMode(button, INPUT);
pinMode(ldr, INPUT);

//Sets initial light level for the start of the game
lightlevel=analogRead(A1);

}

void loop() {
  
  //debug
  //debug();
  
  //starts timer
  unsigned long currentMillis=millis();

  if(state==stateRedLight)
  {
    //just checks to see if the player moves via the light level until changes to GL
    //turns on red light
    analogWrite(redLED,HIGH);
    analogWrite(greenLED,LOW);

    //reads current light level
    int redRead=analogRead(A1);

    //if player ajusts the light level by a degree of 300 then you lose
    if(redRead>lightlevel+300 || redRead<lightlevel-300)
    {
      state=stateLose;
    }
    
  }else if(state==stateGreenLight)
  {
    //waits until changes to RL but checks if button has been pressed too
    //turns on green light
    analogWrite(greenLED,HIGH);
    analogWrite(redLED,LOW);

    //reads the button
    int greenRead=digitalRead(button);
    
    if(greenRead==HIGH)   //if button is pushed while light is green then you win
    {
      state=stateWin;
    }
    
  }else if(state==stateWin)
  {
    //turns on all lights and prints you win
    analogWrite(redLED,HIGH);
    analogWrite(greenLED,HIGH);
    analogWrite(blueLED,HIGH);
    Serial.println("YOU WIN");
    
  }else if(state==stateLose)
  {
    //turns on blue light on RGB and red light on esp32 and prints you lose
    analogWrite(internalLED,HIGH);
    analogWrite(redLED,LOW);
    analogWrite(greenLED,LOW);
    analogWrite(blueLED,HIGH);
    Serial.println("YOU LOSE");
    
  }



  if(state==stateRedLight)
  {
    //timer for red light
    int randotime=random(-3,3)*1000; //creates a random number that will add or subtract up to 3 seconds from the time interval
    if((currentMillis-prevMillis)>=(interval+randotime))
    {
      state=stateGreenLight; //changes to green light
      prevMillis=millis();
    }

    //boolean variable so that lightlevel is only gotten once
    lightbool=0;
    
  }else if(state==stateGreenLight)
  {
    //timer for green light
    int randotime=random(-3,3)*1000; //creates a random number that will add or subtract up to 3 seconds from the time interval
    if((currentMillis-prevMillis)>=(interval+randotime))
    {
      state=stateRedLight; //changes to red light
      prevMillis=millis();
      if(lightbool==0)
        {
          lightlevel=analogRead(A1);
          Serial.println(lightlevel); //gets light level that determines if player moves during red light
          lightbool=1; //changes lightlevel bool so this is only done once
        }
    }
    
    
  }
  
}


//debug to check if my wiring is set up correctly
void debug() {
  
  //checks if sensor is working
  int ldrread = analogRead(A1);
  Serial.print("Reading: ");
  Serial.println(ldrread);

  //checks if lights are working
  analogWrite(redLED,HIGH);
  analogWrite(greenLED,LOW);
  analogWrite(blueLED,LOW);

  //checks if button is working
  int buttonstate=digitalRead(button);
  if(buttonstate==HIGH)
    {
      Serial.println("ON");
    }else
    {
      Serial.println("OFF");
    }

  
}
