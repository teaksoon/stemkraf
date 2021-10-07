// Arduino Project - stemkraf
// https://github.com/teaksoon/project_stemkraf
//
// Program: sk_ssd1306oled_helloworld.ino
// SSD1306OLED - Hello World
// by TeakSoon Ding for STEMKRAF ( Oct-2021 )
// ----------------------------------------------------------------------
//
#include <lib_c_ssd1306avr.h>
//
void g_pauseMillis(unsigned long nn );

#define BUZZER_PIN 8 

char bitmap_8bit_u[14] = {
  0b10000000,
  0b10000000,
  0b11110000,
  0b10001000,
  0b10001000,
  0b10001000,
  0b11111111,
  0b11111111,  
  0b10001000,
  0b10001000,    
  0b10001000,
  0b11110000,
  0b10000000,
  0b10000000
};

char bitmap_8bit_d[14] = {
  0b00000001,
  0b00000001,
  0b00001111,
  0b00010001,
  0b00010001,
  0b00010001,
  0b11111111,
  0b11111111,
  0b00010001,  
  0b00010001,  
  0b00010001,
  0b00001111,
  0b00000001,
  0b00000001
};
// Turn this bitmap array 90degrees anti-clockwise, that is how the bitmap will look like

void setup() {  
  pinMode(BUZZER_PIN, OUTPUT);    
  digitalWrite(BUZZER_PIN, HIGH);g_pauseMillis(300);digitalWrite(BUZZER_PIN, LOW);
  cssd1306avr_begin();
  cssd1306avr_showString(0,0,"OLED 012345689");
  cssd1306avr_showBitmap(3,57,bitmap_8bit_u,14);  
  cssd1306avr_showBitmap(4,57,bitmap_8bit_d,14);    
}

void loop() {  
}

void g_pauseMillis(unsigned long nn ) {
unsigned long storedMillis = millis();
  while( !( (millis()-storedMillis)> nn) ) { }  
}
