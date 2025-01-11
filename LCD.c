//------------------------------------------------------------------------------
//
//  Description: This file contains the LCD Routine -
//
//
//  Jim Carlson
//  Aug 2023
//  Built with TI Code Composer Version: 12.5.0
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "header_files/functions.h"
#include  "msp430.h"
#include  "header_files/ports.h"
#include  "header_files/lcd.h"
#include <string.h>


#define LCD_INTERVAL        (12500) // 8,000,000 / 8 / 8 / [1/100msec] = 12500
#define SPI_CLK_PRESCALER      (80)

// Function Prototypes
void LCD_test(void);
void spi_chipselect_active(void);
void spi_chipselect_idle(void);
void spi_reset_idle(void);
void spi_reset_active(void);
void Display_Process(void);
void Display_Update(char p_L1,char p_L2,char p_L3,char p_L4);
void enable_display_update(void);
void update_string(char *string_data, int string);
void Init_LCD(void);
void lcd_4line(void);
void lcd_BIG_mid(void);
void lcd_BIG_bot(void);
void lcd_180(void);
void lcd_out(char *s, char line, char position);
void lcd_puts(char *s);
void WriteIns(char instruction);
void WriteData(char data);
unsigned char CheckBusy(void);
void ClrDisplay(void);
void ClrDisplay_Buffer_0(void);
void ClrDisplay_Buffer_1(void);
void ClrDisplay_Buffer_2(void);
void ClrDisplay_Buffer_3(void);
void SetPostion(char pos);
void DisplayOnOff(char data);
void Init_SPI_B1(void);
void SPI_B1_write(char byte);

// Variables =====================================================================
volatile unsigned char current_character;
volatile unsigned int update_display_count;
volatile unsigned int display_update_time;
volatile unsigned char update_display;
volatile unsigned char display_changed;
unsigned char lcd_control;
char display_line[4][11];
char *display[4];
char posi_L1;
char posi_L2;
char posi_L3;
char posi_L4;
char lcd_orient;
unsigned int char_count;
char current_line;

extern volatile unsigned char SPI_Char;
extern volatile unsigned char current_character;

void LCD_test(void){
//  unsigned int temporary_value;
//  int temp;
  if (wait_for_character()){
    print_CR();
    return;
  }
  outchar(current_character);
  switch (current_character) {
    case 0x30:  // 0
    break;
    case 0x33:  // 3 3 lines of Text
    lcd_BIG_mid();
    break;
    case 0x34:  // 4 4 Lines of Text
    lcd_4line();
    break;
    case 0x49:  // I
    case 0x69:  // i
    Init_LCD();
    break;
    default:
    break;
  } // end of character determination Switch
  current_character = 0x00;
//------------------------------------------------------------------------------
}

void spi_chipselect_idle(void){
  __delay_cycles(600);
  P4OUT |= UCB1_CS_LCD;
}

void spi_chipselect_active(void){
  P4OUT &= ~UCB1_CS_LCD;
  __delay_cycles(10);
}

void spi_reset_idle(void){
  P2OUT |= RESET_LCD;
}

void spi_reset_active(void){
  P2OUT &= ~RESET_LCD;
}

//void Display_Process(void){
//  if(update_display){
//    update_display = 0;
//    if(display_changed){
//      display_changed = 0;
//      Display_Update(0,0,0,0);
//    }
//  }
//}

void Display_Update(char p_L1,char p_L2,char p_L3,char p_L4){
  lcd_out(display_line[0], LCD_HOME_L1, p_L1);
  lcd_out(display_line[1], LCD_HOME_L2, p_L2);
  lcd_out(display_line[2], LCD_HOME_L3, p_L3);
  lcd_out(display_line[3], LCD_HOME_L4, p_L4);
}

void enable_display_update(void){
// Display Update Timer
  TB0CCR2 = LCD_INTERVAL;     // CCR2
  TB0CCTL2 |= CCIE;           // CCR2 enable interrupt
}

void update_string(char *string_data, int string){
  int i =0;
  while (*string_data) {
    display_line[string][i++] = *string_data++;
  }
}

void Init_LCD(void){
//------------------------------------------------------------------------------
  Init_SPI_B1();  // Initialize SPI Port
// turn on power
  spi_reset_idle();
  spi_reset_active();
  __delay_cycles(500000);

// delay 8000 * [1/8MHz]
  __delay_cycles(80000);

// delay 300000 * [1/8MHz]
  __delay_cycles(500000);
  spi_reset_idle();
// delay 300000 * [1/8MHz]
  __delay_cycles(300000);

// init Display
  WriteIns(0x3A);    //8-Bit data length extension Bit RE=1; REV=0
  WriteIns(0x09);    //4 line display
  WriteIns(0x06);    //Bottom view
  WriteIns(0x1E);    //Bias setting BS1=1
  WriteIns(0x39);    //8-Bit data length extension Bit RE=0; IS=1
  WriteIns(0x1B);    //BS0=1 -> Bias=1/6
  WriteIns(0x6E);    //Divider on and set value
  WriteIns(0x57);    //Booster on and set contrast (BB1=C5, DB0=C4)
  WriteIns(0x7F);    //Set contrast (DB3-DB0=C3-C0)
  WriteIns(0x38);    //8-Bit data length extension Bit RE=0; IS=0
  ClrDisplay();
  DisplayOnOff(DISPLAY_ON);
// delay 8000 * [1/8MHz]
  __delay_cycles(8000);

}

void lcd_4line(void){
//  0x3A ->Function set: RE = 1
  WriteIns(0x38);	//4 Lines middle
  display_changed = 1;
}

void lcd_BIG_mid(void){
//  0x3A ->Function set: RE = 1
  WriteIns(0x3A);	//3 Lines middle
//  0x17 -> 3 Lines middle
  WriteIns(0x17);	//3 Lines middle
//  0x3C ->Function set: RE = 0, DH = 1
  WriteIns(0x3C);	//3 Lines middle
  display_changed = 1;
}

void lcd_BIG_bot(void){
//  0x3A ->Function set: RE = 1
  WriteIns(0x3A);	//3 Lines middle
//  0x13 -> 3 Lines bot
  WriteIns(0x13);	//3 Lines middle
//  0x3C ->Function set: RE = 0, DH = 1
  WriteIns(0x3C);	//3 Lines middle
  display_changed = 1;
}

void lcd_180(void){
  WriteIns(0x3A);	// 0x3A ->Function set: RE = 1 Enable Change
  if(lcd_orient){
    lcd_orient = 0;
    WriteIns(0x06);	// 0x06 -> Bottom View
  }else{
    lcd_orient = 1;
    WriteIns(0x05);	//  0x05 -> Top View
  }
  WriteIns(0x3C);	// 0x3C ->Function set: RE = 0, DH = 1 Disable Change
  display_changed = 1;
}

void lcd_out(char *s, char line, char position){
// Write a string to the display, using lcd_putc.
  WriteIns(line+position);
  while (*s) {
    WriteData(*s++);
  }
}

void lcd_puts(char *s){
// Write a string to the display, using lcd_putc.
  char_count = 1;
  if (s == NULL){
    return;
  }
  while (*s) {
    WriteData(*s++);
  }
}

void WriteIns(char instruction){
//-----------------------------------------------------
// Func: WriteIns(instruction)
// Desc: sends instruction to display
//-----------------------------------------------------
  spi_chipselect_active();
  SPI_B1_write(START_WR_INSTRUCTION);    // Send 5 synchronization bits, RS = 0, R/W = 0
  SPI_B1_write(instruction & 0x0F);      // Send lower data bits
  SPI_B1_write((instruction>>4) & 0x0F); // Send higher data bits
  spi_chipselect_idle();
}

void WriteData(char data){
//-----------------------------------------------------
//Func: WriteData(data)
//Desc: sends data to display
//-----------------------------------------------------
  spi_chipselect_active();
  //  CheckBusy();
  SPI_B1_write(START_WR_DATA);      // Send 5 synchronization bits, RS = 1, R/W = 0
  SPI_B1_write(data & 0x0F);        // Send lower data bits
  SPI_B1_write((data>>4) & 0x0F);   // Send higher data bits
  spi_chipselect_idle();
}

unsigned char CheckBusy(void){
//-----------------------------------------------------
//Func: CheckBusy()
//Desc: checks if display is idle
//-----------------------------------------------------
  unsigned char readData = 1;
  //  do{
  //    SPI_put(0x3F);         //Send 5 synchronization bits, RS = 0, R/W = 1
  //    SPI_put(0x00);         //dummy write to receive data
  //    while(ri_u0c1 == 0);   //wait while data is received
  //    readData= ~u0rbl;      //store data
  //  }while(readData&0x80);   //check for busy flag
  //
  return readData;
}

void ClrDisplay(void){
//-----------------------------------------------------
// Func: ClrDisplay
// Desc: Clears entire Display content and set home pos
//-----------------------------------------------------
  ClrDisplay_Buffer_0();
  ClrDisplay_Buffer_1();
  ClrDisplay_Buffer_2();
  ClrDisplay_Buffer_3();
  display_changed = 0;
}

void ClrDisplay_Buffer_0(void){
  int i;
  for(i=0;i<10;i++){
    display_line[0][i] = ' ';
  }
}

void ClrDisplay_Buffer_1(void){
  int i;
  for(i=0;i<10;i++){
    display_line[1][i] = ' ';
  }
}

void ClrDisplay_Buffer_2(void){
  int i;
  for(i=0;i<10;i++){
    display_line[2][i] = ' ';
  }
}

void ClrDisplay_Buffer_3(void){
  int i;
  for(i=0;i<10;i++){
    display_line[3][i] = ' ';
  }
}

void SetPostion(char pos){
//-----------------------------------------------------
// Func: SetPostion(postion)
// Desc: set postion
//-----------------------------------------------------
  WriteIns(LCD_HOME_L1+pos);
  display_changed = 0;
}

void DisplayOnOff(char data){
//-----------------------------------------------------
// Func: DisplayOnOff(control)
// Desc: use definitions of header file to set display
//-----------------------------------------------------
  WriteIns(0x08+data);
  display_changed = 0;
}

void Init_SPI_B1(void){
//------------------------------------------------------------------------------
//SPI
//------------------------------------------------------------------------------
// Reset peripheral
  UCB1CTLW0 = UCSWRST;
  UCB1CTLW0 |= UCSSEL__SMCLK; // SMCLK is clock source
  UCB1BRW = SPI_CLK_PRESCALER;
  UCB1CTLW0 |= UCCKPL;        // Clock polarity high
  UCB1CTLW0 |= UCMST;         // master mode
  UCB1CTLW0 |= UCSYNC;
  UCB1CTLW0 |= UCMODE_0;
  UCB1CTLW0 &= ~UCMSB;        // master mode, LSB transmitted first
  // Enable peripheral
  UCB1CTLW0 &= ~UCSWRST;
  // Wait to ensure the peripheral is up
  __delay_cycles(100);
//------------------------------------------------------------------------------
}

void SPI_B1_write(char byte){
//------------------------------------------------------------------------------
// Writes a byte out of SPIB, assumes CSB already low
// Wait for TX empty
  while (!(UCB1IFG & UCTXIFG));
  UCB1TXBUF = byte;
  // Wait for TX empty
  while (!(UCB1IFG & UCTXIFG));
  current_character = UCB1RXBUF;
  //------------------------------------------------------------------------------
}



