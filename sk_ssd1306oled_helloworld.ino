// Arduino Project - stemkraf
// https://github.com/teaksoon/stemkraf
//
// Program: sk_ssd1306oled_helloworld.ino
// SSD1306OLED - Hello World
// by TeakSoon Ding for STEMKRAF ( Oct-2021 )
// ----------------------------------------------------------------------
//
#include <lib_c_ssd1306avr.h>
// https://github.com/teaksoon/lib_c_ssd1306avr
//
#define BUZZER_PIN 8 

void setup() {  
  pinMode(BUZZER_PIN, OUTPUT);    
  digitalWrite(BUZZER_PIN, HIGH);g_pauseMillis(300);digitalWrite(BUZZER_PIN, LOW);
  cssd1306avr_begin();
  cssd1306avr_showString(1,5,"SSD1306 OLED");
  cssd1306avr_showString(3,5,"I2C 64x128 pixel");  
}

void loop() {  
}

void g_pauseMillis(unsigned long nn ) {
unsigned long storedMillis = millis();
  while( !( (millis()-storedMillis)> nn) ) { }  
}
