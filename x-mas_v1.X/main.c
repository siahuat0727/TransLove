/*
 * File:   main.c
 * Author: HoeYuan
 *
 * Created on December 16, 2017, 11:21 PM
 */

#include <xc.h>
#include <pic18f4520.h>
#include <stdlib.h>

// CONFIG1H
#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
// CONFIG3H
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
// CONFIG4L
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)

#define MAX_DATA 250

void UART_WRITE(char);
void UART_READ();
void UART_INIT();
void ISR_INIT();
void LED_DISP();
void show_words();
char* next(char* ptr);
void add_word(char c);

char data[MAX_DATA] = "Welcome to TransLove <3 <3 <3";
char* now = (char*)data;
char* tail = data + MAX_DATA;
char temp;

void test(){
    LATA = 0xFF;
    long int bright = 200;
    while(bright--);
    LATA = 0;
    long int dark = 18000;
    while(dark--);
}

void main(void) {
    // set frequency to 8MHz
    OSCCONbits.IRCF = 0x7;
    
    //set port a as output
    TRISD = 0;
    TRISA = 0;
    
    // init uart and isr
    UART_INIT();
    ISR_INIT();
    while(1)
        test();
    LED_DISP();
    // main
    while (1) 
        show_words();
    return;
}

void show_words(){
    char* iter = now;
    for(int i = 0; i < MAX_DATA; ++i){
        LATA = *iter;
        int bright = 50;
        while(bright--);
        int dark = 50;
        while(dark--);
        iter = next(iter);
    }
}

void add_word(char c){
    *now = c;
    now = next(now);
}

char* next(char* ptr){
    if(++ptr == tail)
        return (char*)data;
    return ptr;
}

void LED_DISP(){
    add_word((char)255);
    add_word((char)255);
    add_word((char)200);
    add_word((char)128);
    add_word((char)64);
}

void UART_WRITE(char output) {
    TXREG = output;
    return;
}

void UART_READ() {
    temp = RCREG;
    
    // update array
    add_word(temp);
    
    return;
}

void UART_INIT() {
    // setup port 6 & 7 as 1
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;
    
    //set bound rate as 9600
    BAUDCONbits.BRG16 = 0;
    TXSTAbits.BRGH = 1;
    SPBRG = 25;
    
    // setup serial mode
    TXSTAbits.SYNC = 0;
    
    // open serial port
    RCSTAbits.SPEN = 1;
    
    // enable RX/TX
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;
    
    // set TX/RX interrupt
    PIE1bits.TXIE = 0; // TX interrupt
    IPR1bits.TXIP = 1; // TX interrupt priority(1 = high, 0 = low)
    PIE1bits.RCIE = 1; // RX interrupt
    IPR1bits.RCIP = 1; // RX interrupt priority(1 = high, 0 = low)
}

void ISR_INIT() {
    RCONbits.IPEN = 1; // enable interrupt priority mode
    INTCONbits.GIEH = 1; // enable high priority interrupt
    INTCONbits.GIEL = 1; // enable low priority interrupt
    return;
}

void interrupt ISR_HIGH(void) {
    if (RCIF) {
        if (RCSTAbits.OERR == 1) {
            
        }
        UART_READ();
        UART_WRITE(temp);
    }    
    return ;
}

void interrupt low_priority ISR_LOW(void) {
    return;
}