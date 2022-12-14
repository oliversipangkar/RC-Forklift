#include <PS2X_lib.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Stik
PS2X ps2x;
#define PS2_DAT 50
#define PS2_CMD 51
#define PS2_SEL 53
#define PS2_CLK 52
#define pressures false
#define rumble false

//Pin L298N
#define m11 3
#define m12 4
#define m21 5
#define m22 6
#define eL 7
#define eR 2

//LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Class
Servo servomanuver, servodh;
int error = 0;
byte vibrate = 0;
int sudut = 0;
int sudutdh = 0;
int motor = 0;
int servovalue = 0;
int servovalue1 = 0;
int servovaluedh = 0;
int pil = 0;
int analogmotor = 0;

void setup()
{
  //LCD
  lcd.begin();

  //Motor
  pinMode(m11, OUTPUT);
  pinMode(m12, OUTPUT);
  pinMode(m21, OUTPUT);
  pinMode(m22, OUTPUT);
  pinMode(eL, INPUT);
  pinMode(eR, INPUT);

  //Servo
  servomanuver.attach(12);
  servomanuver.write(78);
  servodh.attach(13);

  //Konek Stik
  delay(300);
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  if (error == 0)
  {
    Serial.println("Kelazzz Konek Stiknya Browww\n");
  }
  else
  {
    Serial.println("Ga Konek Stiknya Pak");
  }

  //Start Menu
  menu(pil);
}

void loop()
{
  ps2x.read_gamepad(false, 0);

  if (pil == 0)
  {
    if (ps2x.Button(PSB_START))
    {
      pil += 1;
      lcd.clear();
    }
  }
  else
  { 
    //Lifter
    if (ps2x.Button(PSB_PAD_DOWN))
    {
      sudutdh += 2;
      servodh.write(sudutdh);
    }
    if (ps2x.Button(PSB_PAD_UP))
    {
      sudutdh -= 2;
      servodh.write(sudutdh);
    }
    servovaluedh = sudutdh;
    servovaluedh = map(servovaluedh, 50, 180, 0, 100);
    

    //Kecepatan Motor
    if (ps2x.Button(PSB_L2))
    {
      analogmotor = ps2x.Analog(PSS_LY);
      
      if (ps2x.Analog(PSS_LY) == 128)
      {
        motor = 0;
        drivemati(motor, motor);
      }
      if (ps2x.Analog(PSS_LY) < 128)
      {
        motor = ((128 - ps2x.Analog(PSS_LY)) * 2) - 100;
        drivemaju(motor, motor);
      }
      if (ps2x.Analog(PSS_LY) > 128)
      {
        motor = ((128 - ps2x.Analog(PSS_LY)) * (-2)) - 100;
        drivemundur(motor, motor);
      }
    }

    
    //Manuver
    if (ps2x.Button(PSB_R2))
    {
      servovalue1 = ps2x.Analog(PSS_RX);
      
      if (ps2x.Analog(PSS_RX) == 128)
      {
        servomanuver.write(78);
      }
      if (ps2x.Analog(PSS_RX) < 128)
      {
        servovalue = map(servovalue1, 0, 127, 55, 78);
        servomanuver.write(servovalue);
      }
      if (ps2x.Analog(PSS_RX) > 128)
      {
        servovalue = map(servovalue1, 129, 255, 78, 110);
        servomanuver.write(servovalue);
      }
    }
    //Monitor
    menu(pil);
  }
  //Delay Loop
  delay(1);
}

void drivemaju (int speedleft, int speedright)
{
  analogWrite(eR, speedright);
  analogWrite(eL, speedleft);
  digitalWrite(m11, LOW);
  digitalWrite(m12, HIGH);
  digitalWrite(m21, LOW);
  digitalWrite(m22, HIGH);
}

void drivemundur(int speedleft, int speedright)
{
  analogWrite(eR, speedright);
  analogWrite(eL, speedleft);
  digitalWrite(m11, HIGH);
  digitalWrite(m12, LOW);
  digitalWrite(m21, HIGH);
  digitalWrite(m22, LOW);
}

void drivemati(int speedleft, int speedright)
{
  analogWrite(eL, speedleft);
  analogWrite(eR, speedright);
  digitalWrite(m11, LOW);
  digitalWrite(m12, LOW);
  digitalWrite(m21, LOW);
  digitalWrite(m22, LOW);
}

void menu(int x)
{
  switch (x)
  {
    case 0:
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("Rokami");
      lcd.setCursor(2, 1);
      lcd.print("Press Start-");
      break;
    case 1:
      lcd.setCursor(0, 0);
      lcd.print("-Rokami-");
      
      lcd.setCursor(9, 0);
      lcd.print("Spd=");
      lcd.setCursor(13, 0);
      if (analogmotor == 128)
      {
        lcd.setCursor(13, 0);
        lcd.print("ST");
      }
      if (analogmotor < 128)
      {
        lcd.setCursor(13, 0);
        lcd.print("FW");
      }
      if (analogmotor > 128)
      {
        lcd.setCursor(13, 0);
        lcd.print("RV");
      }
      
      lcd.setCursor(0, 1);
      lcd.print("Dir=");
      if (servovalue1 == 128)
      {
        lcd.setCursor(4, 1);
        lcd.print("F");
      }
      if (servovalue1 < 128)
      {
        lcd.setCursor(4, 1);
        lcd.print("L");
      }
      if (servovalue1 > 128)
      {
        lcd.setCursor(4, 1);
        lcd.print("R");
      }

      lcd.setCursor(6, 1);
      lcd.print("Lifter=");
      lcd.setCursor(13, 1);
      lcd.print(servovaluedh);
      lcd.setCursor(15, 1);
      lcd.print("%");
      
      break;
  }
}
