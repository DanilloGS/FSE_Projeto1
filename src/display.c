#include "display.h"

int fd;

void print_display(float TI,float TR, float TE)   {

  if (wiringPiSetup () == -1) exit (1);

  fd = wiringPiI2CSetup(I2C_ADDR);

  connect_display(); // setup LCD
  displayLoc(LINE1);
  typeln("TI");
  typeFloat(TI);
  typeln(" TR");
  typeFloat(TR);
  displayLoc(LINE2);
  typeln("TE");
  typeFloat(TE);
}

// float to string
void typeFloat(float myFloat)   {
  char buffer[20];
  sprintf(buffer, "%4.2f",  myFloat);
  typeln(buffer);
}


// clr lcd go home loc 0x80
void ClrDisplay(void)   {
  display_byte(0x01, LCD_CMD);
  display_byte(0x02, LCD_CMD);
}

// go to location on LCD
void displayLoc(int line)   {
  display_byte(line, LCD_CMD);
}

// this allows use of any size string
void typeln(const char *s)   {

  while ( *s ) display_byte(*(s++), LCD_CHR);

}

void display_byte(int bits, int mode)   {

  //Send byte to data pins
  // bits = the data
  // mode = 1 for data, 0 for command
  int bits_high;
  int bits_low;
  // uses the two half byte writes to LCD
  bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT ;
  bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT ;

  // High bits
  wiringPiI2CReadReg8(fd, bits_high);
  display_toggle_enable(bits_high);

  // Low bits
  wiringPiI2CReadReg8(fd, bits_low);
  display_toggle_enable(bits_low);
}

void display_toggle_enable(int bits)   {
  // Toggle enable pin on LCD display
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits | ENABLE));
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
  delayMicroseconds(500);
}


void connect_display()   {
  // Initialise display
  display_byte(0x33, LCD_CMD); // Initialise
  display_byte(0x32, LCD_CMD); // Initialise
  display_byte(0x06, LCD_CMD); // Cursor move direction
  display_byte(0x0C, LCD_CMD); // 0x0F On, Blink Off
  display_byte(0x28, LCD_CMD); // Data length, number of lines, font size
  display_byte(0x01, LCD_CMD); // Clear display
  delayMicroseconds(500);
}
