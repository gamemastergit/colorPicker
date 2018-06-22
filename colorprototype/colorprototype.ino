/*
Color picker prototype

Finds a the color of an object passed by the sensor and compares it to a 2D array of correct colors

Feedback is displated on an LCD panel

Written by Jack Eller for Escape Reality Games

 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Adafruit_TCS34725.h"
LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
byte gammatable[256];
String color = "nothing";
String change;



byte percentage_1[8] = { B10000, B10000, B10000, B10000, B10000, B10000, B10000, B10000 };
byte percentage_2[8] = { B11000, B11000, B11000, B11000, B11000, B11000, B11000, B11000 };
byte percentage_3[8] = { B11100, B11100, B11100, B11100, B11100, B11100, B11100, B11100 };
byte percentage_4[8] = { B11110, B11110, B11110, B11110, B11110, B11110, B11110, B11110 };
byte percentage_5[8] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };


int speaker = 2;

void setup()
{
  pinMode(2, OUTPUT);
  randomSeed(analogRead(0));
  lcd.init();                      // initialize the lcd
  lcd.backlight();
  Serial.begin(9600);
    if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }

  lcd.print("Warming Up...");

  lcd.createChar(0, percentage_1);
  lcd.createChar(1, percentage_2);
  lcd.createChar(2, percentage_3);
  lcd.createChar(3, percentage_4);
  lcd.createChar(4, percentage_5);

  lcd.begin(16, 2);
    for(int i = 0; i <= 100; i++){
    lcd.setCursor(0,0);
    lcd.print("Loading :");
    lcd.print(i);
    lcd.print("%");
    lcd_percentage(i, 0, 16, 1);
    delay(10);
  }

  lcd.clear();
  lcd.noCursor();
  //delay(5000);
}

String colorMatrix [5][5] = {{"Blue","Yellow","Green","Red","Purple"},{"Green","Purple","Blue","Yellow","Red"},{"Blue","Green","Red","Yellow","Purple"},{"Red","Blue","Green","Purple","Yellow"},{"Yellow","Purple","Red","Green","Blue"}};

void loop()
{
  uint16_t r, g, b, c, colorTemp, lux;

  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);

  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
  int puzzle = 0;
  while(puzzle < 6){
  int x = random(5);
  int y = random(5);
  char letter = char(x + 65);
  boolean working = true;
  String answer = colorMatrix[x][y];
  Serial.println("NOW WORKING");
  lcd.clear();
  lcd.print("PLEASE PLACE ");
  lcd.print(letter);
  lcd.print(y + 1);
  while(working){

  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);

  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
  double dr = r;
  double dg = g;
  double db = b;
  double dc = c;
  if(r > 1000 && g > 1000 && b > 1000){
    Serial.println("I SEE SOMETHING!");
    boolean got = false;

  if(dr > db + dg && !got){
        color = ("Red");
        Serial.println("IT'S " + color);
        got = true;
  }

  if(db > dg + dr && !got){
        color = ("Blue");
        Serial.println("IT'S " + color);
                got = true;

  }



  if((dr + dg) / dc > 0.80 && !got && colorTemp < 3500){
          color = ("Yellow");
          Serial.println("IT'S " + color);
                  got = true;

  }
  if((dr + db) / dc > 0.65 && !got){
           color = ("Purple");
        Serial.println("IT'S " + color);

          got = true;

  }
    if(dg / dc > 0.4 && !got && colorTemp. > 3500){

        color=("Green");
        Serial.println("IT'S " + color);
                got = true;

  }



  }

  if(color.equalsIgnoreCase(answer)){
    lcd.clear();
    thumbsup();
    working = false;
    tone(speaker, 1000, 200);
    delay(200);
    tone(speaker, 1500, 200);
    delay(200);
    tone(speaker, 2500, 200);

    delay(5000);
  }


  }
  }
}

 void thumbsup() {
   byte thumb1[8] = {B00100,B00011,B00100,B00011,B00100,B00011,B00010,B00001};
   byte thumb2[8] = {B00000,B00000,B00000,B00000,B00000,B00000,B00000,B00011};
   byte thumb3[8] = {B00000,B00000,B00000,B00000,B00000,B00000,B00001,B11110};
   byte thumb4[8] = {B00000,B01100,B10010,B10010,B10001,B01000,B11110,B00000};
   byte thumb5[8] = {B00010,B00010,B00010,B00010,B00010,B01110,B10000,B00000};
   byte thumb6[8] = {B00000,B00000,B00000,B00000,B00000,B10000,B01000,B00110};
   lcd.createChar(0, thumb1);
   lcd.createChar(1, thumb2);
   lcd.createChar(2, thumb3);
   lcd.createChar(3, thumb4);
   lcd.createChar(4, thumb5);
   lcd.createChar(5, thumb6);
   lcd.setCursor(4,1);
   lcd.write(0);
   lcd.setCursor(4,0);
   lcd.write(1);
   lcd.setCursor(5,1);
   lcd.write(2);
   lcd.setCursor(5,0);
   lcd.write(3);
   lcd.setCursor(6,1);
   lcd.write(4);
   lcd.setCursor(6,0);
   lcd.write(5);
  }

 void thumbdown() {
 byte thumb1[8] = {B00001,B00010,B00011,B00100,B00011,B00100,B00011,B00100};
 byte thumb2[8] = {B00011,B00000,B00000,B00000,B00000,B00000,B00000,B00000};
 byte thumb3[8] = {B11110,B00001,B00000,B00000,B00000,B00000,B00000,B00000};
 byte thumb4[8] = {B00000,B11110,B01000,B10001,B10010,B10010,B01100,B00000};
 byte thumb5[8] = {B00000,B10000,B01110,B00010,B00010,B00010,B00010,B00010};
 byte thumb6[8] = {B00110,B01000,B10000,B00000,B00000,B00000,B00000,B00000};
 lcd.createChar(0, thumb1);
 lcd.createChar(1, thumb2);
 lcd.createChar(2, thumb3);
 lcd.createChar(3, thumb4);
 lcd.createChar(4, thumb5);
 lcd.createChar(5, thumb6);
 lcd.setCursor(4,0);
 lcd.write(0);
 lcd.setCursor(4,1);
 lcd.write(1);
 lcd.setCursor(5,0);
 lcd.write(2);
 lcd.setCursor(5,1);
 lcd.write(3);
 lcd.setCursor(6,0);
 lcd.write(4);
 lcd.setCursor(6,1);
 lcd.write(5);
}
void lcd_percentage(int percentage, int cursor_x, int cursor_x_end, int cursor_y){

  int calc = (percentage*cursor_x_end*5/100)-(percentage*cursor_x*5/100);
  while(calc >= 5){
    lcd.setCursor(cursor_x,cursor_y);
    lcd.write((byte)4);
    calc-=5;
    cursor_x++;
  }
  while(calc >= 4 && calc < 5){
    lcd.setCursor(cursor_x,cursor_y);
    lcd.write((byte)3);
    calc-=4;

  }
  while(calc >= 3 && calc < 4){
    lcd.setCursor(cursor_x,cursor_y);
    lcd.write((byte)2);
    calc-=3;
  }
  while(calc >= 2 && calc < 3){
    lcd.setCursor(cursor_x,cursor_y);
    lcd.write((byte)1);
    calc-=2;
  }
  while(calc >= 1 && calc < 2){
    lcd.setCursor(cursor_x,cursor_y);
    lcd.write((byte)0);
    calc-=1;
  }

}
