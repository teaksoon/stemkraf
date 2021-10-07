// Arduino Project - stemkraf
// https://github.com/teaksoon/stemkraf
//
// Arduino Program: sk_menu.ino
// MENU
// by TeakSoon Ding for STEMKRAF ( Oct-2021 )
// ----------------------------------------------------------------------
//
#include <lib_c_ssd1306avr.h>
//
// --- start function declaration ---
//
int g_getMedian(int dataArray[], int8_t dataCount);
void g_pauseMillis(unsigned long nn );
int8_t g_cycle2digit(int8_t iDir, int8_t iNum, int8_t iMin, int8_t iMax);

void dac_menu_selector(int moveDir);
void dac_menu_stop();

void button_ENTER_function();
void button_ESC_function();
void button_LEFT_function();
void button_DOWN_function(); 
void button_RIGHT_function();
void button_UP_function();
void process_digitalButtons();
void process_analogButtons(); 
// --- end function declaration ---

// --- start defines and global variables ---
//
#define BUZZER_PIN        8
#define NAVIGATE_BTN_PIN  A1
#define ENTER_BTN_PIN     7
#define ESC_BTN_PIN       6

// delay in milliseconds for debounce correction
// increase if not sufficient, reduce if button needs speed
#define DEBOUNCE_MS       10  

// Get these Analog number from running the "apg_tactile_analog_raw.ino" program
// Make sure the gap between two buttons is more than 40 when AREF_BUFFER=20
// Use different Resistor Values to adjust the gap
#define AREF_BUFFER       20
#define AREF_LEFT         950 // This button is firs button. We do a -100 to max (1023)
#define AREF_DOWN         495 
#define AREF_RIGHT        335
#define AREF_UP           250

#define MENU_FN_ONE       0
#define MENU_FN_TWO       1
#define MENU_FN_THR       2
#define MENU_FN_FOU       3

#define MODE_MENU         0
#define MODE_FN_ONE       1
#define MODE_FN_TWO       2
#define MODE_FN_THR       3
#define MODE_FN_FOU       4

char btnEnter           = 'N'; // Y ( Digital Pin )
char btnEsc             = 'N'; // Y ( Digital Pin )
char btnNavigate        = 'N'; // L D R U ( Analog Pin )

char selector_bmp[10]       = {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18};
char selector_bmpblank[10]  = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

int8_t active_mode   = MODE_MENU;

int8_t active_menu    = MENU_FN_ONE;
int8_t rc_menu[4][2]  = { {7, 32}, {7, 50}, {7, 68}, {7, 86} }; // selector row, col position
char menu_bmp[64]     = {
  0b11111111,0b10000001,0b11000011,0b10100101,0b10011001,0b10011001,0b10100101,0b11000011,0b10000001,0b11111111, 
  0x0,0x00,0x00,0x00,0x00,0x00,0x00,0x000,
  0b11111111,0b10000001,0b11000011,0b10100101,0b10011001,0b10011001,0b10100101,0b11000011,0b10000001,0b11111111, 
  0x0,0x00,0x00,0x00,0x00,0x00,0x00,0x000,
  0b11111111,0b10000001,0b11000011,0b10100101,0b10011001,0b10011001,0b10100101,0b11000011,0b10000001,0b11111111,  
  0x0,0x00,0x00,0x00,0x00,0x00,0x00,0x000,
  0b11111111,0b10000001,0b11000011,0b10100101,0b10011001,0b10011001,0b10100101,0b11000011,0b10000001,0b11111111 
};
// --- end defines and global variables---

// -- start Arduino Setup/Loop ---
//
void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);g_pauseMillis(300);digitalWrite(BUZZER_PIN, LOW);

  cssd1306avr_begin(); // SSD1306 OLED Display
  
  menu_selector(active_menu); 
  
}

void loop() {  
  process_digitalButtons();
  process_analogButtons();  
}
// --- end Arduino Setup/Loop ---

// --- start Menu ----
//
void menu_selector(int moveDir) {
char tmp[3];

  cssd1306avr_clearScreen();
  cssd1306avr_showBitmap(6, 32, menu_bmp,     64); // Fixed Menu Bar display at bottom 

  // Navigate and display each options
  active_menu = g_cycle2digit(moveDir, active_menu, 0, 3);
  cssd1306avr_showBitmap(rc_menu[active_menu][0], rc_menu[active_menu][1], selector_bmp, 10); 

  // While navigating the Menu options, display function screen
  switch (active_menu) {
    case MENU_FN_ONE:  
      cssd1306avr_showLargeString(2,10,"One");
    break;
    case MENU_FN_TWO:  
      cssd1306avr_showLargeString(2,10,"Two");
    break;
    case MENU_FN_THR:   
      cssd1306avr_showLargeString(2,10,"Three");
    break;
    case MENU_FN_FOU:  
      cssd1306avr_showLargeString(2,10,"Four");  
    break;           
    default:break;
  }
}

void menu_inactivated() {
  cssd1306avr_showBitmap(rc_menu[active_menu][0], rc_menu[active_menu][1], selector_bmpblank, 10);
}
// --- end Menu ---

// --- start Button Processing ---
//_ONE
void button_ENTER_function() {
   switch (active_mode) {  
    case MODE_MENU:
      switch (active_menu) {
        case MENU_FN_ONE:
          cssd1306avr_showBitmap(0,59,  selector_bmp, 10);           
          menu_inactivated(); 
          active_mode = MODE_FN_ONE;          
        break;  
        case MENU_FN_TWO:  
          cssd1306avr_showBitmap(0,59,  selector_bmp, 10);            
          menu_inactivated();           
          active_mode = MODE_FN_TWO;               
        break;            
        case MENU_FN_THR:      
          cssd1306avr_showBitmap(0,59,  selector_bmp, 10);            
          menu_inactivated();          
          active_mode = MODE_FN_THR;            
        break;
        case MENU_FN_FOU:
          cssd1306avr_showBitmap(0,59,  selector_bmp, 10);          
          menu_inactivated();        
          active_mode = MODE_FN_FOU;
        break;           
        default:break;
      }
    break;
    case MODE_FN_ONE:
       active_mode  = MODE_MENU; menu_selector(0); // return to MENU
    break;
    case MODE_FN_TWO:   
       active_mode  = MODE_MENU; menu_selector(0); // return to MENU
    break;
    case MODE_FN_THR:
       active_mode  = MODE_MENU; menu_selector(0); // return to MENU
    break;
    case MODE_FN_FOU:
       active_mode  = MODE_MENU; menu_selector(0); // return to MENU
    break;    
    default:break;
  }
  cssd1306avr_showChar(7,0,'E');  // Debug, to be removed
}

void button_ESC_function() {
  switch (active_mode) {
  case MODE_MENU:
    active_menu=0;
  break;    
  default:break;
  }    
  active_mode = MODE_MENU;
  menu_selector(0); // return to menu
  
  cssd1306avr_showChar(7,0,'X');  // debug, to be removed  
}

void button_LEFT_function() {    
  switch (active_mode) {
    case MODE_MENU:
      menu_selector(-1);
    break;
    default:break;
  };
  cssd1306avr_showChar(7,0,'L'); // debug, to be removed
}
void button_RIGHT_function() {  
  switch (active_mode) {
    case MODE_MENU:  
      menu_selector(1);
    break;    
    default:break;
  };     
  cssd1306avr_showChar(7,0,'R');
}
void button_UP_function() {  
  switch (active_mode) {
    case MODE_MENU:  
      // Not Used in Menu State
    break;    
    default:break;
  };  
  cssd1306avr_showChar(7,0,'U');  // debug, to be removed
}
void button_DOWN_function() { 
  switch (active_mode) {
    case MODE_MENU:  
      // Not used in Menu State
    default:break;
  }; 
  cssd1306avr_showChar(7,0,'D'); 
}

void process_digitalButtons() {
  if (digitalRead(ENTER_BTN_PIN) == HIGH) {
    if (btnEnter == 'N') {    g_pauseMillis(DEBOUNCE_MS);
      btnEnter='Y'; button_ENTER_function();          
    }
  } else {
    if (btnEnter != 'N') btnEnter='N'; 
  }
  
  if (digitalRead(ESC_BTN_PIN) == HIGH) {
    if (btnEsc == 'N') {    g_pauseMillis(DEBOUNCE_MS);
      btnEsc='Y'; button_ESC_function();          
    }
  } else {
    if (btnEsc != 'N') btnEsc='N'; 
  }
}
  
void process_analogButtons() {
int tempData[5];
int pinValue;

  // Get Median value from 5 readings to filter stray values
  for (int8_t i=0; i<5; i++) {
    pinValue = analogRead(NAVIGATE_BTN_PIN); 
    tempData[i] = pinValue;
  }
  pinValue = g_getMedian(tempData, 5);

  if ( pinValue < 6 ) { 
    // The Analog Reading 5 and below are considered as "Button Released"
    // Analog Reading 1023 = 5v
    // Analog Reading 5 = 5*5/1023 ( 0.024 volt and below )
    //
    if (btnNavigate != 'N') btnNavigate='N';
  } else {
    if ( btnNavigate == 'N') {         
      // This is a freshly pressed button, ignore if it was already pressed and held-on
      //
      if ( pinValue > (AREF_LEFT-100) ) { // LEFT - First button. Special case, range it from -100 to max
        if (btnNavigate != 'L') {   g_pauseMillis(DEBOUNCE_MS);
          btnNavigate='L'; button_LEFT_function();              
        }
      } 
      else if ( (pinValue > (AREF_DOWN-AREF_BUFFER)) && (pinValue < (AREF_DOWN+AREF_BUFFER)) ) { // DOWN Button
        if (btnNavigate != 'D') {   g_pauseMillis(DEBOUNCE_MS);
          btnNavigate='D'; button_DOWN_function();                     
        }
      }
      else if ( (pinValue > (AREF_RIGHT-AREF_BUFFER)) && (pinValue < (AREF_RIGHT+AREF_BUFFER)) ) { // RIGHT Button
        if (btnNavigate != 'R') {   g_pauseMillis(DEBOUNCE_MS);
          btnNavigate='R';button_RIGHT_function();                  
        }
      } 
      else if ( (pinValue > (AREF_UP-AREF_BUFFER)) && (pinValue < (AREF_UP+AREF_BUFFER)) ) { // UP Button
        if (btnNavigate != 'U') {     g_pauseMillis(DEBOUNCE_MS);
          btnNavigate='U';button_UP_function();                  
        }
      } else {
        // Out of Range Reading, Ignore
      }
    }     
  }
}
// --- end Button Processing ---

// --- start General Purpose functions ---
//
int8_t g_cycle2digit(int8_t iDir, int8_t iNum, int8_t iMin, int8_t iMax) {
  iNum += iDir;
  if (iNum > iMax) {
    iNum = iMin;  
  } else {
    if ( iNum < iMin) {
      iNum = iMax;
    }
  }
  return iNum;
}

int g_getMedian(int dataArray[], int8_t dataCount) {
int temp;
  for(int8_t x=0;x<dataCount;x++){
    for(int8_t y=x+1;y<dataCount;y++){
      if(dataArray[x]>dataArray[y]){
        temp=dataArray[x];
        dataArray[x]=dataArray[y];
        dataArray[y]=temp;
      }
    }
  }
  return dataArray[int(dataCount/2)]; 
}

void g_pauseMillis(unsigned long nn ) {
unsigned long storedMillis = millis();
  while( !( (millis()-storedMillis)> nn) ) {}
}
// --- end General purpose functions ---
