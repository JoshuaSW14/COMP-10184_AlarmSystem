
// COMP-10184 – Mohawk College 
// COMP-10184_AlarmSystem
//  
// This program demonstrates an alarm system. When the application starts, the alarm is enabled and when
// motion is detected the 10 second countdown starts. If the button is not clicked during the countdown then
// the alarm is activated. If the button is clicked during the countdown then the alarm will be disabled
// and not enabled until the button is clicked again.
//
// @author  Joshua Symons-Webb
// @id      000812836
// 
// I, Joshua Symons-Webb, 000812836 certify that this material is my original work. No
// other person's work has been used without due acknowledgement.
// 

#include <Arduino.h>

#define PIN_PIR D5 //Input pin for PIR Sensor
#define PIN_BUTTON D6 //Input pin for Button

#define BUTTON_DELAY 200 //Delay for button input

//Different Alarm States
#define ALARM_DISABLED 0
#define ALARM_ENABLE 1
#define ALARM_COUNTDOWN 2
#define ALARM_ACTIVE 3

int iAlarmState; //Variable to hold alarm state
bool bPIR; //Variable to hold the state of the PIR sensor

// *********************************************************** 
// Collects the input of the PIR sensor and will start the alarm countdown if motion is detected
//
void collectInputs()
{
  bPIR = digitalRead(PIN_PIR); //Read PIR sensor for input

  if (bPIR && iAlarmState == ALARM_ENABLE) //Check if alarm is enabled and motion is detected
  {
    Serial.println("Motion Detected: Alarm Countdown Started");
    iAlarmState = ALARM_COUNTDOWN; //Alarm state changed to countdown
  }
}

// *********************************************************** 
// Starts a 10 second countdown and waits for a button input from the user or will activate the alarm if no input is detected
//
void countdown()
{
  int count = 0;

  while (count < 10 && iAlarmState == ALARM_COUNTDOWN) //Loop to count each second
  {
    int count2 = 0;
    while(count2 < 4){ //Blink 4 times

      //If button was pressed during countdown, disable alarm and turn off LED
      if(!digitalRead(PIN_BUTTON)){
        Serial.println("Button Pressed: Countdown Stopped");
        digitalWrite(LED_BUILTIN, HIGH);
        iAlarmState = ALARM_DISABLED;
        delay(BUTTON_DELAY); //Delay to avoid immediately flipping between enabled and disabled alarm states
        break;
      }

      //Blink LED On/Off
      digitalWrite(LED_BUILTIN, HIGH);
      delay(125);
      digitalWrite(LED_BUILTIN, LOW);
      delay(125);

      count2 += 1;
    }
    count += 1;
  }

  //If countdown has reach 10 seconds
  if(count == 10){
    iAlarmState = ALARM_ACTIVE; //Set alarm to active
  }
}

// *********************************************************** 
// Checks the state of the variable iAlarmState
//
void checkAlarmState()
{
  switch (iAlarmState) //Switch case to switch between the different alarm states
  {
  case ALARM_ENABLE: //Alarm is enabled, turn off LED
    Serial.println("Alarm Enabled");
    digitalWrite(LED_BUILTIN, HIGH);
    break;
  case ALARM_DISABLED: //Alarm is disabled, wait for button input to enable alarm
    Serial.println("Alarm Disabled");
    while(!digitalRead(PIN_BUTTON)){
      iAlarmState = ALARM_ENABLE;
    }
    delay(BUTTON_DELAY); //Delay to avoid immediately flipping between enabled and disabled alarm states
    break;
  case ALARM_ACTIVE: //Alarm has been activated, turn on LED
    Serial.println("Alarm Activated");
    digitalWrite(LED_BUILTIN, LOW);
    break;
  case ALARM_COUNTDOWN: //Motion detect, start the countdown
    Serial.println("Alarm Countdown Started: 10 seconds");
    countdown();
    break;
  default:
    break;
  }
}

// ***********************************************************
void setup()
{
  Serial.begin(115200); //configure the USB serial monitor

  //Introduction Message
  Serial.println("COMP-10184 - Alarm System");
  Serial.println("Name: Joshua Symons-Webb");
  Serial.println("Student ID: 000812836 \n\n\n");
  delay(500); //Delay for intro message

  pinMode(LED_BUILTIN, OUTPUT); //configure the LED output 
  pinMode(PIN_PIR, INPUT); //configure the PIR sensor as an input
  pinMode(PIN_BUTTON, INPUT_PULLUP); //configure the Button as an input
  iAlarmState = ALARM_ENABLE; //Enable the alarm
}

// ***********************************************************
void loop()
{
  collectInputs(); //check for inputs
  checkAlarmState(); //check alarm state
}