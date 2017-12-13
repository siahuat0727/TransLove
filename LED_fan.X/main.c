#include <xc.h>

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
#define ONE_CYCLE 2200
#define HALF_CYCLE (ONE_CYCLE >> 1)
#define QUATER_CYCLE (HALF_CYCLE >> 1) // 550
#define STATE_CHANGE (1 << 7) // 128

void main(void) {
    TRISA = 0; // set as output
    TRISD = 0; // set as output
    LATD  = 0;
    LATA = 1;
    int four[] = {1,0,0,0};
    
//    long int delay = 1 << 12;
//    while(delay--)
//        LATA = ~LATA;
    int state = 0;
    long int state_change_cycle = STATE_CHANGE;
    int move = QUATER_CYCLE - 10; // TODO change magic integer
    while(1){
        if(move > 0)
            move--;
        
        // loop for one cycle
        for(int i = 0; i < 4; ++i){
            if(four[i])
                LATA = 0xFF;
            int bright = 10;
            while(bright--);
            LATA = 0x00;
            int dark = QUATER_CYCLE - bright + ( i&1 ? move : -move);
            while(dark--);
        }
        
        
        // state
        state_change_cycle--;
        if(state_change_cycle == 0){
            state_change_cycle = STATE_CHANGE;
            state++;
            state %= 3;
            move = QUATER_CYCLE - 10; // TODO change magic integer
            switch(state){
                case 2:
                    four[0] = 1;
                    four[1] = 1;
                    four[2] = 1;
                    four[3] = 1;
                    break;
                case 1:
                    four[0] = 1;
                    four[1] = 0;
                    four[2] = 1;
                    four[3] = 0;
                    break;
                case 0:
                    four[0] = 1;
                    four[1] = 0;
                    four[2] = 0;
                    four[3] = 0;
                default:
                    break;
            }
            
        }
    }
}



//void interrupt tc_int(void)     // High priority interrupt
//{
//    if(TMR1IE && TMR1IF){
//        count++;
//        TMR1IF=0;
//    }  
//    
//}