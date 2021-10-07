// Arduino Project - stemkraf
// https://github.com/teaksoon/stemkraf
//
// Program: sk_tactile_digital_raw.ino
// Tactile Button - Digital Pin Raw Readings
// by TeakSoon Ding for STEMKRAF ( Oct-2021 )
// ----------------------------------------------------------------------
//
#define BUZZER_PIN          8
#define ENTER_BTN_PIN       7
#define ESC_BTN_PIN         6

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);g_pauseMillis(200);digitalWrite(BUZZER_PIN, LOW);

  Serial.begin(9600);
  Serial.print("\nSerial Monitor\n"); // Serial Monitor for display
}

void loop() {  
  process_digitalButtons();
}

void process_digitalButtons() {
  Serial.print("\ndigitalRead(7) = ");  Serial.print(digitalRead(ENTER_BTN_PIN));
  Serial.print(", digitalRead(6) = ");  Serial.print(digitalRead(ESC_BTN_PIN));
}

void g_pauseMillis(unsigned long nn ) {
unsigned long storedMillis = millis();
  while( !( (millis()-storedMillis)> nn) ) { }  
}
