#include <Event.h>
#include <Timer.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


 Timer t;
 
// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to

float sensorValue = 0;        // value read from the pot
float sensorValueOld;
float voltage;
float pH;
float senSorValueSum;

int pHcount=0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  
 //  int tickEvent = t.every(10800000, doSomething); // every 3 hours
   int tickEvent = t.every(30000, doSomething);
   pinMode(7, OUTPUT);  
digitalWrite(7, HIGH);

  lcd.begin(20, 4);
    lcd.print("  pH Controller  ");
}

void loop() {
  t.update();
  

  
  senSorValueSum = 0;
  
  for(int l=1;l<=20;l++)
  {
    sensorValue = analogRead(analogInPin);            
    senSorValueSum = senSorValueSum + sensorValue;

  }

sensorValue =senSorValueSum/20;  
pH = 7.3 - ((0.026315789473684210526315789473684*sensorValue)-12.421052631578947368421052631579);

  Serial.print("pH = " );                       
  Serial.print(pH,1);      
  Serial.print("   ADC = " );                       
  Serial.println(sensorValue);
  delay(50);             

  
     lcd.setCursor(0, 1);
     lcd.print("pH = ");
     lcd.print(pH,1);
     delay(1000);
     
}

void doSomething()
{
  Serial.print("pH = " );                       
  Serial.print(pH,1);      
  Serial.print("   ADC = " );                       
  Serial.println(sensorValue);

  if(sensorValue<=441)
  {
    Serial.println("pH = 6.5 Feed water 50 ml.");  
   digitalWrite(7, LOW);
   //delay(12000); 
   digitalWrite(7, HIGH);
  }
}
