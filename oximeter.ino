#include <LiquidCrystal.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
LiquidCrystal lcd(9,8, 7,6,5,4);
//Creates an LCD object.
// Parameters: (rs, enable, d4, d5, d6, d7)
#define REPORTING_PERIOD_MS     1000
PulseOximeter pox;
uint32_t tsLastReport = 0;
void onBeatDetected()
{
  Serial.println("Beat!");
}
void setup()
{
  Serial.begin(115200);
  Serial.print("Initializing pulse oximeter..");
  lcd.begin(16,2);
  lcd.print("Initializing...");
  //prints Initializing...  for 3 sec
  delay(3000);
  lcd.clear();
  /* Initialize the PulseOximeter instance.Failures are generally due to an improperI2C wiring, missing power supplieror wrong target chip*/
  if (!pox.begin()){Serial.println("FAILED");for(;;);}
  else
  {
    Serial.println("SUCCESS");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  // Register a callback for the beat detection
  pox.setOnBeatDetectedCallback(onBeatDetected);
}
void loop()
{
  pox.update();
  if (millis()-tsLastReport>REPORTING_PERIOD_MS)
  {
    Serial.print("Heart rate:");
    Serial.print(pox.getHeartRate());
    Serial.print("bpm / SpO2:");
    Serial.print(pox.getSpO2());
    Serial.println("%");
    lcd.clear();
    lcd.setCursor(0,0);
    //sets the cursor at (0,0) i.e
    // first position in first line in lcd
    lcd.print("BPM : ");
    lcd.print(pox.getHeartRate());
    lcd.setCursor(0,1);
    //sets the cursor at (0,1) position that
    //is in first position in
    //second line in lcd
    lcd.print("SpO2: ");
    lcd.print(pox.getSpO2());
    lcd.print("%");
    tsLastReport = millis();
  }
}
