//I, Joshua Symons-Webb, 000812836 certify that this material is my original work. No
//other person's work has been used without due acknowledgement.

#include <Arduino.h>

#define PIN_PIR D5 //Input pin for PIR Sensor
#define PIN_BUTTON D6 //Input pin for Button

//Different Alarm States
#define ALARM_DISABLED 0
#define ALARM_ENABLE 1
#define ALARM_COUNTDOWN 2
#define ALARM_ACTIVE 3

int iAlarmState; //Variable to hold alarm state
bool bPIR; //Variable to hold the state of the PIR sensor

void collectInputs()
{
  bPIR = digitalRead(PIN_PIR); //Read PIR sensor for input

  if (bPIR && iAlarmState == ALARM_ENABLE) //Check if alarm is enabled and motion is detected
  {
    Serial.println("Motion Detected: Alarm Countdown Started");
    iAlarmState = ALARM_COUNTDOWN; //Alarm state changed to countdown
  }
}

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
    delay(500);
    while(!digitalRead(PIN_BUTTON)){
      iAlarmState = ALARM_ENABLE;
    }
    delay(500);
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

void setup()
{
  Serial.begin(115200); //configure the USB serial monitor
  delay(500);

  //Introduction Message
  Serial.println("COMP-10184 - Alarm System");
  Serial.println("Name: Joshua Symons-Webb");
  Serial.println("Student ID: 000812836 \n\n\n");

  pinMode(LED_BUILTIN, OUTPUT); //configure the LED output 
  pinMode(PIN_PIR, INPUT); //configure the PIR sensor as an input
  pinMode(PIN_BUTTON, INPUT_PULLUP); //configure the Button as an input
  iAlarmState = ALARM_ENABLE; //Enable the alarm
}

void loop()
{
  collectInputs(); //check for inputs
  checkAlarmState(); //check alarm state
}