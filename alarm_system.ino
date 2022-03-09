#include "IRremote.h"

#define receiver 2 // Signal Pin of IR receiver to Arduino Digital Pin 2
#define redLED 3
#define greenLED 5
#define pir 4
#define buzzer 7

int pirValue;
bool alarmStatus = false;

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

/*-----( Function )-----*/
void translateIR() // takes action based on IR code received


{

  switch (results.value)

  {
    case 0xFFE01F:
      Serial.println("ALARM___OFF");
      digitalWrite(greenLED, HIGH);
      digitalWrite(redLED, LOW);
      alarmStatus = false;
      break;

    case 0xFFA857:
      Serial.println("ALARM___ON");
      digitalWrite(redLED, HIGH);
      digitalWrite(greenLED, LOW);
      alarmStatus = true;
      break;


    default:
      Serial.println(" other   ");

  }

  delay(500); // Do not get immediate repeat


} //END translateIR

void Alarm()
{

  if (alarmStatus == true)
  {

    digitalWrite(LED_BUILTIN, HIGH);


    int i;
    pirValue = digitalRead(pir);

    i ++;
    if  (i <= 1)
    {
      digitalWrite(buzzer, pirValue);
      delay(10);
      digitalWrite(buzzer, LOW);
    }
  }

  else {

    digitalWrite(LED_BUILTIN, LOW);
  }

}


void setup()
{
  Serial.begin(9600);
  Serial.println("system started");
  irrecv.enableIRIn();

  pinMode(pir, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
}


void loop()
{
  if (irrecv.decode(&results)) // IR signal received

  {
    translateIR();
    Serial.println(alarmStatus);
    irrecv.resume(); // receive the next value
  }

  Alarm();
}
