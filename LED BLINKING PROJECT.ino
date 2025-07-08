#include <LiquidCrystal.h>

const int PIR = 2;
const int TRIG = 7;
const int ECHO = 8;
const int GAS_SENSOR = A2;
const int TEMP_SENSOR = A4;
const int PHOTO_SENSOR = A5;

const int LED_PIR = 4;
const int LED_TEMP = A0;
const int LED_DIST = A3;
const int LED_GAS = 3;
const int LED_PHOTO = A1;
const int BUZZER = 5;

const int TEMP_HIGH = 100;
const int TEMP_LOW = 10;
const int GAS_ALERT = 300;
const int CLOSE_DISTANCE = 30;

unsigned long motionTime = 0;

LiquidCrystal lcd(13, 12, 11, 10, 9, 6);

void setup() {
  pinMode(PIR, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(LED_PIR, OUTPUT);
  pinMode(LED_TEMP, OUTPUT);
  pinMode(LED_DIST, OUTPUT);
  pinMode(LED_GAS, OUTPUT);
  pinMode(LED_PHOTO, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  analogWrite(LED_PHOTO, 0);

  lcd.begin(16, 2);
  lcd.print("System Ready");
  delay(1500);
  lcd.clear();
}

void loop() {
  int lightLevel = analogRead(PHOTO_SENSOR);
  bool motion = digitalRead(PIR);
  long distance = readDistance();
  int gas = analogRead(GAS_SENSOR);
  int temp = analogRead(TEMP_SENSOR);

  if (motion) motionTime = millis();

  updateLEDs(motion, distance, gas, temp, lightLevel);
  updateLCD(motion, distance, gas, temp, lightLevel);

  delay(100);
}

void updateLEDs(bool motion, long distance, int gas, int temp, int light) {
  
  analogWrite(LED_PHOTO, map(light, 0, 1023, 0, 255));

  if (millis() - motionTime < 2000) {
    blinkLED(LED_PIR, 150);
  } else {
    digitalWrite(LED_PIR, LOW);
  }
  if (distance < CLOSE_DISTANCE && distance > 0) {
    int rate = map(distance, 5, CLOSE_DISTANCE, 100, 600);
    blinkLED(LED_DIST, constrain(rate, 100, 600));
    digitalWrite(BUZZER, (millis() % rate) < (rate / 2));
  } else {
    digitalWrite(LED_DIST, LOW);
    digitalWrite(BUZZER, LOW);
  }

  if (gas > GAS_ALERT) {
    int rate = map(gas, GAS_ALERT, 800, 600, 100);
    blinkLED(LED_GAS, constrain(rate, 100, 600));
  } else {
    digitalWrite(LED_GAS, LOW);
  }

  if (temp > TEMP_HIGH) {
    int rate = map(temp, TEMP_HIGH, 800, 600, 100);
    blinkLED(LED_TEMP, constrain(rate, 100, 600));
  } else if (temp < TEMP_LOW) {
    int rate = map(temp, 0, TEMP_LOW, 100, 600);
    blinkLED(LED_TEMP, constrain(rate, 100, 600));
  } else {
    digitalWrite(LED_TEMP, LOW);
  }
}

void blinkLED(int pin, int rate) {
  bool state = (millis() % rate) < (rate / 2);
  digitalWrite(pin, state);
}

void updateLCD(bool motion, long distance, int gas, int temp, int light) {
  static unsigned long lastChange = 0;
  static int displayState = 0;
  static bool alertActive = false;

  if (gas > GAS_ALERT || temp > TEMP_HIGH || temp < TEMP_LOW || (distance < CLOSE_DISTANCE && distance > 0)) {
    alertActive = true;
    lcd.clear();
    lcd.setCursor(0, 0);

    if (gas > GAS_ALERT) {
      lcd.print("GAS LEAKED!");
    } else if (temp > TEMP_HIGH) {
      lcd.print("HIGH TEMPERATURE!");
    } else if (temp < TEMP_LOW) {
      lcd.print("LOW TEMPERATURE!");
    } else {
      lcd.print("OBJECT TOO CLOSE!");
    }
    return;
  }
 
  if (millis() - motionTime < 2000) { 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MOTION DETECTED!");
    lcd.setCursor(0, 1);
    lcd.print("ALERT ACTIVE!");
    return;
  }

  if (alertActive) {
    alertActive = false;
    lcd.clear();
    lastChange = millis();
  }

  if (millis() - lastChange > 3000) {
    lastChange = millis();
    displayState = (displayState + 1) % 3;
    lcd.clear();
  }

  switch (displayState) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("Motion: ");
      lcd.print(motion ? "YES" : "NO ");
      lcd.setCursor(0, 1);
      lcd.print("Light: ");
      lcd.print(map(light, 0, 1023, 0, 100));
      lcd.print("%");
      break;

    case 1:
      lcd.setCursor(0, 0);
      lcd.print("Distance: ");
      lcd.print(distance);
      lcd.print("cm");
      lcd.setCursor(0, 1);
      lcd.print("Gas: ");
      lcd.print(gas);
      break;

    case 2:
      lcd.setCursor(0, 0);
      lcd.print("Temperature: ");
      lcd.print(temp);
      lcd.setCursor(0, 1);
      if (temp < TEMP_LOW) {
        lcd.print("STATUS: LOW TEMP");
      } else if (temp > TEMP_HIGH) {
        lcd.print("STATUS: HIGH TEMP");
      } else {
        lcd.print("STATUS: NORMAL");
      }
      break;
  }
}

long readDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH, 30000);
  if (duration == 0) return 9999; 
  return duration * 0.034 / 2;
}

