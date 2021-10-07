// Arduino Project - stemkraf
// https://github.com/teaksoon/stemkraf
//
// Program: sk_ssd1306oled_animate.ino
// SSD1306OLED - animate
// by TeakSoon Ding for STEMKRAF ( Oct-2021 )
// ----------------------------------------------------------------------
//
#include <lib_c_ssd1306avr.h>
// https://github.com/teaksoon/lib_c_ssd1306avr
//
#define BUZZER_PIN 8 

char bitmap_8bit_u[14] = {
  0b11000000,
  0b11100000,
  0b10110000,
  0b10011000,
  0b10001100,
  0b11000110,
  0b11111111,
  0b11111111,
  0b11000110,
  0b10001100,
  0b10011000,
  0b10110000,
  0b11100000,
  0b11000000,  
};
char bitmap_8bit_d[14] = {
  0b00000011,
  0b00000111,
  0b00001101,
  0b00011001,
  0b00110001,
  0b01100011,
  0b11111111,
  0b11111111,
  0b01100011,
  0b00110001,
  0b00011001,
  0b00001101,
  0b00000111,
  0b00000011,  
};
// Turn this bitmap array 90degrees anti-clockwise, that is how the bitmap will look like

char bitmap_8bit_clear[14] = {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

int8_t col_u,col_d,movedir,movesteps;

void setup() {  
  pinMode(BUZZER_PIN, OUTPUT);    
  digitalWrite(BUZZER_PIN, HIGH);g_pauseMillis(300);digitalWrite(BUZZER_PIN, LOW);

  cssd1306avr_begin();

  // Middle of the 128pixel OLED Screen for 14pixel bitmap = (128/2) - (14/2)
  col_u     = 57;
  col_d     = 57;
  cssd1306avr_showString(1,5,"SSD1306 OLED");
  cssd1306avr_showString(3,5,"I2C 64x128 pixel");  
  cssd1306avr_showBitmap(5,col_u,bitmap_8bit_u,14);  
  cssd1306avr_showBitmap(6,col_d,bitmap_8bit_d,14);    
  g_pauseMillis(2000);
  
  cssd1306avr_clearScreen();
  
  cssd1306avr_showString(0,53,"OLED");
  cssd1306avr_showLargeString(2,40,"OLED");
  cssd1306avr_showChar(0,0,   '0');
  cssd1306avr_showChar(7,0,   '7'); 
  cssd1306avr_showChar(0,122, 'A');
  cssd1306avr_showChar(7,122, 'B');  

  // 28 total steps distance
  movesteps = 28/2; // start from middle
  movedir   = 1;
  cssd1306avr_showBitmap(5,col_u,bitmap_8bit_u,14);
  cssd1306avr_showBitmap(6,col_d,bitmap_8bit_d,14);
}

void loop() {  

  cssd1306avr_showBitmap(5,col_u,bitmap_8bit_clear,14);  
  cssd1306avr_showBitmap(6,col_d,bitmap_8bit_clear,14);
  if ( movesteps < 28 ) {
    col_u = col_u-(4*movedir);
    col_d = col_d+(4*movedir);
    movesteps++;
  } else {
    movesteps=0;
    movedir = -movedir; // reverse movement
  }
  cssd1306avr_showBitmap(5,col_u,bitmap_8bit_u,14);  
  cssd1306avr_showBitmap(6,col_d,bitmap_8bit_d,14); 
  
  g_pauseMillis(80);
}

void g_pauseMillis(unsigned long nn ) {
unsigned long storedMillis = millis();
  while( !( (millis()-storedMillis)> nn) ) { }  
}
