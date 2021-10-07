// Arduino Project - stemkraf
// https://github.com/teaksoon/stemkraf
//
// Arduino Program: sk_thermistor.ino
// Thermistor
// by TeakSoon Ding for STEMKRAF ( Oct-2021 )
// ----------------------------------------------------------------------
//
#define THERMISTOR_PIN A0

void setup() {
  Serial.begin(9600);
  Serial.print("\nSerial Monitor\n"); // Serial Monitor for Display
}
void loop() {  
int8_t cel_nn, celfrac_nn;

  g_thermistor3380_c_read(THERMISTOR_PIN, &cel_nn, &celfrac_nn);
  
  Serial.print("\nTemperature reading from Thermistor (10Kohm beta=3380): ");
  Serial.print(cel_nn);Serial.print(".");Serial.print(celfrac_nn);  
  Serial.print(" celsius");
  
  g_pauseMillis(1000);  
}
// --- end Arduino Setup/Loop ---

void g_thermistor3380_c_read(uint8_t analogPin, int8_t *cel_nn, int8_t *celfrac_nn) {
const float tBeta     = 1.0/3380.0;   // Set by factory Beta=3380, 10Kohm Thermistor use with 10Kohm Resistor
const float tControl  = 1.0/298.15;   // Factory Control Temperature=25 Celsius. (25+273.15) = 298.15 in Kelvin
float tf_Kelvin,tf_Celsius;           // float Working Memory
int pinValue;
int tempData[5];

  // Make 5 readings to filter strays
  for (int8_t i=0; i<5; i++) {
    pinValue = analogRead(analogPin); tempData[i] = pinValue;
  }
  pinValue    = g_getMedian(tempData, 5);
  
  tf_Kelvin   = 1.0/(tControl+(tBeta*(log((1023/(float)pinValue)-1.0))));
  tf_Celsius  = tf_Kelvin-273.15; // Kelvin to Celsius
    
  *cel_nn     = (int8_t) tf_Celsius;
  *celfrac_nn = (int8_t) ((tf_Celsius-*cel_nn)*100);
  *celfrac_nn = abs(*celfrac_nn);
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
unsigned long storedMillis;
  storedMillis = millis();
  while( !( (millis()-storedMillis)> nn) ) { 
    // do nothing in this loop until current millis counter is more than
    // previously stored millis counter by nn miliseconds
  }  
}
