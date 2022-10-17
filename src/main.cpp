#include <Arduino.h>

#define PIN_PIR D5
#define PIN_BUTTON D6

// define all possible alarm states.
#define ALARM_DISABLED 0
#define ALARM_ENABLE 1
#define ALARM_COUNTDOWN 2
#define ALARM_ACTIVE 3

int count = 0;
int iAlarmState;
bool bPIR;

int led = LOW;
int buttonState;
int lastButtonState;

void collectInputs()
{
  bPIR = digitalRead(PIN_PIR);

  lastButtonState = buttonState;
  buttonState = digitalRead(PIN_BUTTON);

  if (bPIR && iAlarmState == ALARM_ENABLE)
  {
    Serial.println("Motion Detected: Alarm Countdown Started");
    iAlarmState = ALARM_COUNTDOWN;
  }

  if (lastButtonState == HIGH && buttonState == LOW && iAlarmState == ALARM_COUNTDOWN)
  {
    Serial.println("Countdown Stopped: Alarm Deactived");
    iAlarmState = ALARM_DISABLED;
  }

  if (lastButtonState == HIGH && buttonState == LOW && iAlarmState == ALARM_DISABLED)
  {
    Serial.println("Alarm Armed");
    iAlarmState = ALARM_ENABLE;
  }
}

void countdown()
{
  while (count < 10 && iAlarmState == ALARM_COUNTDOWN)
  {
    collectInputs();
    digitalWrite(LED_BUILTIN, HIGH);
    delay(125);
    digitalWrite(LED_BUILTIN, LOW);
    delay(125);

    digitalWrite(LED_BUILTIN, HIGH);
    delay(125);
    digitalWrite(LED_BUILTIN, LOW);
    delay(125);

    digitalWrite(LED_BUILTIN, HIGH);
    delay(125);
    digitalWrite(LED_BUILTIN, LOW);
    delay(125);

    digitalWrite(LED_BUILTIN, HIGH);
    delay(125);
    digitalWrite(LED_BUILTIN, LOW);
    delay(125);

    count += 1;
  }

  if(iAlarmState == ALARM_COUNTDOWN){
    iAlarmState = ALARM_ACTIVE;
  }
}

void checkAlarmState()
{
  switch (iAlarmState)
  {
  case ALARM_ENABLE:
    Serial.println("Alarm Enabled");
    digitalWrite(LED_BUILTIN, HIGH);
    count = 0;
    break;
  case ALARM_DISABLED:
    Serial.println("Alarm Disabled");
    break;
  case ALARM_ACTIVE:
    Serial.println("Alarm Activated");
    digitalWrite(LED_BUILTIN, LOW);
    break;
  case ALARM_COUNTDOWN:
    Serial.println("Alarm Countdown");
    countdown();
    break;
  default:
    break;
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  buttonState = digitalRead(PIN_BUTTON);
  iAlarmState = ALARM_ENABLE;
}

void loop()
{
  collectInputs();
  checkAlarmState();
  delay(1000);
}