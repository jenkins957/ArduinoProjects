// Basic Digital Clock using a seven segment display (I2C backpack) & DS1307 real-time clock (I2C).

#include "RTClib.h"
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#define TIME_24_HOUR true
//#define DEBUG

RTC_DS1307 rtc;
Adafruit_7segment clockDisplay = Adafruit_7segment();

void setup() 
{
  clockDisplay.begin(0x70);

#if defined(DEBUG)
  Serial.begin(57600);
  if (!rtc.begin()) 
  {
    Serial.println("RTC Module not available");
    Serial.flush();
    abort();
  }
#endif
}

void loop() 
{
  updateClockDisplay();
  delay(1000);
}

void updateClockDisplay()
{
  DateTime now = rtc.now();

  int hours = now.hour();
  int minutes = now.minute();
    
  int displayValue = hours * 100 + minutes;

  // 24 hour to 12 hour format conversion when required.
  if (!TIME_24_HOUR) 
  {
    if (hours > 12) 
    {
      displayValue -= 1200;
    }
    else if (hours == 0) 
    {
      displayValue += 1200;
    }
  }

  clockDisplay.print(displayValue, DEC);
    
  if (TIME_24_HOUR && hours == 0) 
  {
    // Pad hour 0.
    clockDisplay.writeDigitNum(1, 0);

    // Pad when the 10's minute is 0 and should be padded.
    if (minutes < 10) 
    {
      clockDisplay.writeDigitNum(2, 0);
    }
  }    

#if defined(DEBUG)
  Serial.println(displayValue);
#endif  
  clockDisplay.drawColon(true);
  clockDisplay.writeDisplay();
}
