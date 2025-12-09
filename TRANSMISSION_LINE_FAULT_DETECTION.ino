#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("System Starting");
  delay(1500);
  lcd.clear();
}

void loop() {
  int L1 = analogRead(A0);
  int L2 = analogRead(A1);
  int L3 = analogRead(A2);

  int val3 = analogRead(A4);
  int val4 = analogRead(A5);

  int flameVal = analogRead(A3);

  int flameThreshold = 100;       

  Serial.print("L1: "); Serial.print(L1); Serial.print(" | ");
  Serial.print("L2: "); Serial.print(L2); Serial.print(" | ");
  Serial.print("L3: "); Serial.print(L3); Serial.print(" | ");
  Serial.print("L3p: "); Serial.print(val3); Serial.print(" | ");
  Serial.print("L4p: "); Serial.print(val4); Serial.print(" | ");
  Serial.print("Flame: "); Serial.println(flameVal);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("L1:"); lcd.print(L1);
  lcd.print(" L2:"); lcd.print(L2);
  lcd.setCursor(0,1);
  lcd.print("L3:"); lcd.print(L3);
  lcd.print(" F:"); lcd.print(flameVal);
  delay(500);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("L3p:"); lcd.print(val3);
  lcd.setCursor(0,1);
  lcd.print("L4p:"); lcd.print(val4);
  delay(500);

  if (flameVal < flameThreshold) {
    Serial.println("🔥 FLAME DETECTED!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("!! FIRE ALERT !!");
    lcd.setCursor(0,1);
    lcd.print("Flame Val: ");
    lcd.print(flameVal);
    beepBuzzer();
  }

  bool groundShort = false;

  if (L1 < 30) {
    Serial.println("L1 SHORT CIRCUITED!");
    lcdAlert("L1 SHORT!");
    groundShort = true;
    beepBuzzer();
  }

  if (L2 < 30) {
    Serial.println("L2 SHORT CIRCUITED!");
    lcdAlert("L2 SHORT!");
    groundShort = true;
    beepBuzzer();
  }

  if (L3 < 30) {
    Serial.println("L3 SHORT CIRCUITED!");
    lcdAlert("L3 SHORT!");
    groundShort = true;
    beepBuzzer();
  }

  if (!groundShort) {
    int d12 = abs(L1 - L2);
    int d23 = abs(L2 - L3);
    int d13 = abs(L1 - L3);

    if (d12 > d23 && d12 > d13 && d12 > 10) {
      Serial.println("L1 & L2 SHORT!");
      lcdAlert("L1 & L2 SHORT!");
      beepBuzzer();
    }

    if (d13 > d12 && d13 > d23 && d13 > 10) {
      Serial.println("L1 & L3 SHORT!");
      lcdAlert("L1 & L3 SHORT!");
      beepBuzzer();
    }

    if (d23 > d12 && d23 > d13 && d23 > 10) {
      Serial.println("L2 & L3 SHORT!");
      lcdAlert("L2 & L3 SHORT!");
      beepBuzzer();
    }
  }

  Serial.println();
  delay(1000);
}

void beepBuzzer() {
  tone(6, 1000);
  delay(500);
  noTone(6);
  delay(300);
}

void lcdAlert(String msg) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  FAULT ALERT!");
  lcd.setCursor(0,1);
  lcd.print(msg);
  delay(500);
}
