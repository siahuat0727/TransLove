/*
 * File:   white_fan.c
 * Author: siahuat0727
 *
 * Created on 20171221
 */

// PIC18F4520 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = INTIO67      // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = ON       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 1        // Watchdog Timer Postscale Select bits (1:1)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <pic18f4520.h>

#define MAX_DATA 600
#define NUM_DISPLAY 202


unsigned char data[MAX_DATA] = {
        0b01111111,
        0b01111111,
        0b00110000,
        0b00011000,
        0b00110000,
        0b01111111,
        0b01111111,
        0b00000000,
        0b00111000,
        0b01111100,
        0b01010100,
        0b01010100,
        0b01011100,
        0b00011000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b01000001,
        0b01111111,
        0b01111111,
        0b01000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00111000,
        0b01111100,
        0b01000100,
        0b01000100,
        0b01101100,
        0b00101000,
        0b00000000,
        0b00000000,
        0b00111000,
        0b01111100,
        0b01000100,
        0b01000100,
        0b01111100,
        0b00111000,
        0b00000000,
        0b00000000,
        0b01111100,
        0b01111100,
        0b00011000,
        0b00110000,
        0b00011100,
        0b01111100,
        0b01111000,
        0b00000000,
        0b00111000,
        0b01111100,
        0b01010100,
        0b01010100,
        0b01011100,
        0b00011000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000100,
        0b00111110,
        0b01111111,
        0b01000100,
        0b01000100,
        0b00100000,
        0b00000000,
        0b00111000,
        0b01111100,
        0b01000100,
        0b01000100,
        0b01111100,
        0b00111000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b01000000,
        0b01101000,
        0b01111010,
        0b11111111,
        0b11111111,
        0b01111010,
        0b01101000,
        0b01000000
    };
unsigned char* NOW = (unsigned char*)data + 96;
unsigned char* TAIL = data + MAX_DATA - 1;
//unsigned char temp;

// easy ring buffer 
int stringC = 0;
unsigned char global_char;

//Some easy function(init...etc)
void Myusartwrite(unsigned char);
void MyusartRead();
void Myusartinit();
void ISR_Init();
void Mylab10_2();
void LED_DISP(unsigned char* start);
void show_words();
unsigned char* prev(unsigned char* start);
unsigned char* next(unsigned char* ptr);
void add_word(unsigned char c);
void init_word();

void main(void) {
    OSCCONbits.IRCF = 0b110;
    TRISD = 0;
    TRISA = 0;
    Myusartinit();
    ISR_Init();
//    for(int i = 0; i < 256; ++i)
//        data[i] = (char)i;
    unsigned char* start = (unsigned char*)data;
//    int count;
    while(1){
//        if(++count & 0b1)
            start = next(start);
        LED_DISP(start);
    }
    return;
    
}

// start from char* start
void LED_DISP(unsigned char* start){
    unsigned char* iter = start;
    for(int i = 0; i < NUM_DISPLAY; ++i){
        LATA = *iter;
        int bright = 30;
        while(bright--);
        LATA = 0;
        int dark = 30;
        while(dark--);
        iter = prev(iter);
    }
    int delay = 0;
    while(delay--);
}

void add_word(unsigned char c){
    *NOW = c;
    NOW = next(NOW);
}

unsigned char* prev(unsigned char* ptr){
    if(ptr == (unsigned char*)data)
        return TAIL;
    return ptr-1;
}

unsigned char* next(unsigned char* ptr){
    if(ptr == TAIL)
        return (unsigned char*)data;
    return ptr+1;
}

void init_word(){
    add_word((unsigned char)255);
    add_word((unsigned char)255);
    add_word((unsigned char)200);
    add_word((unsigned char)128);
    add_word((unsigned char)64);
}


void ISR_Init()
{
    //TODO for intrrupt
    RCONbits.IPEN = 1;      //enable Interrupt Priority mode
    INTCONbits.GIEH = 1;    //enable high priority interrupt
    INTCONbits.GIEL = 1;     //enable low priority interrupt
            
    return ;
}
void MyusartRead()
{
    //TODO
    global_char = RCREG;
    add_word(global_char);
    
    return ;
}
void Myusartwrite(unsigned char a)
{
    //TODO
    TXREG = a;       //write to TXREG will send data 
    return ;
}
void Myusartinit()
{
    /*portC pin6/7 is mult with usart setting their tris*/    
    TRISCbits.TRISC6 = 1;//???;            // Setting by data sheet 
    TRISCbits.TRISC7 = 1;//???;            //  
    
    /*
           Serial Setting      
     * 1.   Setting Baud rate
     * 2.   choose sync/async mode 
     * 3.   enable Serial port (configures RX/DT and TX/CK pins as serial port pins)
     * 3.5  enable Tx, Rx Interrupt(optional)
     * 4.   Enable Tx & RX
     */      
    
    
    //  Setting baud rate 8-bit/Asynchronous/9600
    BAUDCONbits.BRG16 = 0 ;          //Read Baud rate table
    TXSTAbits.BRGH = 1;
    SPBRG = 25;      
    
   //   Serial enable
    TXSTAbits.SYNC = 0;             //choose the async moode
    RCSTAbits.SPEN = 1;              //open serial port
    
    // Interrutp Setting 
    
    //setting TX/RX
    PIR1bits.TXIF = 1; //1 = The EUSART transmit buffer, TXREG, is empty (cleared when TXREG is written), 0 = The EUSART transmit buffer is full
    PIR1bits.RCIF = 0; //1 = The EUSART receive buffer, RCREG, is full (cleared when RCREG is read), 0 = The EUSART receive buffer is empty
    TXSTAbits.TXEN = 1;          //Enable Tx
    RCSTAbits.CREN = 1;            //Enable Rx
    //setting TX/RX interrupt
    PIE1bits.TXIE = 0;              //Tx interrupt
    IPR1bits.TXIP = 1;              //Setting Tx as high/low priority interrupt 1 high, 0 low
    PIE1bits.RCIE = 1;              //Rx interrupt
    IPR1bits.RCIP = 1;              //Setting Rc as high/low priority interrupt 1 high, 0 low
    
    
    return ;
}
void interrupt  Hi_ISR(void)
{
    if(RCIF)
    {
        MyusartRead();
        Myusartwrite(global_char);
    }
    return ;
}

