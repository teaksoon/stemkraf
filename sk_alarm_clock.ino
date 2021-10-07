// Arduino Project - stemkraf
// https://github.com/teaksoon/stemkraf
//
// Arduino Program: sk_alarm_clock.ino
// Alarm Clock
// by TeakSoon Ding for STEMKRAF ( Oct-2021 )
// ----------------------------------------------------------------------
//
#include <lib_c_ssd1306avr.h>
// https://github.com/teaksoon/lib_c_ssd1306avr
//
// --- start function declaration ---
//
int g_getMedian(int dataArray[], int8_t dataCount);
void g_pauseMillis(unsigned long nn );
char g_isLeapYear(int yyyy);
void g_str2digit(int8_t nn, char bStr[]);
void g_str4digit(int nn, char bStr[]);
int8_t g_cycle2digit(int8_t iDir, int8_t iNum, int8_t iMin, int8_t iMax);
int g_cycle4digit(int8_t iDir, int iNum, int iMin, int iMax);

void sf_turn_off_buzzer();
void sf_show_hhmode(int8_t hhmode, int8_t hh12mode);
void sf_show_hh(int8_t hh);
void sf_show_mm(int8_t mm);
void sf_show_ss(int8_t ss);
void menu_selector(int8_t moveDir);
void menu_inactivated();
void fn_ext_selector(int8_t moveDir);
void fn_ext_show();
void fn_ext_edit(int8_t editDir); 
void fn_clock_selector(int8_t moveDir);
void fn_clock_show();
void fn_clock_edit(int8_t editDir); 
void fn_clock_edit_hhmode();
void fn_clock_edit_hh();
void fn_clock_update();
void fn_clock_update_nextday();
void fn_alarm_selector(int8_t moveDir);
void fn_alarm_show();
void fn_alarm_edit(int8_t editDir); 
void fn_alarm_edit_hh();
void fn_alarm_monitor();
void fn_timer_selector(int8_t moveDir);
void fn_timer_show();
void fn_timer_edit(int8_t editDir); 
void fn_timer_update(); 
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

// Get these Analog number from the "sk_tactile_analog_raw.ino" program
// Make sure the gap between two buttons is more than 40 when AREF_BUFFER=20
// Use different Resistor Values to adjust the gap
#define AREF_BUFFER       20
#define AREF_LEFT         950 // First button. We do a -100 to max (1023)
#define AREF_DOWN         495 
#define AREF_RIGHT        335
#define AREF_UP           250

#define MENU_FN_CLOCK     0
#define MENU_FN_ALARM     1
#define MENU_FN_TIMER     2
#define MENU_FN_EXT       3

#define MODE_MENU         0
#define MODE_FN_CLOCK     1
#define MODE_FN_ALARM     2
#define MODE_FN_TIMER     3
#define MODE_FN_EXT       4

#define HHMODE_24         0
#define HHMODE_12         1
#define HH12MODE_AM       0
#define HH12MODE_PM       1
#define HH12MODE_NA       2
#define ALARM_OFF         0
#define ALARM_ON          1
#define TIMER_STOP        0
#define TIMER_START       1

char selector_bmp[10]       = {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18};
char selector_bmpblank[10]  = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
char str_off_on[2][4]       = {"OFF","ON "};
char str_stop_start[2][6]   = {"STOP ","START"};
char str_dow[7][4]          = {"MON","TUE","WED","THU","FRI","SAT","SUN"};
char str_mo[12][4]          = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};

char btnEnter         = 'N'; // Y ( Digital Pin )
char btnEsc           = 'N'; // Y ( Digital Pin )
char btnNavigate      = 'N'; // L D R U ( Analog Pin )

int8_t active_mode    = MODE_MENU;      // Start with Menu Mode

int8_t active_menu    = MENU_FN_CLOCK;  // First Menu Option, Clock
int8_t rc_menu[4][2]  = { {7, 32}, {7, 50}, {7, 68}, {7, 86} }; // selector row, col position
char menu_bmp[64]     = {
  0b11111111,0b11111111,0b11000011,0b10000001,0b10011001,0b10011001,0b10011001,0b10011001,0b11111111,0b11111111,  
  0x0,0x00,0x00,0x00,0x00,0x00,0x00,0x000,
  0b11111111,0b11111111,0b10000011,0b10000001,0b11101001,0b11101001,0b10000001,0b10000011,0b11111111,0b11111111,  
  0x0,0x00,0x00,0x00,0x00,0x00,0x00,0x000,
  0b11111111,0b11111111,0b11111001,0b11111001,0b10000001,0b10000001,0b11111001,0b11111001,0b11111111,0b11111111,
  0x0,0x00,0x00,0x00,0x00,0x00,0x00,0x000,
  0b11111111,0b10000001,0b11000011,0b10100101,0b10011001,0b10011001,0b10100101,0b11000011,0b10000001,0b11111111 
};

int8_t active_ext     = 0;
int8_t rc_ext[1][2]   = { {3,105} };

int8_t active_clock   = 0;
int8_t rc_clock[8][2] = { {5,110}, {1,  4}, {1, 61}, {1, 80}, {1,104}, {5, 30}, {5, 58}, {5, 86} };
unsigned long millis_clock;
int8_t mo_end[12]     = {31,28,31,30,31,30,31,31,30,31,30,31};
int8_t t_mo_end[12]   = {31,28,31,30,31,30,31,31,30,31,30,31};
int8_t clo_hh         = 0;
int8_t clo_mm         = 0;
int8_t clo_ss         = 0;
int8_t clo_dow        = 1;
int8_t clo_dd         = 1;
int8_t clo_mo         = 1;
int    clo_yyyy       = 2001;
int8_t clo_hhmode     = HHMODE_24;
int8_t clo_hh12mode   = HH12MODE_NA;
int8_t clo_t_hh;
int8_t clo_t_mm;
int8_t clo_t_ss;
int8_t clo_t_dow;
int8_t clo_t_dd;
int8_t clo_t_mo;
int    clo_t_yyyy;
int8_t clo_t_hhmode;
int8_t clo_t_hh12mode;

int8_t active_alarm     = 0;
int8_t rc_alarm[4][2]   = { {5, 30}, {5, 58}, {5, 86}, {2, 92} };
unsigned long millis_alarm_buzzer;
int8_t alarm_buzzer     = 0;
int8_t ala_off_on       = ALARM_OFF;
int8_t ala_hh           = 0;
int8_t ala_mm           = 0;
int8_t ala_ss           = 0;
int8_t ala_hh12mode     = HH12MODE_NA;
int8_t ala_duration_ss  = 3;
int8_t ala_t_off_on;
int8_t ala_t_hh;
int8_t ala_t_mm;
int8_t ala_t_ss;
int8_t ala_t_hh12mode;
int8_t ala_t_duration_ss;

int8_t active_timer   = 0;
int8_t rc_timer[4][2] = { {1,91}, {5, 30}, {5, 58}, {5, 86}  };
unsigned long cti_millis;
int8_t cti_stop_start = TIMER_STOP;
int8_t cti_hh         = 0;
int8_t cti_mm         = 0;
int8_t cti_ss         = 30; // default 30 seconds
// --- end defines and global variables---

// -- start Arduino Setup/Loop ---
//
void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);g_pauseMillis(300);digitalWrite(BUZZER_PIN, LOW);

  cssd1306avr_begin(); // SSD1306 OLED Display
  mo_end[1] = (g_isLeapYear(clo_yyyy)==1)?29:28; // Leap Year Check  
  menu_selector(active_menu);
}

void loop() {  

  // FN_CLOCK_ALARM
  if ( (millis()-millis_clock) >= 1000 ) { 
    millis_clock = millis();

    // Main Clock is updated in every seconds
    fn_clock_update();          

    // Show Running Clock only in Menu Mode at Clock Function Option
    if (active_mode == MODE_MENU && active_menu == MENU_FN_CLOCK) {
      fn_clock_show(); 
    }
    
    // Check Alarm every seconds when it is switched ON
    if (ala_off_on == ALARM_ON) {
      fn_alarm_monitor();
    }   
  } 
  if ( alarm_buzzer == 1) {
    if (millis()-millis_alarm_buzzer >= (ala_duration_ss*1000)) {
      // Alarm buzzer is turned OFF when alarm buzzer duration is completed
      alarm_buzzer = 0; digitalWrite(BUZZER_PIN,LOW);
    }
  }

  // FN_TIMER
  if ( active_mode == MODE_FN_TIMER && cti_stop_start == TIMER_START ) {
    if ( (millis()-cti_millis) >= 1000 ) {  
      // Update every 1000 milliseconds
      cti_millis = millis();
      fn_timer_update();
    }
  }
  
  process_digitalButtons();
  process_analogButtons();  
}
// --- end Arduino Setup/Loop ---

// --- start Menu ----
//
void menu_selector(int8_t moveDir) {
char tmp[3];

  cssd1306avr_clearScreen();
  cssd1306avr_showBitmap(6, 32, menu_bmp, 64); // Fixed Menu Bar display at bottom 

  // Navigate and display each options
  active_menu = g_cycle2digit(moveDir, active_menu, 0, 3);
  cssd1306avr_showBitmap(rc_menu[active_menu][0], rc_menu[active_menu][1], selector_bmp, 10); 

  // While navigating the Menu options, display function screen
  switch (active_menu) {
    case MENU_FN_CLOCK: fn_clock_show();
    break;
    case MENU_FN_ALARM: fn_alarm_show();
    break;
    case MENU_FN_TIMER: fn_timer_show();
    break;
    case MENU_FN_EXT:   fn_ext_show();
    break;           
    default:break;
  }
}

void menu_inactivated() {
  cssd1306avr_showBitmap(rc_menu[active_menu][0], rc_menu[active_menu][1], selector_bmpblank, 10);
}
// --- end Menu ---

// --- start Ext ---
void fn_ext_selector(int moveDir) {
  // Edit Positions
  cssd1306avr_showBitmap(rc_ext[active_ext][0], rc_ext[active_ext][1], selector_bmpblank, 10);
  active_ext = g_cycle2digit(moveDir, active_ext, 0, 0);
  cssd1306avr_showBitmap(rc_ext[active_ext][0], rc_ext[active_ext][1], selector_bmp, 10);  
}

void fn_ext_show() {
char tmp[3];
  cssd1306avr_showString(0,0,"EXT");
  cssd1306avr_showString(2,0,"1.alarm duration");
  g_str2digit(ala_duration_ss,tmp);cssd1306avr_showString(2,105,tmp);
}

void fn_ext_edit(int8_t editDir) {
char tmp[3];

  switch (active_ext) {
    case 0: 
      // buzzer duration
      ala_t_duration_ss = g_cycle2digit(editDir, ala_t_duration_ss,1,59); 
      g_str2digit(ala_t_duration_ss,tmp);cssd1306avr_showString(2,105,tmp); 
    break;         
    default:break;    
  }
}
// --- end EXT ---

// --- start Clock ---
void fn_clock_selector(int moveDir) {
  // Edit Positions
  cssd1306avr_showBitmap(rc_clock[active_clock][0], rc_clock[active_clock][1], selector_bmpblank, 10);
  active_clock = g_cycle2digit(moveDir, active_clock, 0, 7); 
  cssd1306avr_showBitmap(rc_clock[active_clock][0], rc_clock[active_clock][1], selector_bmp, 10);  
}

void fn_clock_show() {
char tmp[3];
char tmp5[5];
  // show hhmode 24/am/pm
  sf_show_hhmode(clo_hhmode,clo_hh12mode); 
  
  // show hh mm ss
  sf_show_hh(clo_hh); sf_show_mm(clo_mm); sf_show_ss(clo_ss);

  // show date
  cssd1306avr_showString(0,0,str_dow[clo_dow-1]);                // date-dow
  g_str2digit(clo_dd,tmp);cssd1306avr_showString(0,60,tmp);      // date-dd
  cssd1306avr_showString(0,76,str_mo[clo_mo-1]);                 // date-mo
  g_str4digit(clo_yyyy,tmp5);cssd1306avr_showString(0,98,tmp5);  // date-yyyy   
}

void fn_clock_edit(int8_t editDir) {
char tmp[3];
char tmp5[5];

  switch (active_clock) {
    case 0: 
      // time-hhmode
      fn_clock_edit_hhmode();
    break;
    case 1: 
      // date-dow
      clo_t_dow = g_cycle2digit(editDir,clo_t_dow,1,7);       
      cssd1306avr_showString(0,0,str_dow[clo_t_dow-1]);
    break;    
    case 2: 
      // date-dd
      clo_t_dd = g_cycle2digit(editDir,clo_t_dd,1,t_mo_end[clo_t_mo-1]);      
      g_str2digit(clo_t_dd,tmp);cssd1306avr_showString(0,60,tmp);
    break;        
    case 3: 
      // date-mo
      clo_t_mo = g_cycle2digit(editDir,clo_t_mo,1,12);              
      cssd1306avr_showString(0,76,str_mo[clo_t_mo-1]);
     
      // Adjust dd, if out of range from Month Change
      if (clo_t_dd > t_mo_end[clo_t_mo-1]) {
        clo_t_dd = t_mo_end[clo_t_mo-1]; 
        g_str2digit(clo_t_dd,tmp);cssd1306avr_showString(0,60,tmp);         
      }       
    break;         
    case 4: 
      // date-yyyy 
      clo_t_yyyy = g_cycle4digit(editDir, clo_t_yyyy, 1001, 9999);     
      g_str4digit(clo_t_yyyy,tmp5);cssd1306avr_showString(0,98,tmp5);  
  
      // Check for Leap Year and adjust Month End for Feb
      t_mo_end[1] = (g_isLeapYear(clo_t_yyyy)== 1) ? 29 : 28;

      // Adjust dd, if out of range for month feb
      if (clo_t_mo == 2) {
        if (clo_t_dd > t_mo_end[1]) {
          clo_t_dd = t_mo_end[1];
          g_str2digit(clo_t_dd,tmp);cssd1306avr_showString(0,60,tmp);          
        }
      }    
    break;      
    case 5: 
      // time-hh
      fn_clock_edit_hh(editDir);
    break;
    case 6: 
      // time-mm
      clo_t_mm = g_cycle2digit(editDir,clo_t_mm,0,59); 
      sf_show_mm(clo_t_mm);
    break; 
    case 7: 
      // time-ss    
      clo_t_ss = g_cycle2digit(editDir,clo_t_ss,0,59);            
      sf_show_ss(clo_t_ss);
    break;     
    default:break;    
  }
}

void fn_clock_edit_hhmode() {
char tmp[3];
  
  // NOTE: Changing Clock Mode ( 12/24 hours ) will effect Alarm Time

  if (clo_t_hhmode == HHMODE_24) {
    // Currently 24hours Mode

    // Clock Hour adjustment from 24 to 12 Hours mode
    if (clo_t_hh> 11) {
      clo_t_hh12mode = HH12MODE_PM;           // hh more than 11, are are all PM
      if (clo_t_hh> 12) clo_t_hh=clo_t_hh-12; // minus 12 hours
    } else {
      clo_t_hh12mode = HH12MODE_AM;           // hh 11 or less, are all AM
      if (clo_t_hh == 0) clo_t_hh=12;         // zero in 24 hours, becomes 12
    } 
    
    // Alarm Time, Hour adjustment
    if (ala_t_hh > 11) {
      ala_t_hh12mode = HH12MODE_PM;           // hh more than 11, are are all PM   
      if (ala_t_hh > 12) ala_t_hh=ala_t_hh-12; // minus 12 hours
    } else {
      ala_t_hh12mode = HH12MODE_AM;           // hh 11 or less, are all AM
      if (ala_t_hh == 0) ala_t_hh=12;         // zero in 24 hours, becomes 12 
    } 

    clo_t_hhmode = HHMODE_12; // Switch to 12 hours Mode for both Clock and Alarm
    
  } else {    
    // Currently 12hours

    // Clock Hour adjustment from 12 to 24 hours mode
    if ( clo_t_hh12mode == HH12MODE_PM) {
      if (clo_t_hh< 12) clo_t_hh=clo_t_hh+12;  // This is PM, Add +12 to all, except for 12pm
    } else {
      if (clo_t_hh== 12) clo_t_hh=0;           // This is AM, Everything is same, except for 12am, 0 in 24 hours
    }
    
    // Alarm Time, Hour adjustment
    if ( ala_t_hh12mode == HH12MODE_PM) {
      if (ala_t_hh < 12) ala_t_hh=ala_t_hh+12;    // This is PM, Add +12 to all, except for 12pm
    } else {
      if (ala_t_hh == 12) ala_t_hh=0;            // This is AM, Everything is same, except for 12am, 0 in 24 hours
    }
  
    clo_t_hhmode = HHMODE_24; clo_t_hh12mode = HH12MODE_NA; // Clock switched to 24 hours
  } 

  // show hhmode 24/am/pm
  sf_show_hhmode(clo_t_hhmode,clo_t_hh12mode); 
  
  // show hh
  sf_show_hh(clo_t_hh);  
}

void fn_clock_edit_hh(int8_t editDir) {
char tmp[3];

  // NOTE:Changing Hour in Clock does not effect the Alarm Time

  if (clo_t_hhmode == HHMODE_24)  {
    // 24 hours mode, Cycle between 0 to 24
    clo_t_hh= g_cycle2digit(editDir,clo_t_hh,0,23);   
  } else {
    // 12 hours mode
    if (editDir == 1) { 
        // Increase One Hour
        if (clo_t_hh12mode == HH12MODE_PM) {
          // This is PM
          if (clo_t_hh == 11) clo_t_hh12mode=HH12MODE_AM;     // 11PM to 12AM
          clo_t_hh++;
          if (clo_t_hh > 12) clo_t_hh=1;                      // next hour affer 12 is 1
        } else {
          // This is AM
          if (clo_t_hh == 11) clo_t_hh12mode=HH12MODE_PM;     // 11AM to 12PM
          clo_t_hh++;
          if (clo_t_hh > 12) clo_t_hh= 1;                     // next hour after 12 is 1
        }
    } else {
      if (editDir == -1) { 
        // Reduce One Hour
        if (clo_t_hh12mode == HH12MODE_PM) {
          // This is PM
          if (clo_t_hh == 12) clo_t_hh12mode=HH12MODE_AM;    // 12PM to 11AM
          clo_t_hh--;
          if (clo_t_hh < 1) clo_t_hh=12;                     // previous hour before 1 is 12
        } else {
          // This is AM
          if (clo_t_hh == 12) clo_t_hh12mode=HH12MODE_PM;    // 12AM to 11PM
          clo_t_hh--;
          if (clo_t_hh < 1) clo_t_hh=12;                     // previous hour before 1 is 12
        }       
      }
    }
  }

  // show hhmode 24/am/pm
  sf_show_hhmode(clo_t_hhmode,clo_t_hh12mode); 
  
  // show hh
  sf_show_hh(clo_t_hh);
}

void fn_clock_update() {
char tmp[3];
  clo_ss++;  
  if (clo_ss > 59) {
    clo_ss = 0;
    clo_mm++;
    if (clo_mm > 59) {         
      clo_mm = 0;       
      if ( clo_hhmode == HHMODE_24 ) {    
        clo_hh++;   
        if (clo_hh > 23) {   
          clo_hh = 0; // After Hour 23, is 0 for 24 Hours mode
          fn_clock_update_nextday();
        }
      } else {   
        // 12 Hours Mode
        if (clo_hh == 12) {          
          clo_hh = 1;
        } else {
          // Hour is not 12, increase Hour by 1
          clo_hh++;
          if (clo_hh == 12) { 
            if (clo_hh12mode == HH12MODE_PM) {  
               // this was previously 11pm, increased to 12, is next day AM
               clo_hh12mode = HH12MODE_AM;
               fn_clock_update_nextday();
            } else {
              // this was previously 11pm, increased to 12, same day PM
              clo_hh12mode = HH12MODE_PM;              
            }
          }
        }
      }     
    } 
  } 
}
void fn_clock_update_nextday() {
  clo_dow++; 
  if(clo_dow > 7) clo_dow = 1;
  
  clo_dd++;
  if (clo_dd > mo_end[clo_mo-1]) {
    clo_dd = 1;
    clo_mo++;
    if (clo_mo > 12) {
      clo_mo = 1;    
      clo_yyyy++;
      if (clo_yyyy > 9999) {
        clo_yyyy = 2001; // Go back to default, 2001
      }
    }
  }
}
// --- end Clock ---

// --- start Alarm ---
void fn_alarm_selector(int moveDir) {
  // Edit Positions
  cssd1306avr_showBitmap(rc_alarm[active_alarm][0], rc_alarm[active_alarm][1], selector_bmpblank, 10);
  active_alarm = g_cycle2digit(moveDir, active_alarm, 0, 3); 
  cssd1306avr_showBitmap(rc_alarm[active_alarm][0], rc_alarm[active_alarm][1], selector_bmp, 10); 
}

void fn_alarm_show() {
char tmp[3];
  cssd1306avr_showString(0, 0,"ALARM"); 
  cssd1306avr_showLargeString(0,91,str_off_on[ala_off_on]);      // alarm ON/OFF
  
  // show hhmode 24/am/pm
  sf_show_hhmode(clo_hhmode,ala_hh12mode); 

  // show hh mm ss
  sf_show_hh(ala_hh); sf_show_mm(ala_mm); sf_show_ss(ala_ss);  
}

void fn_alarm_edit(int8_t editDir) {
char tmp[3];

  switch (active_alarm) {
    case 0: 
      // time-hh
      fn_alarm_edit_hh(editDir);
    break;
    case 1: 
      // time-mm
      ala_t_mm = g_cycle2digit(editDir, ala_t_mm,0,59); 
      sf_show_mm(ala_t_mm);
    break;
    case 2: 
      // time-ss
      ala_t_ss = g_cycle2digit(editDir,ala_t_ss,0,59);              
      sf_show_ss(ala_t_ss);       
    break;    
    case 3: 
      // alarm ON/OFF
      ala_t_off_on  = (ala_t_off_on==ALARM_OFF) ? ALARM_ON : ALARM_OFF; 
      cssd1306avr_showLargeString(0,91,str_off_on[ala_t_off_on]);
    break;         
    default:break;    
  }
}
void fn_alarm_edit_hh(int8_t editDir) {
char tmp[3];

  if (clo_t_hhmode == HHMODE_24)  {
    // 24 hours mode, Cycle between 0 to 24
    ala_t_hh= g_cycle2digit(editDir,ala_t_hh,0,23);   
  } else {
    // 12 hours mode
    if (editDir == 1) { 
        // Increase One Hour
        if (ala_t_hh12mode == HH12MODE_PM) {
          // This is PM
          if (ala_t_hh == 11) ala_t_hh12mode=HH12MODE_AM;     // 11PM to 12AM
          ala_t_hh++;
          if (ala_t_hh > 12) ala_t_hh=1;                      // next hour affer 12 is 1
        } else {
          // This is AM
          if (ala_t_hh == 11) ala_t_hh12mode=HH12MODE_PM;     // 11AM to 12PM
          ala_t_hh++;
          if (ala_t_hh > 12) ala_t_hh= 1;                     // next hour after 12 is 1
        }
    } else {
      if (editDir == -1) { 
        // Reduce One Hour
        if (ala_t_hh12mode == HH12MODE_PM) {
          // This is PM
          if (ala_t_hh == 12) ala_t_hh12mode=HH12MODE_AM;    // 12PM to 11AM
          ala_t_hh--;
          if (ala_t_hh < 1) ala_t_hh=12;                     // previous hour before 1 is 12
        } else {
          // This is AM
          if (ala_t_hh == 12) ala_t_hh12mode=HH12MODE_PM;    // 12AM to 11PM
          ala_t_hh--;
          if (ala_t_hh < 1) ala_t_hh=12;                     // previous hour before 1 is 12
        }       
      }
    }
  }

  // show hhmode 24/am/pm
  sf_show_hhmode(clo_hhmode,ala_t_hh12mode); 
  
  // show hh
  sf_show_hh(ala_t_hh);  
}

void fn_alarm_monitor() {
  if (alarm_buzzer == 1) return; // already triggered
  
  if (clo_hh == ala_hh) { // same hour
    if (clo_mm == ala_mm) {  // same min
      if (clo_ss == ala_ss ) { // same seconds
        // Trigger Buzzer
        alarm_buzzer = 1; 
        digitalWrite(BUZZER_PIN, HIGH);
        millis_alarm_buzzer = millis();
      }
    }   
  }
}
// --- end Alarm ---

// --- start Countdown Timer ---
//
void fn_timer_selector(int8_t moveDir) {
  if (cti_stop_start == TIMER_START) return; // disable when timer is running 
  
  // Edit Positions
  cssd1306avr_showBitmap(rc_timer[active_timer][0], rc_timer[active_timer][1], selector_bmpblank, 10);
  active_timer = g_cycle2digit(moveDir, active_timer, 0, 3); 
  cssd1306avr_showBitmap(rc_timer[active_timer][0], rc_timer[active_timer][1], selector_bmp, 10);  
}

void fn_timer_show() {
  cssd1306avr_showString(0, 0,"TIMER"); 
  cssd1306avr_showString(0,91,str_stop_start[cti_stop_start]); // timer STOP/START
  
  // show hh mm ss
  sf_show_hh(cti_hh); sf_show_mm(cti_mm); sf_show_ss(cti_ss);
}

void fn_timer_edit(int8_t editDir) {
char tmp[3];
  
  switch (active_timer) {
    case 0:
      // START/STOP Timer
      cti_stop_start  = ( cti_stop_start == TIMER_STOP) ? TIMER_START : TIMER_STOP; 
      cssd1306avr_showString(0,91,str_stop_start[cti_stop_start]);    
    case 1: 
      // time-hh
      cti_hh = g_cycle2digit(editDir, cti_hh, 0, 99);          
      sf_show_hh(cti_hh);
    break;
    case 2: 
      // time-mm
      cti_mm = g_cycle2digit(editDir, cti_mm, 0, 59); 
      sf_show_mm(cti_mm);
    break;    
    case 3: 
      // time-ss
      cti_ss = g_cycle2digit(editDir, cti_ss, 0, 99);
      sf_show_ss(cti_ss);      
    break;        
    default:break;    
  }
}

void fn_timer_update() {
  if (cti_ss > 0) {
    cti_ss--;   
  } else {
    if ( cti_mm > 0 ) {
      cti_mm--;
      cti_ss = 59;
    } else {
      if ( cti_hh > 0 ) {
        cti_hh--;
        cti_ss = 59;        
        cti_mm = 59;     
      } else {
        // Timer is now 00:00:00, Countdown Ended
        cti_stop_start = TIMER_STOP;  
        digitalWrite(BUZZER_PIN, HIGH);g_pauseMillis(500);digitalWrite(BUZZER_PIN, LOW);
      }
    }  
  }
  fn_timer_show();  
}
// --- end Timer ---

// --- start Shared functions  ---
//
void sf_turn_off_buzzer() {
  alarm_buzzer = 0;
  digitalWrite(BUZZER_PIN, LOW);
}
void sf_show_hhmode(int8_t hhmode, int8_t hh12mode) {
char tmp[3];
  tmp[0]='2';tmp[1]='4';tmp[2]='\0';
  if ( hhmode == HHMODE_12 ) {        
    if (hh12mode == HH12MODE_PM) {
      tmp[0]='P';
      tmp[1]='M'; 
    } else {
      tmp[0]='A';
      tmp[1]='M';          
    }     
  } 
  cssd1306avr_showString(4,110,tmp);
}
void sf_show_hh(int8_t hh) {
char tmp[3];
  g_str2digit(hh,tmp);cssd1306avr_showLargeString(3,24,tmp);          
}
void sf_show_mm(int8_t mm) {
char tmp[3];        
  g_str2digit(mm,tmp);cssd1306avr_showLargeString(3,52,tmp);        
}
void sf_show_ss(int8_t ss) {
char tmp[3];       
  g_str2digit(ss,tmp);cssd1306avr_showLargeString(3,80,tmp); 
}
// --- end Shared functions ---

// --- start Button Processing ---
//
void button_ENTER_function() {
  if ( alarm_buzzer == 1) {
    alarm_buzzer = 0; digitalWrite(BUZZER_PIN, LOW);
    return;  
  }  
  switch (active_mode) {  
    case MODE_MENU:
      switch (active_menu) {
        case MENU_FN_CLOCK:
          clo_t_hh          = clo_hh;
          clo_t_mm          = clo_mm;
          clo_t_ss          = clo_ss;
          clo_t_dow         = clo_dow;
          clo_t_dd          = clo_dd;
          clo_t_mo          = clo_mo;
          clo_t_yyyy        = clo_yyyy;
          clo_t_hhmode      = clo_hhmode;
          clo_t_hh12mode    = clo_hh12mode;
          t_mo_end[1]       = mo_end[1];          
          ala_t_hh          = ala_hh;
          ala_t_hh12mode    = ala_hh12mode;                 
          menu_inactivated();          
          active_clock      = 0;           
          active_mode       = MODE_FN_CLOCK; fn_clock_selector(0); // Clock Function
        break;  
        case MENU_FN_ALARM:
          ala_t_hh          = ala_hh;
          ala_t_mm          = ala_mm;
          ala_t_ss          = ala_ss;
          ala_t_off_on      = ala_off_on;            
          menu_inactivated();          
          active_alarm      = 0;
          active_mode       = MODE_FN_ALARM; fn_alarm_selector(0); // Alarm Function
        break;            
        case MENU_FN_TIMER:
          menu_inactivated();        
          active_timer      = 0;
          active_mode       = MODE_FN_TIMER; fn_timer_selector(0); // Timer function
        break;
        case MENU_FN_EXT:        
          ala_t_duration_ss  = ala_duration_ss;        
          menu_inactivated();          
          active_ext        = 0;
          active_mode       = MODE_FN_EXT; fn_ext_selector(0); // Ext Mode
        break;           
        default:break;
      }
    break;
    case MODE_FN_CLOCK:
       clo_hh         = clo_t_hh;
       clo_mm         = clo_t_mm;
       clo_ss         = clo_t_ss; 
       clo_dow        = clo_t_dow;
       clo_dd         = clo_t_dd;
       clo_mo         = clo_t_mo;
       clo_yyyy       = clo_t_yyyy;
       clo_hhmode     = clo_t_hhmode;
       clo_hh12mode   = clo_t_hh12mode;    
       mo_end[1]      = t_mo_end[1];
       ala_hh         = ala_t_hh;
       ala_hh12mode   = ala_t_hh12mode;           
       millis_clock   = millis(); // Reset Clock
       active_mode    = MODE_MENU; menu_selector(0); // return to MENU
    break;
    case MODE_FN_ALARM:   
       ala_hh         = ala_t_hh;
       ala_hh12mode   = ala_t_hh12mode;         
       ala_mm         = ala_t_mm;
       ala_ss         = ala_t_ss;
       ala_off_on     = ala_t_off_on;                   
       active_mode    = MODE_MENU; menu_selector(0); // return to MENU
    break;
    case MODE_FN_TIMER:
      cssd1306avr_showBitmap(rc_timer[active_timer][0], rc_timer[active_timer][1], selector_bmpblank, 10);
      active_timer  = 0; 
      cssd1306avr_showBitmap(rc_timer[active_timer][0], rc_timer[active_timer][1], selector_bmp, 10);     
      if (cti_stop_start == TIMER_STOP) {
        cti_stop_start = TIMER_START; cti_millis = millis();
      } else {
        cti_stop_start = TIMER_STOP;        
      }
      cssd1306avr_showString(0,91,str_stop_start[cti_stop_start]);
      // DOES NOT return to MENU, use ESC to return to MENU      
    break;
    case MODE_FN_EXT:
       ala_duration_ss = ala_t_duration_ss;    
       active_mode     = MODE_MENU; menu_selector(0); // return to MENU
    break;    
    default:break;
  }
  cssd1306avr_showChar(7,0,'E');  // Debug, to be removed
}

void button_ESC_function() {
  if ( alarm_buzzer == 1) {
    alarm_buzzer = 0; digitalWrite(BUZZER_PIN, LOW);
    return;  
  }
  switch (active_mode) {
  case MODE_MENU:
    // Move to first Option in MENU
    active_menu = 0; 
  break;    
  case MODE_FN_TIMER:
    // Stop and Reset Timer
    cti_stop_start = TIMER_STOP; 
    cti_hh=0; cti_mm=0; cti_ss=30;
  break;  
  default:break;
  }    
  active_mode = MODE_MENU;  menu_selector(0); // return to MENU
  
  cssd1306avr_showChar(7,0,'X');  // debug, to be removed  
}

void button_LEFT_function() {    
  if ( alarm_buzzer == 1) {
    alarm_buzzer = 0; digitalWrite(BUZZER_PIN, LOW);
    return;  
  }  
  switch (active_mode) {
    case MODE_MENU: menu_selector(-1);
    break;
    case MODE_FN_CLOCK: fn_clock_selector(-1);
    break;   
    case MODE_FN_ALARM: fn_alarm_selector(-1);
    break;     
    case MODE_FN_TIMER: fn_timer_selector(-1);      
    break;
    case MODE_FN_EXT: fn_ext_selector(-1);      
    break;    
    default:break;
  };
  cssd1306avr_showChar(7,0,'L'); // debug, to be removed
}
void button_RIGHT_function() {  
  if ( alarm_buzzer == 1) {
    alarm_buzzer = 0; digitalWrite(BUZZER_PIN, LOW);
    return;  
  }  
  switch (active_mode) {
    case MODE_MENU: menu_selector(1);
    break;
    case MODE_FN_CLOCK: fn_clock_selector(1);
    break;   
    case MODE_FN_ALARM: fn_alarm_selector(1);
    break;   
    case MODE_FN_TIMER: fn_timer_selector(1);
    break;    
    case MODE_FN_EXT: fn_ext_selector(1);
    break;        
    default:break;
  };     
  cssd1306avr_showChar(7,0,'R');
}
void button_UP_function() {  
  if ( alarm_buzzer == 1) {
    alarm_buzzer = 0; digitalWrite(BUZZER_PIN, LOW);
    return;  
  }  
  switch (active_mode) {
    case MODE_MENU: // UP Not Used in Menu State
    break;
    case MODE_FN_CLOCK: fn_clock_edit(1);      
    break;
    case MODE_FN_ALARM: fn_alarm_edit(1);      
    break;    
    case MODE_FN_TIMER: fn_timer_edit(1);      
    break;    
    case MODE_FN_EXT: fn_ext_edit(1);      
    break;      
    default:break;
  };  
  cssd1306avr_showChar(7,0,'U');  // debug, to be removed
}
void button_DOWN_function() { 
  if ( alarm_buzzer == 1) {
    alarm_buzzer = 0; digitalWrite(BUZZER_PIN, LOW);
    return;  
  }
  switch (active_mode) {
    case MODE_MENU: // DOWN Not used in Menu State
    break;
    case MODE_FN_CLOCK: fn_clock_edit(-1);      
    break;
    case MODE_FN_ALARM: fn_alarm_edit(-1);      
    break;
    case MODE_FN_TIMER: fn_timer_edit(-1);      
    break;
    case MODE_FN_EXT: fn_ext_edit(-1);      
    break;    
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
int g_cycle4digit(int8_t iDir, int iNum, int iMin, int iMax) {
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
void g_str2digit(int8_t nn, char bStr[]) {
int8_t nIdx;
  bStr[0]='0';bStr[1]='0';bStr[2]='\0'; 
  nIdx = 1;
  while (nn > 0) { // Load Numbers
    bStr[nIdx] = (nn % 10) + 48; // ASCII 48 = 0
    nn = nn / 10;
    nIdx--;
  }
}
void g_str4digit(int nn, char bStr[]) {
int8_t nIdx;
  bStr[0]='0';bStr[1]='0';bStr[2]='0';bStr[3]='0';bStr[4]='\0'; 
  nIdx = 3;
  while (nn > 0) { // Load Numbers
    bStr[nIdx] = (nn % 10) + 48; // ASCII 48 = 0
    nn = nn / 10;
    nIdx--;
  }
}
char g_isLeapYear(int yyyy) {
  if ( ( (yyyy%4 == 0) && (yyyy%100 != 0) ) || (yyyy%400 == 0) ) {
    return 1;
  } else {
    return 0;
  }
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
