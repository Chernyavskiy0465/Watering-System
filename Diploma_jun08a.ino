#include "arduino_secrets.h"
#include "thingProperties.h"
#include "DHT.h"
#define DHTpin 15
#define DHTTYPE DHT11
#define analogPin A0

DHT dht (DHTpin, DHTTYPE);

unsigned long previousMillis = 0;
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 73;
unsigned long lastTime;
/*unsigned long previousMillis3 = 0;
unsigned long previousMillis4 = 0;*/
unsigned long period = 1050;
unsigned long period1 = 1030;
unsigned long period2 = 120000;
/*unsigned long period3 = 2300;
unsigned long period4 = 1300;*/
unsigned long currentMillis, currentMillis1, currentMillis2, currentMillis3, currentMillis4;


const int ADC_Pin = A0;
int LEDG = 14;
int LEDR = 12;
int LEDB = 13;
int PUMP = 4;



void setup() {
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(PUMP, OUTPUT);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}
/*----------------------------------------------------------------------------------------------*/
void loop() {
  ArduinoCloud.update();

currentMillis = millis();
  //Serial.println(currentMillis);
  if (currentMillis - previousMillis >= period) { // check if 1000ms passed
    dht_sensor_getdata();
    previousMillis = currentMillis;
  }
currentMillis1 = millis();
  if (currentMillis1 - previousMillis1 >= (period1)) { // check if 2000ms passed
    onMoistureValueChange();
    previousMillis1 = currentMillis1;
  }
currentMillis2 = millis();
int moisture = analogRead(ADC_Pin);// reads the value at a digital input
  if (currentMillis2 - previousMillis2 >= (period2) && moisture < 400) { // check if 2000ms passed
    onMoistureCheck();
    previousMillis2 = currentMillis2;
  }

  
  
  
}
/*----------------------------------------------------------------------------------------------*/

void dht_sensor_getdata()
{
  float hm = dht.readHumidity();
  Serial.print("---------");
  Serial.print("Humidity ");
  Serial.println(hm);
  float temp = dht.readTemperature();
  Serial.print("Temperature ");
  Serial.println(temp);
  humidity = hm;
  temperature = temp;
  Serial.println(currentMillis);
}

void onMoistureValueChange()
{
  int moisture = analogRead(ADC_Pin);// reads the value at a digital input
  Serial.print("---------");
  Serial.println("Moisture ");
  Serial.println(moisture);
  moisture_value = map(moisture, 0, 1023, 0, 100);
  Serial.println(currentMillis1);
  
  if(moisture < 400){
    digitalWrite(LEDB, LOW);
    digitalWrite(LEDG, LOW);
  }
  else{
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  }
  
  
}

void onWateringChange()  {
 
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
}
  

 
void onPowerChange(){
    if(power && watering == 8){
    pump8();
    }
    if(power && watering == 15){
    pump15();
    }
    if(power && watering == 22){
    pump22();
    }
}  

void onMoistureCheck () {
  if(watering == 8){
    pump8();
    }
    if(watering == 15){
    pump15();
    }
    if(watering == 22){
    pump22();
    }
}

void pump8(){
  digitalWrite( PUMP, HIGH );
  delay(8000);
  digitalWrite( PUMP, LOW );
}

void pump15(){
  digitalWrite( PUMP, HIGH );
  delay(15000);
  digitalWrite( PUMP, LOW );
}
void pump22(){
  digitalWrite( PUMP, HIGH );
  delay(22000);
  digitalWrite( PUMP, LOW );
}

