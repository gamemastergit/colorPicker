/*
Color picker prototype

Finds a the color of an object passed by the sensor and compares it to a 2D array of correct colors

Feedback is displated on an LCD panel

Written by Jack Eller for Escape Reality Games

 */


/*
   VERSION : 1.2
   UPDATES :
    -  Added Counter
    -  Added Infinite loop and data out pin to main controller
   PINS    :
    - Green-White      = SDA        [ A4 ]
    - Blue-White       = SLC / CLK  [ A5 ]
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Adafruit_TCS34725.h"
#include "talkie.h"

Talkie voice;
LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
byte gammatable[256];
String color = "";
String change;

const uint8_t spE[]         PROGMEM = {0x21,0x3A,0x4A,0x41,0x77,0x63,0xCF,0x68,0x27,0x3B,0x1D,0xC5,0x6A,0x4A,0x38,0x55,0x7B,0x16,0xB3,0x2E,0xED,0xAA,0x42,0x78,0x15,0xB5,0xB6,0xA5,0x9F,0x19,0xD9,0x2F,0xE3,0xD4,0x3E,0x0F,0x69,0x98,0x01,0xD9,0x2F,0xE3,0xE4,0xE1,0xFF,0x09,0x61,0x41,0x51};
const uint8_t spD[]         PROGMEM = {0x0C,0xB8,0xD0,0x64,0x64,0x0F,0x15,0x8B,0x5A,0xE8,0x15,0x6C,0xE1,0xA0,0x39,0x8B,0x8E,0xFB,0x4C,0x00,0x57,0x2F,0x3A,0xEE,0x75,0x09,0x5C,0x33,0xEB,0xF8,0xC7,0x3A,0x70,0xEF,0xEC,0x13,0x6D,0xEB,0x20,0xB3,0xB5,0x46,0x32,0x2D,0x43,0xF6,0xC4,0x1E,0xC5,0x8A,0x0A,0x3A,0x13,0xBB,0x55,0xDB,0x3B,0x70,0x4F,0xE5,0xD6,0xEC,0x68,0xC0,0xBD,0x95,0x7B,0x97,0x87,0x38,0xFD,0x41,0x15,0x41,0x11};
const uint8_t spN[]         PROGMEM = {0x27,0x9E,0x71,0x99,0x3D,0x16,0x9D,0x68,0xBA,0x22,0xCB,0xAC,0x75,0xA2,0xE9,0x92,0x22,0x73,0xD6,0x89,0xA6,0x2F,0xF2,0xC8,0x59,0x27,0x59,0x61,0x55,0x4D,0x6A,0x9D,0x7C,0xBB,0x52,0x0D,0xAD,0x35,0xAA,0xAE,0x9D,0x5A,0x3C,0x76,0xAB,0xBB,0x72,0x1A,0x0E,0x3B,0xA5,0xED,0x2A,0x68,0x30,0x9D,0x96,0xBE,0x69,0xE7,0xC1,0x54,0x53,0x87,0x3A,0xD4,0xA1,0x0C,0x4D,0xBB,0x14,0xBA,0xDB,0x34,0x14,0x1F,0x62,0x16,0xB1,0x42,0x57,0x8C,0x49,0x8A,0xC5,0xFC,0x51,0x59,0x41,0x39};
const uint8_t spO[]         PROGMEM = {0x2B,0xC9,0x21,0xCD,0x2C,0x1F,0xEE,0xE4,0xA4,0xC5,0x54,0x9A,0xE9,0xA6,0x9B,0xAE,0x2C,0xBB,0x4A,0x55,0xDB,0xB4,0xB3,0x95,0xA7,0xD4,0x2E,0xEA,0xAB,0x67,0xDE,0xF2,0x68,0xC7,0x4D,0x74,0x63,0x29,0x82,0x5E,0x4F,0xD4,0x9D,0xB9,0x08,0x95,0xB1,0xE7,0x2E,0xB6,0xD1,0x57,0xA6,0x52,0x69,0xD4,0xB5,0x16,0xAA,0xCA,0xB7,0x54,0xE1,0x1C,0xFD,0x69,0x61,0x79,0x19,0x61,0x72,0x79};
const uint8_t spR[]         PROGMEM = {0x23,0x2C,0xEE,0x2A,0x8C,0x57,0xAE,0x28,0xC7,0x4F,0x33,0x5D,0xB8,0xB2,0xEC,0xAF,0xD2,0x78,0xD5,0xCE,0x46,0x53,0xCC,0x74,0x25,0x6E,0xEE,0x7D,0x19,0xAA,0xDA,0xF1,0xC4,0xCD,0x7D,0xE8,0x73,0x9F,0xFB,0x5C,0x86,0xA2,0x7F,0x2C,0x79,0x55,0x1D,0xD2,0x98,0xE6,0xA6,0xAB,0x8D,0xCE,0xE3,0xFF,0x11,0x51,0x25};
const uint8_t spT[]         PROGMEM = {0x0E,0x38,0xC2,0xD4,0x00,0x57,0xB5,0x36,0x77,0x29,0x26,0xCF,0xB6,0xBD,0xBC,0xE5,0x0C,0xA4,0xA7,0xF6,0xF2,0x97,0x53,0xB0,0xD9,0xC6,0x3B,0x5C,0xC9,0x76,0x0A,0x32,0x5B,0x7B,0x66,0xA3,0xDC,0x41,0x50,0x7A,0x6A,0xB7,0x66,0x27,0x45,0xDA,0x89,0x53,0xFA,0x95,0x18,0x79,0x27,0x4E,0x19,0x56,0x20,0x94,0xEB,0x3A,0x75,0x4C,0xE3,0x74,0x48,0xFE,0x15,0x27,0x8E,0x71,0x74,0xE3,0xF0,0x20,0x3D,0x15,0xC5,0x0D,0xCD,0xA0,0x6E,0x8F,0xED,0xFF,0x39,0x51,0x15};


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
   delay(500);
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

byte percentage_1[8] = { B10000, B10000, B10000, B10000, B10000, B10000, B10000, B10000 };
byte percentage_2[8] = { B11000, B11000, B11000, B11000, B11000, B11000, B11000, B11000 };
byte percentage_3[8] = { B11100, B11100, B11100, B11100, B11100, B11100, B11100, B11100 };
byte percentage_4[8] = { B11110, B11110, B11110, B11110, B11110, B11110, B11110, B11110 };
byte percentage_5[8] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };


int speaker = 2;
int count_right = 0 ; // Keep track of correct code count inputted
int data_out    = 12; // Digital Pin 12 = Data Out to Main Room Controller [ Outside of room by toy shop ]

void setup()
{
  pinMode(data_out,OUTPUT);
  digitalWrite(data_out,HIGH);
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

//String colorMatrix [5][5] = {{"Blue","Yellow","Green","Red","Purple"},{"Green","Purple","Blue","Yellow","Red"},{"Blue","Green","Red","Yellow","Purple"},{"Red","Blue","Green","Purple","Yellow"},{"Yellow","Purple","Red","Green","Blue"}};
String answers[5] = {"Yellow", "Purple", "Green", "Red", "Blue"};
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
  while(puzzle < 5){

  boolean working = true;
  String answer = answers[puzzle];
  Serial.println("NOW WORKING");
  lcd.clear();
  lcd.print("PLACE BALLS HERE");

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
  int type = 0;
  if(r > 1000 && g > 1000 && b > 1000){
    Serial.println("I SEE SOMETHING!");
    boolean got = false;

  if(dr > db + dg && !got){
        color = ("Red");
        Serial.println("IT'S " + color);
        got = true;
        type = 1;
  }

  if(db > dg + dr && !got){
        color = ("Blue");
        Serial.println("IT'S " + color);
        got = true;
        type = 2;
  }



  if((dr + dg) / dc > 0.80 && !got && colorTemp < 3500){
          color = ("Yellow");
          Serial.println("IT'S " + color);
          got = true;
          type = 3;

  }
  if((dr + db) / dc > 0.65 && !got){
          color = ("Purple");
          Serial.println("IT'S " + color);
          got = true;
          type = 4;
  }
    if(dg / dc > 0.4 && !got && colorTemp > 3500){

          color=("Green");
          Serial.println("IT'S " + color);
          got = true;
          type = 5;

  }



  }

  if(color.equalsIgnoreCase(answer)){
    puzzle--;
    lcd.clear();
    thumbsup();
    working = false;
    tone(speaker, 1000, 200);
    delay(200);
    tone(speaker, 1500, 200);
    delay(200);
    tone(speaker, 2500, 200);
    delay(5000);

switch(type){

  case 1:
    voice.say(spN);
    delay(200);
    voice.say(spT);
  break;
  case 2:
    voice.say(spE);
    delay(200);
    voice.say(spR);
  break;
  case 3:
    voice.say(spD);
    delay(200);
    voice.say(spO);
  break;
  case 4:
    voice.say(spN);
    delay(200);
    voice.say(spO);
  break;
  case 5:
    voice.say(spT);
    delay(200);
    voice.say(spE);
  break;
}

  }else{

    puzzle = 0;
    lcd.clear();
    thumbdown();


  }
  }
  lcd.clear();
    lcd.print("COLOR MATRIX HACKED!");
    Serial.println("HACKED");
    delay(1000);
    digitalWrite(data_out,LOW); // Main controller looking for zero / low

 }

}

