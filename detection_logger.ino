#include "IRremote.h"
#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <SPI.h>
#include <SD.h>

#define receiver 2
#define redLED 3
#define greenLED 5
#define pir 7

const int SDModule = 4;

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
  digitalWrite(LED_BUILTIN, HIGH);

  int i;
  pirValue = digitalRead(pir);

  i ++;
  if  (i <= 1 && alarmStatus == true)
  {
    digitalWrite(greenLED, pirValue);
    //Print();

    delay(100);
    digitalWrite(greenLED, LOW);
  }

  else {

    digitalWrite(LED_BUILTIN, LOW);
  }

}

//better formatting for time
void Print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

//for testing with serial monitor
void Print()
{
  tmElements_t tm;

  RTC.read(tm);
  Serial.print("Time = ");
  Print2digits(tm.Hour);
  Serial.write(':');
  Print2digits(tm.Minute);
  Serial.write(':');
  Print2digits(tm.Second);
  Serial.print(", Date (D/M/Y) = ");
  Serial.print(tm.Day);
  Serial.write('/');
  Serial.print(tm.Month);
  Serial.write('/');
  Serial.print(tmYearToCalendar(tm.Year));
  Serial.println();

  delay(500);

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

  pinMode(SDModule, OUTPUT);

  //is SD card detected
  if (!SD.begin(SDModule)) {
    Serial.println("no card");
    return;
  }

  Serial.println("sd ready");

}

void loop()
{

  if (irrecv.decode(&results)) // IR signal received

  {
    translateIR();
    Serial.println(alarmStatus);
    irrecv.resume(); // receive next value
  }

  Alarm();

  if  (pirValue > 0 && alarmStatus == true)
  {
    //serial monitor
    Print();
    File logData = SD.open("example.txt", FILE_WRITE);

  if (logData)
  {
    Serial.println("logging");
    tmElements_t tm;
    RTC.read(tm);
    logData.print("date: ");
    logData.print(tm.Day);
    logData.write('.');
    logData.print(tm.Month);
    logData.write('.');
    logData.print(tmYearToCalendar(tm.Year));
    logData.print(", time: ");
    logData.print(tm.Hour);
    logData.write(':');
    logData.print(tm.Minute);
    logData.write(':');
    logData.print(tm.Second);
    logData.println();

    logData.close();

    delay(2000);

  }
  else if (!logData) {
    Serial.println("error with file");
  }

  else  {
    Serial.println("alarm not on or no motion");
  }
  }


  


  /*
    Serial.print(pirValue);
    delay(500);
  */
}
