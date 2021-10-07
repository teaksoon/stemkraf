// Arduino Project - stemkraf
// https://github.com/teaksoon/stemkraf
//
// Program: sk_tactile_analog_raw.ino
// Tactile Button - Analog Pin Raw Readings
// by TeakSoon Ding for STEMKRAF ( Oct-2021 )
// ----------------------------------------------------------------------
//
#define BUZZER_PIN          8
#define NAVIGATE_BTN_PIN    A1

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);  
  digitalWrite(BUZZER_PIN, HIGH);g_pauseMillis(200);digitalWrite(BUZZER_PIN, LOW);

  Serial.begin(9600);
  Serial.print("\nSerial Monitor\n");  // Serial Monitor Display
}
void loop() {  
  process_analogButtons();
}

void process_analogButtons() {
  Serial.print("\nanalogRead(A1) = ");
  Serial.print(analogRead(NAVIGATE_BTN_PIN));
}

void g_pauseMillis(unsigned long nn ) {
unsigned long storedMillis = millis();
  while( !( (millis()-storedMillis)> nn) ) {}  
}
