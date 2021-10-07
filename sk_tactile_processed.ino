// Arduino Project - stemkraf
// https://github.com/teaksoon/stemkraf
//
// Arduino Program: sk_tactile_processed.ino
// Tactile - Processed
// by TeakSoon Ding for STEMKRAF ( Oct-2021 )
// ----------------------------------------------------------------------
//
// --- start function declaration ---
//
int g_getMedian(int dataArray[], int8_t dataCount);
void g_pauseMillis(unsigned long nn );

void process_digitalButtons(); 
void process_analogButtons(); 
void button_ENTER_function();
void button_ESC_function();
void button_LEFT_function();
void button_DOWN_function(); 
void button_RIGHT_function();
void button_UP_function();
// --- end function declaration ---

// --- start defines and global variables ---
//
#define BUZZER_PIN          8
#define NAVIGATE_BTN_PIN    A1
#define ENTER_BTN_PIN       7
#define ESC_BTN_PIN         6

// delay in milliseconds for debounce correction
// increase if not sufficient, reduce if button needs speed
#define DEBOUNCE_MS         10  

// Get these Analog number from running the "apg_tactile_analog_raw.ino" program
// Make sure the gap between two buttons is more than 40 when AREF_BUFFER=20
// Use different Resistor Values to adjust the gap
#define AREF_BUFFER        20
#define AREF_LEFT          950 // This button is firs button. We do a -100 to max (1023)
#define AREF_DOWN          500 
#define AREF_RIGHT         335
#define AREF_UP            250

char btnEnter         = 'N'; // Y ( Digital Pin )
char btnEsc           = 'N'; // Y ( Digital Pin )
char btnNavigate      = 'N'; // L D R U ( Analog Pin )
// --- end defines and global variables---

// -- start Arduino Setup/Loop ---
//
void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);g_pauseMillis(300);digitalWrite(BUZZER_PIN, LOW);

  Serial.begin(9600);
  Serial.println("\nSerial Monitor"); // Serial Monitor Display
}

void loop() {  
  process_digitalButtons();
  process_analogButtons();  
}
// --- end Arduino Setup/Loop ---

// --- start Button Processing ---
//
void button_ENTER_function() {
  Serial.print("\nENTER Button Pressed");  
}

void button_ESC_function() {
  Serial.print("\nESC Button Pressed");  
}

void button_LEFT_function() {   
  Serial.print("\nLEFT Button Pressed");  
}

void button_RIGHT_function() {
  Serial.print("\nRIGHT Button Pressed");  
}

void button_UP_function() { 
  Serial.print("\nUP Button Pressed");    
}

void button_DOWN_function() { 
  Serial.print("\nDOWN Button Pressed");   
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
