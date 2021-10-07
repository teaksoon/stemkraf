// Arduino Project - stemkraf
// https://github.com/teaksoon/stemkraf
//
// Program: sk_abuzzer_blink.ino
// Active Buzzer - Blink
// by TeakSoon Ding for STEMKRAF ( Oct-2021 )
// ----------------------------------------------------------------------
//
#define BUZZER_PIN 8

unsigned long blink_millis;
uint8_t buzzer_on;
uint8_t iCounter;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT); 

  digitalWrite(BUZZER_PIN, HIGH);
  g_pauseMillis(500);
  digitalWrite(BUZZER_PIN, LOW);
  
  g_pauseMillis(2000);

  iCounter      = 0;
  buzzer_on     = 1;
  blink_millis  = millis();  
}

void loop() {   
  if (iCounter > 10) return; // will not run anymore after 10 times

  if (millis()-blink_millis > 300) {
    blink_millis = millis();
    if ( buzzer_on == 1) {
      digitalWrite(BUZZER_PIN, LOW);
      buzzer_on = 0;
      iCounter++;      
    } else {
      buzzer_on = 1;
      digitalWrite(BUZZER_PIN, HIGH);
    }
  }
}
void g_pauseMillis(unsigned long nn ) {
unsigned long storedMillis = millis();
  while( !( (millis()-storedMillis)> nn) ) {}  
}
