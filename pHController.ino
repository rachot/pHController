#include <Event.h>
#include <Timer.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Timer t;
 
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to

float sensorValue = 0;        // value read from the pot
float sensorValueOld;
float voltage;
float pH;
float senSorValueSum;
const float pump_feed = 4.347826; // ml/sec

int pHcount=0;

void sensor_Read()
{
    senSorValueSum = 0;
  
  for(int l=1;l<=100;l++)
  {
    sensorValue = analogRead(analogInPin);            
    senSorValueSum = senSorValueSum + sensorValue;

  }

sensorValue =senSorValueSum/100;  
pH = 7.8 - ((0.026315789473684210526315789473684*sensorValue)-12.421052631578947368421052631579);

}

void setup() {
  Serial.begin(9600); 
     int tickEvent = t.every(10800000, doSomething); // every 3 hours
   //int tickEvent = t.every(3600000, doSomething);   // every 1 hour
   //int tickEvent = t.every(1000, doSomething);   // every 10 seconds
   pinMode(7, OUTPUT);  
   digitalWrite(7, HIGH);

  lcd.begin(20, 4);
  lcd.print("  pH Controller  ");
  
}

void loop() {
  t.update();
  
sensor_Read();

  Serial.print("pH = " );                       
  Serial.print(pH,1);      
  Serial.print("   ADC = " );                       
  Serial.println(sensorValue);
  delay(50);             

    lcd.setCursor(0, 1);
      lcd.print("Monitoring Mode");
  lcd.setCursor(0, 2);
  lcd.print("pH = ");
  lcd.print(pH,1);
  lcd.print("  EC = --");
  lcd.setCursor(0, 3);
  lcd.print("Temp = --");
  //lcd.setCursor(0, 2);
  //lcd.print("ADC = ");
  //lcd.print(sensorValue);

  
  delay(3000);
     
}

void doSomething() // do after tick 
{
  sensor_Read();
  //while(sensorValue<=515)  // pH = 6.2
  while(pH>6.2)  // pH = 6.2
  {
    
    float ml_feed=0;
    float pump_time=0;
    ml_feed = (pH - 6.0)/0.009;
    pump_time = ml_feed/pump_feed;
    
      Serial.print("pH = " );                       
      Serial.print(pH,1);      
      Serial.print("   ADC = " );                       
      Serial.println(sensorValue);
      Serial.print("Feed water = ");
      Serial.print(ml_feed);
      Serial.print("\r\n");
      Serial.print("Pump time = ");
      Serial.print((int)pump_time);
      Serial.print("\r\n");
      
      digitalWrite(7, LOW);
      delay((int)pump_time*1000); 
      digitalWrite(7, HIGH);
      
      // wait for mixing time
      Serial.print("Mixing....");
      Serial.print("\r\n");
 
      lcd.setCursor(0, 3);
      lcd.print("Mixing...");
  
      for(int co=0;co<900;co++)
        delay(1000);
        
     Serial.print("Finished Mixing");
        
         sensor_Read();
  }
  Serial.print("pH Ready...");
}

