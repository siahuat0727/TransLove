#include <xc.h>
//#include <stdio.h>

// CONFIG1H
#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
// CONFIG3H
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
// CONFIG4L
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)

int count=0;
void main(void)
{
//    RCONbits.IPEN=0x01;
//    T1CON=0x95;
	TRISAbits.RA0 = 0; // set as output
	LATAbits.LA0 = 1;

//    while(1){
//        LATAbits.LA0 = ~LATAbits.LA0;
//
//        long int
//    }
//    PIE1bits.TMR1IE=1;
//    IPR1bits.TMR1IP=1;
//    PIR1bits.TMR1IF=0;
//
//    INTCONbits.GIE=1;
//
	while(1);
}

//void interrupt tc_int(void)     // High priority interrupt
//{
//    if(TMR1IE && TMR1IF){
//        count++;
//        TMR1IF=0;
//    }
//
//}