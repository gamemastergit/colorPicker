/*
  Color picker prototype

  Finds a the color of an object passed by the sensor and compares it to an array of correct colors
  Feedback is displated on an LCD panel and audio is piped out of pin D3

  Written by Jack Eller for Escape Reality Games

*/


/*
   PINS    :
    - Green-White      = SDA        [ A4 ]
    - Blue-White       = SLC / CLK  [ A5 ]
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x3F, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display


void thumbsup() {
  const byte thumb1[8] PROGMEM = {B00100, B00011, B00100, B00011, B00100, B00011, B00010, B00001};
  const byte thumb2[8] PROGMEM = {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00011};
  const byte thumb3[8] PROGMEM = {B00000, B00000, B00000, B00000, B00000, B00000, B00001, B11110};
  const byte thumb4[8] PROGMEM = {B00000, B01100, B10010, B10010, B10001, B01000, B11110, B00000};
  const byte thumb5[8] PROGMEM = {B00010, B00010, B00010, B00010, B00010, B01110, B10000, B00000};
  const byte thumb6[8] PROGMEM = {B00000, B00000, B00000, B00000, B00000, B10000, B01000, B00110};
  lcd.createChar(0, thumb1);
  lcd.createChar(1, thumb2);
  lcd.createChar(2, thumb3);
  lcd.createChar(3, thumb4);
  lcd.createChar(4, thumb5);
  lcd.createChar(5, thumb6);
  lcd.setCursor(4, 1);
  lcd.write(0);
  lcd.setCursor(4, 0);
  lcd.write(1);
  lcd.setCursor(5, 1);
  lcd.write(2);
  lcd.setCursor(5, 0);
  lcd.write(3);
  lcd.setCursor(6, 1);
  lcd.write(4);
  lcd.setCursor(6, 0);
  lcd.write(5);
  delay(500);
}



void thumbdown() {
  const byte thumb1[8] PROGMEM = {B00001, B00010, B00011, B00100, B00011, B00100, B00011, B00100};
  const byte thumb2[8] PROGMEM = {B00011, B00000, B00000, B00000, B00000, B00000, B00000, B00000};
  const byte thumb3[8] PROGMEM = {B11110, B00001, B00000, B00000, B00000, B00000, B00000, B00000};
  const byte thumb4[8] PROGMEM = {B00000, B11110, B01000, B10001, B10010, B10010, B01100, B00000};
  const byte thumb5[8] PROGMEM = {B00000, B10000, B01110, B00010, B00010, B00010, B00010, B00010};
  const byte thumb6[8] PROGMEM = {B00110, B01000, B10000, B00000, B00000, B00000, B00000, B00000};
  lcd.createChar(0, thumb1);
  lcd.createChar(1, thumb2);
  lcd.createChar(2, thumb3);
  lcd.createChar(3, thumb4);
  lcd.createChar(4, thumb5);
  lcd.createChar(5, thumb6);
  lcd.setCursor(4, 0);
  lcd.write(0);
  lcd.setCursor(4, 1);
  lcd.write(1);
  lcd.setCursor(5, 0);
  lcd.write(2);
  lcd.setCursor(5, 1);
  lcd.write(3);
  lcd.setCursor(6, 0);
  lcd.write(4);
  lcd.setCursor(6, 1);
  lcd.write(5);
}
void lcd_percentage(int percentage, int cursor_x, int cursor_x_end, int cursor_y) {

  int calc = (percentage * cursor_x_end * 5 / 100) - (percentage * cursor_x * 5 / 100);
  while (calc >= 5) {
    lcd.setCursor(cursor_x, cursor_y);
    lcd.write((byte)4);
    calc -= 5;
    cursor_x++;
  }
  while (calc >= 4 && calc < 5) {
    lcd.setCursor(cursor_x, cursor_y);
    lcd.write((byte)3);
    calc -= 4;

  }
  while (calc >= 3 && calc < 4) {
    lcd.setCursor(cursor_x, cursor_y);
    lcd.write((byte)2);
    calc -= 3;
  }
  while (calc >= 2 && calc < 3) {
    lcd.setCursor(cursor_x, cursor_y);
    lcd.write((byte)1);
    calc -= 2;
  }
  while (calc >= 1 && calc < 2) {
    lcd.setCursor(cursor_x, cursor_y);
    lcd.write((byte)0);
    calc -= 1;
  }

}

const byte percentage_1[8] PROGMEM = { B10000, B10000, B10000, B10000, B10000, B10000, B10000, B10000 };
const byte percentage_2[8] PROGMEM = { B11000, B11000, B11000, B11000, B11000, B11000, B11000, B11000 };
const byte percentage_3[8] PROGMEM = { B11100, B11100, B11100, B11100, B11100, B11100, B11100, B11100 };
const byte percentage_4[8] PROGMEM = { B11110, B11110, B11110, B11110, B11110, B11110, B11110, B11110 };
const byte percentage_5[8] PROGMEM = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };



void setup()
{
  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  Serial.println(F("Init LCD"));
  lcd.init();                      // initialize the lcd
  lcd.backlight();


  lcd.print("Warming Up...");

  lcd.createChar(0, percentage_1);
  lcd.createChar(1, percentage_2);
  lcd.createChar(2, percentage_3);
  lcd.createChar(3, percentage_4);
  lcd.createChar(4, percentage_5);

  lcd.begin(16, 2);
  for (int i = 0; i <= 100; i++) {
    lcd.setCursor(0, 0);
    lcd.print("Loading :");
    lcd.print(i);
    lcd.print("%");
    lcd_percentage(i, 0, 16, 1);
    delay(10);
  }

}

const String answers[5] PROGMEM = {"Yellow", "Purple", "Green", "Red", "Blue"};
void loop()
{

  while (puzzle < 5) {

    boolean working = true;
    String answer = answers[puzzle];
    Serial.println(F("NOW WORKING"));
    lcd.clear();
    lcd.print(F("PLACE BALLS HERE"));

    while (working) {


      }

      if (color.equalsIgnoreCase(answer)) {
        puzzle--;
        lcd.clear();
        thumbsup();

        switch (type) {

          case 1:
            lcd.print(F("N T"));
            break;
          case 2:
            lcd.print(F("E R"));
            break;
          case 3:
            lcd.print(F("D O"));
            break;
          case 4:
            lcd.print(F("N O"));
            break;
          case 5:
            lcd.print(F("T E"));
            break;
        }

      } else if (got) {

        lcd.clear();
        lcd.print(F("NEED MORE MATERIALS"));
        delay(1000);


      }
    }
    lcd.clear();
    lcd.print(F("SEQUENCE COMPLETE"));
    Serial.println(F("Completed"));
    delay(1000);
    digitalWrite(A0, HIGH);

  }

}
