// STEMKRAF - ARDUINO
// https://github.com/teaksoon/stemkraf
//
// Program: libsk_c_oled.h
//        : LIBSK OLED Library "C" Version
//        :
//        : by TeakSoon Ding for STEMKRAF ( OCT-2021 ) R1
// ------------------------------------------------------
/*
The LIBSK OLED Library is targetted specially for the the SSD1306 OLED i2c 64x128 pixel Display Module, 
to be used with the atmega328 micro-controller ( on Arduino Uno, Nano and etc ). The main aim of making
this library is to have a very small and simple OLED Library. Many error corrections are not coded
intentionally, in order to keep this library as small as posssible. Use this library within its limits.

There are 2 version of this library, "C" Version and "G" Version. Following are the
functions available for both Versions ( Text based )
1. sk_oled_begin() - Run this just once, before other library display functions
2. sk_oled_clearScreen() - Clears the entire OLED screen
3. sk_oled_showBitmap(_row,_col,_a_byt,_len,_len_idx) - display 8-bit bitmap (1-128 pixel)
4. sk_oled_showChar(_row,_col,_cha) - Show a ASCII Char ( 32 to 126 )
5. sk_oled_showString(_row,_col,_str) - Show a String ( array of Char terminated by NULL )
6. sk_oled_showChar_L(_row,_col,_cha) - Same as "_showChar()" but doubled in size
7. sk_oled_showString_L(_row,_col,_str) - Same as "_showString()" but doubled in size
_row - (0 to 8 - top to bottom ), for Char and Bitmap
_row - (0 to 7 - top to bottom ), for "Char_L" ( each "Char_L" also uses 2 rows )
_col - (0 to 127 pixel - left to right), for Bitmap ( each Byte of Bitmap is 1 pixel )
_col - (0 to 123 pixel - left to right), for Char ( each Char is 5-pixel ) 
_col - (0 to 118 pixel - left to right), for "Char_L" ( each "Char_L" is 10-pixel ) 
_cha     - ASCII Char ( 32 to 126 )
_str     - Array of Chars, terminated by NULL ( 1-pixel gap between the Chars, 2-pixel gap between the "Char_L" )
_a_byt   - Array of Byte ( 8-bits each ) - for _showBitmap()
_len     - Total elements in _a_byt to be used - for _showBitmap()
_len_idx - Starting index from _a_byt ( 0 to _len-1 ) - for _showBitmap()

The following are functions for the "G" Version Only ( Pixel Based Graphics )
1. sk_oled_showDot(_row,_col,_state ) - display a single dot on the screen
2. sk_oled_showHLine(_row,_col,_len,_state) - Draw Horizontal Line
3. sk_oled_showVLine(_row,_col,_len,_state) - Draw Vertical Line
4. sk_oled_showCircle(_row,_col,__radius,_state) - Draw a Circle
5. sk_oled_showLine(_row_1,_row_2,_col_1,_col_2,_state) - Draw a Diagonal Line
6. sk_oled_showRadialSpokes(_row,_col,_len,_len_idx,_step,__step_idx,_state) - Draw Radial Spokes
_state    - 1 for ON, 0 for OFF
_row      - ( 0 to 63 pixel, top to bottom )
_col      - ( 0 to 127 pixel, left to right )
_len      - Number of pixel ( Length )
_radius   - Number of Pixel ( Diameter / 2 ) - for showCircle()
_len_idx  - Starting index from _len - for showRadialSpokes()
_step     - Total number of evenly distributed spokes in a "Circle" - for showRadialSpokes()
_step_idx - Index of step ( 0 to _step-1 ), step_idx=0 is vertical top from center in clockwise movement

---

How to install this library into Arduino IDE Software ?

1. Find [Arduino User Program Folder] from the Arduino IDE Software, 
"File | Preferences | Sketchbook Location: [Arduino User Program Folder]"

2. Use the Computer File Browser, 
Look for an existing folder named "libraries" inside the [Arduino User Program Folder]

3. Inside the "libraries" folder, 
Create a new folder named "libsk_oled"

4. Copy "libsk_c_oled.h" into the newly created "libsk_oled" folder
[Arduino User Program Folder] / libraries / libsk_oled / libsk_c_oled.h

5. Copy "libsk_g_oled.h" into the newly created "libsk_oled" folder
[Arduino User Program Folder] / libraries / libsk_oled / libsk_g_oled.h

Installation DONE!!!

---

After installation, you may want to try out this two small test program for both versions of the Library

"C" Version 
------
#include <libsk_c_oled.h> 
void setup() {
  sk_oled_begin(); // Run this just once
  sk_oled_showChar(3,61,'H'); // Send a single ASCII Char to OLED screen
}
void loop(){}
------
- Sketch uses 1302 bytes (4%) of program storage space. Maximum is 32256 bytes.
- Global variables use 35 bytes (1%) of dynamic memory, leaving 2013 bytes for local variables. Maximum is 2048 bytes.


"G" Version 
------
#include <libsk_g_oled.h> 
void setup() { 
  sk_oled_begin(); // Run this just once
  sk_oled_showChar(3,61,'H'); // Send a single ASCII Char to OLED screen ( Text Mode )
. sk_oled_showDot(16,64); // Send a Dot OLED screen ( Graphics Mode )
}
void loop(){}
------
- Sketch uses 1374 bytes (4%) of program storage space. Maximum is 32256 bytes.
- Global variables use 1059 bytes (51%) of dynamic memory, leaving 989 bytes for local variables. Maximum is 2048 bytes.


There are two Arduino example program for this library,

1. "libsk_c_oled_example.ino" - demonstrates all the functions in the "C" Version
2. "libsk_g_oled_example.ino" - demonstrates all the functions in the "G" Version


NOTE: 
The "C" version have smaller memory footprint as it does not need to use a video buffer of 1024 bytes to handle
pixel based rendering. 

You cannot use both"C" and "G" version at the same time because both versions are using the same function names. 
Having same function names, allows us to swap between the two versions by just changing the include header. 

My suggestion, is to use the "C" version first, by using #include <libsk_c_oled.h> and when you need to use functions
from the "G" Version, simply change to #include <libsk_g_oled.h> and there is no need to change anything else.

*/
#ifndef LIBSK_C_OLED_H
    #define LIBSK_C_OLED_H
    
#include <avr/io.h>
#include <avr/pgmspace.h> // use Program memory to store font array

#define ADDR_I2C_R  0x79 // i2c Read Address, NOT USED
#define ADDR_I2C_W  0x78 // i2c Write Address(SSD1306OLED is a write only device)
#define ADDR_PAGE   0x22 // rows update - ( 0 to 7 ) each is 8-pixel vertical each=64 pixel (8x8)
#define ADDR_COL    0x21 // cols update - ( 0 to 127) each is pixel=128pixel
#define ADDR_DATA   0x40 // Address to start data transmission
#define ADDR_COMM   0x00 // Address to start Command
//
// --- start Function Declaration ---
//
void _sk_oled_twi_init();
void _sk_oled_twi_start(); 
void _sk_oled_twi_stop();
void _sk_oled_twi_write(char _cha);
char _sk_oled_twi_read();
void _sk_oled_twi_startComm(); 
void _sk_oled_twi_startData(); 
void _sk_oled_setRowCol(uint8_t _row,uint8_t _col);
void _sk_oled_showCharSpace(uint8_t _row, uint8_t _col, uint8_t _len);
void sk_oled_begin(); 
void sk_oled_clearScreen(); 
void sk_oled_showBitmap(uint8_t _row,uint8_t _col,uint8_t *_a_byt,uint8_t _len,uint8_t _len_idx);
void sk_oled_showChar(uint8_t _row,uint8_t _col,char _cha);
void sk_oled_showString(uint8_t _row,uint8_t _col,char *_str);
void sk_oled_showChar_L(uint8_t _row,uint8_t _col,char _cha);
void sk_oled_showString_L(uint8_t _row,uint8_t _col,char *_str);
void __skdebug_oled_mem_dump();
// --- end Function Declaration ---

// --- start Global Vars ---
//
const uint8_t _gsk_oled_font[] PROGMEM = {  // FONT ARRAY
 0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,  /* space (ascii 32) */
 0b00000000,0b00000000,0b01011111,0b00000000,0b00000000,  /* ! */
 0b00000000,0b00000111,0b00000000,0b00000111,0b00000000,  /* " */
 0b00010100,0b01111111,0b00010100,0b01111111,0b00010100,  /* # */
 0b00100100,0b00101010,0b01111111,0b00101010,0b00010010,  /* $ */
 0b00100011,0b00010011,0b00001000,0b01100100,0b01100010,  /* % */
 0b00110110,0b01001001,0b01010101,0b00100010,0b01010000,  /* & */
 0b00000000,0b00000101,0b00000011,0b00000000,0b00000000,  /* ' */
 0b00000000,0b00011100,0b00100010,0b01000001,0b00000000,  /* ( */
 0b00000000,0b01000001,0b00100010,0b00011100,0b00000000,  /* ) */
 0b00010100,0b00001000,0b00111110,0b00001000,0b00010100,  /* * */
 0b00001000,0b00001000,0b00111110,0b00001000,0b00001000,  /* + */
 0b00000000,0b01010000,0b00110000,0b00000000,0b00000000,  /* , */
 0b00001000,0b00001000,0b00001000,0b00001000,0b00001000,  /* - */
 0b00000000,0b01100000,0b01100000,0b00000000,0b00000000,  /* .  */
 0b00100000,0b00010000,0b00001000,0b00000100,0b00000010,  /* / */
 0b00111110,0b01010001,0b01001001,0b01000101,0b00111110,  /* 0 (ascii 48) */
 0b00000000,0b01000010,0b01111111,0b01000000,0b00000000,  /* 1 */
 0b01000010,0b01100001,0b01010001,0b01001001,0b01000110,  /* 2 */
 0b00100001,0b01000001,0b01000101,0b01001011,0b00110001,  /* 3 */
 0b00011000,0b00010100,0b00010010,0b01111111,0b00010000,  /* 4 */
 0b00100111,0b01000101,0b01000101,0b01000101,0b00111001,  /* 5 */
 0b00111100,0b01001010,0b01001001,0b01001001,0b00110000,  /* 6 */
 0b00000001,0b01110001,0b00001001,0b00000101,0b00000011,  /* 7 */
 0b00110110,0b01001001,0b01001001,0b01001001,0b00110110,  /* 8 */
 0b00000110,0b01001001,0b01001001,0b00101001,0b00011110,  /* 9 (ascii 57) */
 0b00000000,0b00110110,0b00110110,0b00000000,0b00000000,  /* : */
 0b00000000,0b01010110,0b00110110,0b00000000,0b00000000,  /* ; */
 0b00001000,0b00010100,0b00100010,0b01000001,0b00000000,  /* < */
 0b00100100,0b00100100,0b00100100,0b00100100,0b00100100,  /* = */
 0b00000000,0b01000001,0b00100010,0b00010100,0b00001000,  /* > */
 0b00000010,0b00000001,0b01010001,0b00001001,0b00000110,  /* ?  */
 0b00110010,0b01001001,0b01111001,0b01000001,0b00111110,  /* @  */
 0b01111110,0b00001001,0b00001001,0b00001001,0b01111110,  /* A (ascii 65) */
 0b01111111,0b01001001,0b01001001,0b01001001,0b00110110,  /* B */
 0b00111110,0b01000001,0b01000001,0b01000001,0b00100010,  /* C */
 0b01111111,0b01000001,0b01000001,0b00100010,0b00011100,  /* D */
 0b01111111,0b01001001,0b01001001,0b01001001,0b01000001,  /* E */
 0b01111111,0b00001001,0b00001001,0b00001001,0b00000001,  /* F */
 0b00111110,0b01000001,0b01001001,0b01001001,0b01111010,  /* G */
 0b01111111,0b00001000,0b00001000,0b00001000,0b01111111,  /* H */
 0b00000000,0b01000001,0b01111111,0b01000001,0b00000000,  /* I */
 0b00100000,0b01000000,0b01000001,0b00111111,0b00000001,  /* J */
 0b01111111,0b00001000,0b00010100,0b00100010,0b01000001,  /* K */
 0b01111111,0b01000000,0b01000000,0b01000000,0b01000000,  /* L */
 0b01111111,0b00000010,0b00001100,0b00000010,0b01111111,  /* M */
 0b01111111,0b00000100,0b00001000,0b00010000,0b01111111,  /* N */
 0b00111110,0b01000001,0b01000001,0b01000001,0b00111110,  /* O */
 0b01111111,0b00001001,0b00001001,0b00001001,0b00000110,  /* P */
 0b00111110,0b01000001,0b01010001,0b00100001,0b01011110,  /* Q */
 0b01111111,0b00001001,0b00011001,0b00101001,0b01000110,  /* R */
 0b01000110,0b01001001,0b01001001,0b01001001,0b00110001,  /* S */
 0b00000001,0b00000001,0b01111111,0b00000001,0b00000001,  /* T */
 0b00111111,0b01000000,0b01000000,0b01000000,0b00111111,  /* U */ 
 0b00011111,0b00100000,0b01000000,0b00100000,0b00011111,  /* V */
 0b00111111,0b01000000,0b00111000,0b01000000,0b00111111,  /* W */
 0b01100011,0b00010100,0b00001000,0b00010100,0b01100011,  /* X */
 0b00000111,0b00001000,0b01110000,0b00001000,0b00000111,  /* Y */
 0b01100001,0b01010001,0b01001001,0b01000101,0b01000011,  /* Z */
 0b00000000,0b01111111,0b01000001,0b01000001,0b00000000,  /* [ */
 0b00000011,0b00000110,0b00011100,0b00110000,0b01100000,  /* backslash */
 0b00000000,0b01000001,0b01000001,0b01111111,0b00000000,  /* ] */
 0b00000100,0b00000010,0b00000001,0b00000010,0b00000100,  /* ^ */
 0b01000000,0b01000000,0b01000000,0b01000000,0b01000000,  /* _ */
 0b00000000,0b00000001,0b00000010,0b00000100,0b00000000,  /* ` */  
 0b00100000,0b01010100,0b01010100,0b01010100,0b01111000,  /* a (ascii 97) */
 0b01111111,0b01001000,0b01000100,0b01000100,0b00111000,  /* b */
 0b00111000,0b01000100,0b01000100,0b01000100,0b00000000,  /* c */
 0b00111000,0b01000100,0b01000100,0b01001000,0b01111111,  /* d */
 0b00111000,0b01010100,0b01010100,0b01010100,0b00011000,  /* e */
 0b00001000,0b01111110,0b00001001,0b00000001,0b00000010,  /* f */
 0b00001100,0b01010010,0b01010010,0b01010010,0b00111110,  /* g */
 0b01111111,0b00001000,0b00000100,0b00000100,0b01111000,  /* h */
 0b00000000,0b01000100,0b01111101,0b01000000,0b00000000,  /* i */
 0b00100000,0b01000000,0b01000100,0b00111101,0b00000000,  /* j */
 0b01111111,0b00010000,0b00101000,0b01000100,0b00000000,  /* k */
 0b00000000,0b00000001,0b01111111,0b01000000,0b00000000,  /* l */
 0b01111100,0b00000100,0b00011000,0b00000100,0b01111000,  /* m */
 0b01111100,0b00001000,0b00000100,0b00000100,0b01111000,  /* n */
 0b00111000,0b01000100,0b01000100,0b01000100,0b00111000,  /* o */
 0b01111100,0b00010100,0b00010100,0b00010100,0b00001000,  /* p */
 0b00001000,0b00010100,0b00010100,0b00011000,0b01111100,  /* q */
 0b01111100,0b00001000,0b00000100,0b00000100,0b00001000,  /* r */
 0b01001000,0b01010100,0b01010100,0b01010100,0b00100000,  /* s */
 0b00000100,0b00111111,0b01000100,0b01000000,0b00100000,  /* t */
 0b00111100,0b01000000,0b01000000,0b00100000,0b01111100,  /* u */
 0b00011100,0b00100000,0b01000000,0b00100000,0b00011100,  /* v */
 0b00111100,0b01000000,0b00110000,0b01000000,0b00111100,  /* w */
 0b01000100,0b00101000,0b00010000,0b00101000,0b01000100,  /* x */
 0b00001100,0b01010000,0b01010000,0b01010000,0b00111100,  /* y */
 0b01000100,0b01100100,0b01010100,0b01001100,0b01000100,  /* z (ascii 122) */
 0b00000000,0b00001000,0b00110110,0b01000001,0b00000000,  /* { */
 0b00000000,0b00000000,0b01111111,0b00000000,0b00000000,  /* | */
 0b00000000,0b01000001,0b00110110,0b00001000,0b00000000,  /* } */
 0b00010000,0b00001000,0b00001000,0b00010000,0b00001000   /* ~ (ascii 126) */
};
// --- end Global Vars ---

// --- start Internal Use Functions ---
void _sk_oled_twi_init() {
  TWSR = 1;TWBR = 0x0C;TWCR = (1<<TWEN);
}
void _sk_oled_twi_start() {
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
  while ((TWCR & (1<<TWINT))==0){}
}
void _sk_oled_twi_stop() {
  TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}
void _sk_oled_twi_write(char _cha) {
  TWDR = _cha;
  TWCR = (1<<TWINT)|(1<<TWEN);
  while((TWCR & (1<<TWINT))==0){}
}
char _sk_oled_twi_read() {
  TWCR = (1 << TWINT) | (1 << TWEN);
  while ((TWCR & (1 << TWINT)) == 0) {}
  return TWDR;
}
void _sk_oled_twi_startComm() { 
  _sk_oled_twi_start();
  _sk_oled_twi_write(ADDR_I2C_W);
  _sk_oled_twi_write(ADDR_COMM);
}
void _sk_oled_twi_startData() {
  _sk_oled_twi_start();
  _sk_oled_twi_write(ADDR_I2C_W);
  _sk_oled_twi_write(ADDR_DATA); 
}
void _sk_oled_setRowCol(uint8_t _row, uint8_t _col ) { 
  _sk_oled_twi_startComm();
  _sk_oled_twi_write(ADDR_PAGE); // Row
  _sk_oled_twi_write(_row);_sk_oled_twi_write(7);
  _sk_oled_twi_write(ADDR_COL);  // Col
  _sk_oled_twi_write(_col);_sk_oled_twi_write(127);  
  _sk_oled_twi_stop();
}
void _sk_oled_showCharSpace(uint8_t _row, uint8_t _col, uint8_t _len) { 
  _sk_oled_setRowCol(_row,_col);  
  _sk_oled_twi_startData();
  for (int i=0; i < _len; i++) { 
    _sk_oled_twi_write(0); 
  } 
  _sk_oled_twi_stop();   
}
// --- end Internal Use Functions ---

// --- start "C" Version  ---
void sk_oled_begin() {
// Intital Setup Values for the SSD1306 I2C OLED 64x128 pixel only
//
uint8_t initData[26]= { 
  0xAE,       // 0xAE Display OFF
  0xD5,0x80,  // 0xD5 Display Clock Div
  0xA8,0x3F,  // 0xA8 Set Multiplex: 0x1F for 128x32, 0x3F for 128x64
  0xD3,0x00,  // 0xD3 Set Display offset 0
  0x40,       // 0x40 Start Line 0
  0x8D,0x14,  // 0x8D Set Charge pump Enabled,internal VCC
  0x20,0x00,  // 0x20 Set Memory Mode: 0x00 for H, 0x01 for V
  0xA1,       // 0xA1 Rotate 180
  0xC8,       // 0xC8 Com Scan Dec
  0xDA,0x12,  // 0xDA Set COM Pins, 0x12 for h=64 or 0x02 for h=32
  0x81,0xCF,  // 0x81 Set Contrast
  0xD9,0xF1,  // 0xD9 Set pre-charge period
  0xDB,0x40,  // 0xDB Set vcom detect
  0xA4,       // 0xA4 all pixel 0N = A4, all pixel OFF=A5
  0xA6,       // 0xA6 Normal Display
  0x2E,       // 0x2E No Scroll
  0xAF        // 0xAF Display on
  };
  
  _sk_oled_twi_init(); // set up AVR twi for I2C com
  for (uint8_t i=0; i < 26; i++) {
    _sk_oled_twi_startComm();    
    _sk_oled_twi_write(initData[i]);
    _sk_oled_twi_stop();    
  } 
  sk_oled_clearScreen(); // Clear screen and ready for use
}

void sk_oled_clearScreen() {
  _sk_oled_setRowCol(0,0); 
  _sk_oled_twi_startData(); 
  for (uint8_t r=0; r < 8; r++) {
    for (uint8_t c=0; c < 128; c++) {
      _sk_oled_twi_write(0);
    }
  } 
  _sk_oled_twi_stop();
  _sk_oled_setRowCol(0,0);
}

void sk_oled_showBitmap(uint8_t _row, uint8_t _col, char *_a_byt, uint8_t _len, uint8_t _len_idx) {
  _sk_oled_setRowCol(_row,_col);
  _sk_oled_twi_startData();
  for (uint8_t i=0; i < _len; i++) {
    _sk_oled_twi_write(_a_byt[i+_len_idx]);
  }
  _sk_oled_twi_stop();
}

void sk_oled_showChar(uint8_t _row, uint8_t _col, char _cha) {
uint8_t byt;  
  
  if (_cha < 32 || _cha > 126 ) return; // Ascii 32 to 126 only, Ignore the rest
  
  _sk_oled_setRowCol(_row,_col);  
  _sk_oled_twi_startData();
  for (int i=0; i < 5; i++) {  // EACH FONT is 5 byte in FONT ARRAY
    byt = pgm_read_byte( &_gsk_oled_font[((_cha-32)*5)+i] ); // read from PROGRAM MEMORY
    _sk_oled_twi_write(byt); 
  } 
  _sk_oled_twi_stop();   
}

void sk_oled_showString(uint8_t _row, uint8_t _col, char *_str) {
uint8_t idx = 0;
  while (idx < 21) { // Maximum 21 chars, index 0 to 20
    sk_oled_showChar(_row,_col,_str[idx]); _col +=5;      
    idx++;
    if (_str[idx] == 0 || idx == 21) { 
      // Next Char is a NULL or Maximum Chars breached
      // Do not add Space to the Last Char in the String
      break; 
    } else {
      _sk_oled_showCharSpace(_row,_col,1); _col+=1;  
    }
  }
}

void sk_oled_showChar_L(uint8_t _row, uint8_t _col, char _cha) {
char cTmp;  
char enlargeMask[]= {
  0b00000000,0b00000011,0b00001100,0b00001111,0b00110000,0b00110011,0b00111100,0b00111111,
  0b11000000,0b11000011,0b11001100,0b11001111,0b11110000,0b11110011,0b11111100,0b11111111
  };
  
  if (_cha < 32 || _cha > 126 ) return; // Ascii 32 to 126 only, Ignore the rest

  // Double Size, Top Part
  _sk_oled_setRowCol(_row,_col); 
  _sk_oled_twi_startData();
  for (uint8_t i=0; i < 5; i++) {   
    cTmp = pgm_read_byte(&_gsk_oled_font[((_cha-32)*5)+i]);
    cTmp = cTmp & 0x0F;       // store LOW BYTE
    cTmp = enlargeMask[cTmp]; // mapped into Enlargement Mask
    _sk_oled_twi_write(cTmp); // write to OLED
    _sk_oled_twi_write(cTmp); // same thing, to double size
  }
  _sk_oled_twi_stop(); 
 
  // Double Size, Bottom Part        
  _sk_oled_setRowCol(_row+1,_col); 
  _sk_oled_twi_startData(); 
  for (uint8_t i=0; i < 5; i++) {
    cTmp = pgm_read_byte(&_gsk_oled_font[((_cha-32)*5)+i]);
    cTmp = (cTmp&0xF0) >> 4 ; // store HIGH BYTE
    cTmp = enlargeMask[cTmp]; // mapped into Enlargement Mask
    _sk_oled_twi_write(cTmp); // write to OLED
    _sk_oled_twi_write(cTmp); // same thing, to double size      
  } 
  _sk_oled_twi_stop();
}

void sk_oled_showString_L(uint8_t _row, uint8_t _col, char *_str) {
uint8_t idx = 0;
  while (idx < 10) { // Maximum 10 Large Chars, index 0 to 9
    sk_oled_showChar_L(_row,_col,_str[idx]); _col +=10; 
    idx++;
    if (_str[idx] == 0 || idx == 10) { 
      // Next Char is a NULL or Maximum Chars breached
      // Do not add Space to the Last Char in the String
      break; 
    } else {
      _sk_oled_showCharSpace(_row,  _col,2); // Top Row
      _sk_oled_showCharSpace(_row+1,_col,2); // Bottom Row
      _col+=2;       
    }
  }
}
// --- end "C" version ---

#endif
