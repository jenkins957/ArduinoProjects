/**
 * Arduino EEPROM Programmer
 * Set up for Atmel AT28C64B EEPROM
 * Can read and write a byte of data to a given memory location.
 * Contents is outputted via serial pins.
 * 
 * Program used 2 x 74595 Shift registers to minimise number of pins required.
 */
#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

#define MAX_ADDRESS 0x1fff //13 address lines 0 - 8192

/*
 * Set up steps. The EEPROM is written or read in the set up
 */
void setup() 
{
   pinMode(SHIFT_DATA, OUTPUT);
   pinMode(SHIFT_CLK, OUTPUT);
   pinMode(SHIFT_LATCH, OUTPUT);  
   
   digitalWrite(WRITE_EN, HIGH);
   pinMode(WRITE_EN, OUTPUT);
   
   Serial.begin(57600);

   /*  
   writeEEprom(0, 0x12);
   writeEEprom(1, 0x34);
   writeEEprom(2, 0x56);
   */
   
   //resetEEprom();

   //printContents(0xFF, 0x1FF);
   printContents();
   Serial.end();
}

/*
 * Set the address lines and OE pin
 * OE is active low
 * 
 * outputEnabled false - Set OE high to go to input mode 
 * outputEnabled true - Set OE low to go to output mode
 */
void setAddress(int address, bool outputEnable)
{
    // OutputEnable on 2nd 595 pin 7
    shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address >> 8 | (outputEnable ? 0x00 : 0x80));
    shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);
    
    digitalWrite(SHIFT_LATCH, LOW);
    digitalWrite(SHIFT_LATCH, HIGH);
    digitalWrite(SHIFT_LATCH, LOW); 
}

byte readEEprom(int address)
{
    for( int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1 )
    {
        pinMode(pin, INPUT);
    }
  
    byte data = 0;
    setAddress(address, true);
    for( int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1 )
    {
        data = (data << 1) + digitalRead(pin);
    }

    return data;
}

/**
 * Reset memory locations to 0xff
 */
void resetEEprom()
{
    for( int address = 0; address <= MAX_ADDRESS; address += 1 )
    {
        writeEEprom(address, 0xff);
    }  
}

/*
 * Write a byte of data to specified address.
 */
void writeEEprom(int address, byte data)
{
    setAddress(address, false);    

    for( int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1 )
    {
        pinMode(pin, OUTPUT);
    }
        
    for( int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1 )
    {
        digitalWrite(pin, data & 1);
        data = data >> 1;
    }

    digitalWrite(WRITE_EN, LOW);
    delayMicroseconds(1);
    digitalWrite(WRITE_EN, HIGH);
    delay(15);
}

/*
 * Print contents in hex of all memory locations.
 */
void printContents() 
{
    for( int base = 0; base <= MAX_ADDRESS; base += 16 )
    {
       byte data[16];
       for( int offset = 0; offset <= 15; offset += 1)
       {
          data[offset] = readEEprom(base + offset);   
       }

       char buf[80];
       sprintf(buf, "%04x: %02x %02x %02x %02x %02x %02x %02x %02x    %02x %02x %02x %02x %02x %02x %02x %02x", 
       base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
             data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);
       Serial.println(buf);
    }
}

void printContents(int startAddress, int endAddress) 
{
    for( int base = startAddress; base <= endAddress; base += 16 )
    {
       byte data[16];
       for( int offset = 0; offset <= 15; offset += 1)
       {
          data[offset] = readEEprom(base + offset);   
       }

       char buf[80];
       sprintf(buf, "%04x: %02x %02x %02x %02x %02x %02x %02x %02x    %02x %02x %02x %02x %02x %02x %02x %02x", 
       base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
             data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);
       Serial.println(buf);
    }
}

void printContentsAtAddress(int address)
{
   byte b = readEEprom(address);
   char buf[80];
   sprintf(buf, "%04x: %02x", address, b); 
   
   Serial.println(buf);
}

/*
 * Main execution loop.
 */
void loop() 
{
    // Reading and wrting to EEPROM occurs in set up steps
    // as one-off activity 
}
