#include <Wire.h>
#include "RTClib.h"
RTC_DS3231 rtc;

int sensorPin = A0; ///////  sensor ldr di pin analog 0
int sensorValue = 0;

#include <Servo.h>
Servo myservo;
int pos = 0;

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);



void setup ()
{
  Serial.begin(9600);

  myservo.attach(9);//// pin digital 9

  pinMode(7, OUTPUT); // relay sebagai output
  pinMode(8, OUTPUT); // buzzer sebagai output

  delay(3000);

  myservo.write(00);

  if (! rtc.begin())
  {
    Serial.println("RTC tidak terbaca");
    while (1);
  }

  if (rtc.lostPower())
  {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  lcd.init();
  lcd.backlight();
}

void loop ()
{
  DateTime now = rtc.now();
  sensorValue = analogRead(sensorPin); //Membaca nilai analog dari pin A5
  float voltage = sensorValue * (5.0 / 1023.0);
  waktu ();

  ////////////////////// pengujian pakan habis////////////////////////
  if (voltage <= 1)
  {
    lcd.setCursor(3, 1);
    lcd.print("PAKAN HABIS");
    digitalWrite (8, HIGH );
    delay (5000);
    lcd.clear();
  }

  //////////////////// pengujian memberi makan ikan/////////////////
  if (now.hour() == 8 && now.minute() == 30 && now.second() == 00  )
  {
    waktu ();
    lcd.setCursor(4, 1);
    lcd.print("PAKAN ON");
    myservo.write(180);
    delay(5000);
    myservo.write(00);
  }

  /////////////////////////////////////////////// pompa air hidup////////////////
  if (now.hour() >= 18 && now.minute() == 00 && now.second() == 00 || now.hour() <= 06 && now.minute() == 00 && now.second() == 00 )
  {
    lcd.clear();
    lcd.setCursor(4, 1);
    lcd.print("POMPA ON");
    digitalWrite (7, LOW );
  }
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  delay(1000);
}

///////////////// pengujian rtc//////////////////
void waktu ()
{
  DateTime now = rtc.now();
  lcd.setCursor(0, 0);
  lcd.print("Jam= ");
  lcd.setCursor(4, 0);
  lcd.print(now.hour());
  lcd.setCursor(7, 0);
  lcd.print(":");
  lcd.setCursor(9, 0);
  lcd.print(now.minute());
  lcd.setCursor(12, 0);
  lcd.print(":");
  lcd.setCursor(14, 0);
  lcd.print(now.second());
}
