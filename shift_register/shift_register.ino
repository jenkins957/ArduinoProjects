/*
 * Demonstration connecting Arduino (Nano) to 8-Bit Shift Register.
 * The output is displayed on 8 LEDs.
 * 74HC595 (CD74HC595E) - Shift Register , Serial to Parallel
 * http://www.ti.com/general/docs/lit/getliterature.tsp?genericPartNumber=CD74HC595&&fileType=pdf
 * 
 */
 
#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4

void setup() 
{
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);  
}

void loop() 
{
  // Output from 0 to 255
  for( int i = 0; i <= 255; i++)
  {
    shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, i);   
    shiftLatch();
    delay(50);
  }  
  delay(500);
  
  // Output from 255 to 0
  for( int i = 255; i >= 0; i--)
  {
    shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, i);
    shiftLatch();
    delay(50);
  }
  delay(500);
}

/*
 * Write the contents of the shift register to 
 * the output register to view the data.
 */
void shiftLatch()
{
    digitalWrite(SHIFT_LATCH, LOW);
    digitalWrite(SHIFT_LATCH, HIGH);
    digitalWrite(SHIFT_LATCH, LOW);
}
