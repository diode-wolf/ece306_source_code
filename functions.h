//******************************************************************************
//
//  Description: This file contains the Function prototypes
//
//  Created by James Sorber - 11/18/2023
//  Modified by James Sorber - 3/31/2023 
//  Last built with Code Composer Studio (12.4.0)
//******************************************************************************

// Functions
// Main
void main(void);

// Run to Completion Scheduler
void RTCS_Init(void);
int RTCS_Add_Task(void (*task)(void), int priority, int period, int ttnr);
void RTCS_Timer_Tick(void);
int RTCS_Get_Index(void (*task)(void));
void RTCS_Enable_Task(void (*task)(void), char enable);
void RTCS_Set_Task_Period(void (*task)(void), int period, char release_now);
void RTCS_Release_Task(void (*task)(void));
void Enter_RTCS_Scheduler(void);

// tasks
void Process_Serial(void);

// Initialization
//void Init_Conditions(void);

// Follow Line
//void Follow_Line(void);

// Menus
// void Update_Menus(void);
// void Update_Menu_1(void);
// void Update_Menu_2(void);
// void Update_Menu_3(void);
// void Update_Menu_4(void);
// void Add_Pointer(char line);

// Interrupts
void enable_interrupts(void);
// __interrupt void Timer0_B0_ISR(void);
// __interrupt void switch_interrupt(void);

// Timers
void Init_Timers(void);
void Init_Timer_B0(void);
void Init_Timer_B1(void);
void Init_Timer_B2(void);
void Init_Timer_B3(void);

// Fun stuff
// void Lights_And_Sirens(void);

// Analog to Digital Converter
void Init_ADC(void);

// IOT Module
// void Process_IOT(void);
// void Reset_IOT(void);

// Digital to Analog Converter
void Init_Ref(void);
void Init_DAC(void);

// Serial
void Init_Serial(void);
void Init_Serial_UCA0(void);
void Init_Serial_UCA1(void);
void Transmit(char port, char* target);
char Search_String(char* target);
void BCD_Out(unsigned int input, char* output, char len);

// Clocks
void Init_Clocks(void);

// Commands
// void Command_Process(char char_recived);
// void Execute_Local(void);

// ADC
void Check_ADC(void);

// LED Configurations
// void Init_LEDs(void);
// void IR_LED_control(char selection);
// void Backlite_control(char selection);

// Movement
//------------------------------------------------------------------------------
void Calculate_Movement(void);

// LCD
//------------------------------------------------------------------------------
void BDC_Out(int raw_value, char line_to_write, char pos_in_line);
void BDC_Clock_Out(long int raw_time, char line_to_write, char pos_in_line);
void Update_Clock(void);
void Display_Process(void);
void Update_Display(void);
void Display_Update(char p_L1,char p_L2,char p_L3,char p_L4);
void enable_display_update(void);
void update_string(char *string_data, int string);
void Init_LCD(void);
void lcd_clear(void);
void lcd_putc(char c);
void lcd_puts(char *s);

void lcd_power_on(void);
void lcd_write_line1(void);
void lcd_write_line2(void);
//void lcd_draw_time_page(void);
//void lcd_power_off(void);
void lcd_enter_sleep(void);
void lcd_exit_sleep(void);
//void lcd_write(unsigned char c);
//void out_lcd(unsigned char c);

void Write_LCD_Ins(char instruction);
void Write_LCD_Data(char data);
void ClrDisplay(void);
void ClrDisplay_Buffer_0(void);
void ClrDisplay_Buffer_1(void);
void ClrDisplay_Buffer_2(void);
void ClrDisplay_Buffer_3(void);

void SetPostion(char pos);
void DisplayOnOff(char data);
void lcd_BIG_mid(void);
void lcd_BIG_bot(void);
void lcd_120(void);

void lcd_4line(void);
void lcd_out(char *s, char line, char position);
void lcd_rotate(char view);

//void lcd_write(char data, char command);
void lcd_write(unsigned char c);
void lcd_write_line1(void);
void lcd_write_line2(void);
void lcd_write_line3(void);

void lcd_command( char data);
void LCD_test(void);
void LCD_iot_meassage_print(int nema_index);

// Menu
void Init_Menus(void);
void Update_Menu(void);

// Ports
void Init_Ports(void);
void Init_Port1(void);
void Init_Port2(void);
void Init_Port3(char smclk);
void Init_Port4(void);
void Init_Port5(void);
void Init_Port6(void);

// SPI
void Init_SPI_B1(void);
void SPI_B1_write(char byte);
void spi_rs_data(void);
void spi_rs_command(void);
void spi_LCD_idle(void);
void spi_LCD_active(void);
void SPI_test(void);
void WriteIns(char instruction);
void WriteData(char data);

// Switches
void Init_Switches(void);
void switch_control(void);
void enable_switch_SW1(void);
void enable_switch_SW2(void);
void disable_switch_SW1(void);
void disable_switch_SW2(void);
void Switches_Process(void);
void Init_Switch(void);
void Switch_Process(void);
void Switch1_Process(void);
void Switch2_Process(void);
void menu_act(void);
void menu_select(void);

void usleep(unsigned int usec);
void usleep10(unsigned int usec);
void five_msec_sleep(unsigned int msec);
void measure_delay(void);
void out_control_words(void);
