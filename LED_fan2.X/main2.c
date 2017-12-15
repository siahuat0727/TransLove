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

typedef int bool;
#define true 1
#define false 0

// for timer
int count=0;

//
short minutes;
short hours;

//#define _XTAL_FREQ 8000000
//#define ONE_CYCLE 8200//(7340\40\20) // pure is 6680  //順時要減少，逆時要增加
//#define HALF_CYCLE (ONE_CYCLE >> 1)
//#define QUATER_CYCLE (ONE_CYCLE >> 2)
//#define HALF_QUATER_CYCLE (ONE_CYCLE >> 3)
//#define STATE_CHANGE (1 << 7) // 128
//#define BRIGHT_INTERVAL 60
//#define DARK_INTERVAL 60
//#define MOVE_START (QUATER_CYCLE - 1080)//(HALF_QUATER_CYCLE - (9*(BRIGHT_INTERVAL + DARK_INTERVAL)))


int tree[2][5] ={ 
                { 0b01100100,
                0b01110110,
                0b11111111,
                0b01110110,
                0b01100100
                },
                { 0b00000000,
                0b01110110,
                0b11111111,
                0b01110110,
                0b00000000
                }
                };


void mode1(){
#define QUATER_CYCLE 2072  //順時要減少(指令太多)，逆時要增加
#define STATE_CHANGE 160
#define BRIGHT_INTERVAL 60
#define DARK_INTERVAL 60
#define MOVE_START 992 // 2100 - 1080 //QUATER_CYCLE - (9*(BRIGHT_INTERVAL + DARK_INTERVAL)))    
#define WALKER_PIC 5
#define WALKER_COLUMN 9
    
    int walker[WALKER_PIC][WALKER_COLUMN] =
        {
            {
            0b0000000000,
            0b1000000000,
            0b0100100000,
            0b0011010000,
            0b0001111111,
            0b1010100111,
            0b0100010000,
            0b0000000000,
            0b0000000000
        },
        {
            0b0000000000,
            0b0010000000,
            0b0100110000,
            0b0010010000,
            0b0001111111,
            0b1010010111,
            0b0100100000,
            0b0000100000,
            0b0000000000
        },
        {
            0b0010000000,
            0b0100001000,
            0b0010001000,
            0b0001010000,
            0b0010111111,
            0b1100010111,
            0b0000100000,
            0b0001000000,
            0b0000000000
        },
        {
            0b0000000000,
            0b0101000000,
            0b0100100000,
            0b0010010000,
            0b1101111111,
            0b0001010111,
            0b0000100000,
            0b0000000000,
            0b0000000000
        },
        {
            0b0000000000,
            0b1000000000,
            0b0100000000,
            0b1010110000,
            0b0101111111,
            0b0000110111,
            0b0000000000,
            0b0000000000,
            0b0000000000
        }
      };

    bool four[4] = {true};
    int state_change_cycle = STATE_CHANGE;
    int state = 0;
    long int move = MOVE_START; 
    bool update = false;
    bool update_next_time = false;
    const int pic_cycle = 3;
    int pic_cycle_i = 0;
    int pic_i = 0;
    unsigned int* show;
    
    long int display_cycle = -1;
    while(display_cycle--){
        if(move > 0)
            move -= 30;
        else{
            int delay = 20;
            while(delay--);
        }
        if(update_next_time == true){
            update_next_time = false;
            // delete walker
            switch(state){
                case 4:
                    four[2] = false;
                case 3:
                    four[1] = four[3] = false;
            }
        }
        if(update == false && move <= 0){
            update = true;
            update_next_time = true;
            move = 0;
        }
        
        // loop for one cycle
        int i;
        for(i = 0; i < 4; ++i){
            // adjust delay for each state
            int delay;
            switch(state){
                case 0:
                    delay = 28;
                    while(delay--);
                    break;
                case 1:
                    delay = 15;
                    while(delay--);
                    break;
                case 2:
                    delay = 0;
                    while(delay--);
                    break;
                case 3:
                    delay = 10;
                    while(delay--);
                    break;
                case 4:
                    delay = 16;
                    while(delay--);
                    break;
            }
            
            show = (unsigned int*)walker[pic_i];
            for(int column = 0; column < WALKER_COLUMN; ++column, ++show){
                if(four[i]){
                    LATA = *show;
                    LATD = (*show) >> 8;
                }
                else{
                    LATA = 0;
                    LATD = 0;
                }
                int bright = BRIGHT_INTERVAL;
                while(bright--);
                LATA = 0;
                LATD = 0;
                int dark = DARK_INTERVAL;
                while(dark--);
            }
            
            // adjust dark interval
            long int dark = MOVE_START;
            if(state == 1 || state == 2){
                if((state==1 && i<2) || (state==2 && (i==0||i==2)))
                    dark -= move;
                else
                    dark += move;
            }else if(state != 0){
                if((state==4 && i<2) || (state==3 && (i==0||i==2)))
                    dark += MOVE_START - move;
                else
                    dark -= MOVE_START - move;
            }
            while(dark--);
        }
        
        if(++pic_cycle_i == pic_cycle){
            pic_cycle_i = 0;
            if(++pic_i == WALKER_PIC)
                pic_i = 0;
        }
        
        // change state
        if(state_change_cycle-- == 0){
            state_change_cycle = STATE_CHANGE;
            move = MOVE_START;
            update = false;
            state++;
            state %= 5;
            for(i = 0; i < 4; ++i)
                four[i] = 0;
            switch(state){
                case 2:
                case 3:
                    four[1] = four[3] = 1;
                case 1:
                case 4:
                    four[2] = 1;
                case 0:
                    four[0] = 1;
					break;
            }
        }
    }
#undef QUATER_CYCLE
#undef STATE_CHANGE
#undef BRIGHT_INTERVAL
#undef DARK_INTERVAL
#undef MOVE_START
#undef WALKER_PIC
#undef WALKER_COLUMN
}

void mode2(){
#define QUATER_CYCLE 2072  //順時要減少(指令太多)，逆時要增加
#define STATE_CHANGE 160
#define BRIGHT_INTERVAL 60
#define DARK_INTERVAL 60
#define MOVE_START 992 // 2100 - 1080 //QUATER_CYCLE - (9*(BRIGHT_INTERVAL + DARK_INTERVAL)))
#define TREE_PIC
#define TREE_COLUMN
    
    bool four[4] = {false, false, true, false};
    int state = 0;
    int state_change_cycle = STATE_CHANGE;
    long int move = MOVE_START; 
    bool update = false;
    const int pic_cycle = 3;
    int pic_cycle_i = 0;
    int pic_i = 0;
    int* show;
    bool update_next_time = false;
    long int display_cycle = -1;
    while(display_cycle--){
        if(move > 0)
            move -= 30;
        else{
            int delay = 20;
            while(delay--);
        }
        if(update_next_time == true){
            update_next_time = false;
            // delete walker
            switch(state){
                case 4:
                    four[2] = false;
                case 3:
                    four[1] = four[3] = false;
            }
        }
        if(update == false && move <= 0){
            update = true;
            update_next_time = true;
            move = 0;
        }
        
        // loop for one cycle
        int i;
        for(i = 0; i < 4; ++i){
            // adjust delay for each state
            int delay;
            switch(state){
                case 0:
                    delay = 28;
                    while(delay--);
                    break;
                case 1:
                    delay = 15;
                    while(delay--);
                    break;
                case 2:
                    delay = 0;
                    while(delay--);
                    break;
                case 3:
                    delay = 10;
                    while(delay--);
                    break;
                case 4:
                    delay = 16;
                    while(delay--);
                    break;
            }
            
            show = (int*)walker[pic_i];
            for(int column = 0; column < WALKER_COLUMN; ++column, ++show){
                if(four[i]){
                    LATA = *show;
                    LATD = (*show) >> 8;
                }
                else{
                    LATA = 0;
                    LATD = 0;
                }
                int bright = BRIGHT_INTERVAL;
                while(bright--);
                LATA = 0;
                LATD = 0;
                int dark = DARK_INTERVAL;
                while(dark--);
            }
            
            // adjust dark interval
            long int dark = MOVE_START;
            if(state == 1 || state == 2){
                if((state==1 && i<2) || (state==2 && (i==0||i==2)))
                    dark -= move;
                else
                    dark += move;
            }else if(state != 0){
                if((state==4 && i<2) || (state==3 && (i==0||i==2)))
                    dark += MOVE_START - move;
                else
                    dark -= MOVE_START - move;
            }
            while(dark--);
        }
        
        if(++pic_cycle_i == pic_cycle){
            pic_cycle_i = 0;
            if(++pic_i == WALKER_PIC)
                pic_i = 0;
        }
        
        // state
        if(state_change_cycle-- == 0){
            state_change_cycle = STATE_CHANGE;
            move = MOVE_START;
            update = false;
            state++;
            state %= 5;
            for(i = 0; i < 4; ++i)
                four[i] = 0;
            switch(state){
                case 2:
                case 3:
                    four[1] = four[3] = 1;
                case 1:
                case 4:
                    four[2] = 1;
                case 0:
                    four[0] = 1;
					break;
            }
        }
    }
#undef QUATER_CYCLE
#undef STATE_CHANGE
#undef BRIGHT_INTERVAL
#undef DARK_INTERVAL
#undef MOVE_START
}

void display(int n){
    int num[10][5] = {
                {
               0b00011111,
               0b00010001,
               0b00010001,
               0b00010001,
               0b00011111 
                },
                {
               0b00010000,
               0b00010010,
               0b00011111,
               0b00010000,
               0b00010000 
                },
                {
               0b00011100,
               0b00010101,
               0b00010101,
               0b00010101,
               0b00010111 
                },
                {
               0b00010000,
               0b00010001,
               0b00010101,
               0b00010101,
               0b00011111 
                },
                {
               0b00000111,
               0b00000100,
               0b00000100,
               0b00011111,
               0b00000100 
                },
                {
               0b00010111,
               0b00010101,
               0b00010101,
               0b00010101,
               0b00011101 
                },
                {
               0b00011111,
               0b00010100,
               0b00010100,
               0b00010100,
               0b00011100 
                },
                {
               0b00000001,
               0b00000001,
               0b00000001,
               0b00000001,
               0b00011111 
                },
                {
               0b00011111,
               0b00010101,
               0b00010101,
               0b00010101,
               0b00011111 
                },
                {
               0b00000111,
               0b00000101,
               0b00000101,
               0b00000101,
               0b00011111 
                }
              };
    int *iter = (int*)num[n];
    for(int i = 0; i < 5; ++i, ++iter){
        LATA = *iter;
        int bright_i = 40;
        while(bright_i--);
        LATA = 0;
        int dark_i = 40;
        while(dark_i--);
    }
}

void mode_num(){
    while(1){
        for(int i = 0; i < 10; ++i){
            display(i);
            int dark = 255;
            while(dark--);
        }
        long int dark = 2300;
        while(dark--);
    }
}


void main(void) {
    OSCCONbits.IRCF = 0b111; // set to 8MHz
    
    // timer setting
    RCONbits.IPEN=0x01;
    T1CON = 0b10110101;//0x95; // prescale = 1:8
    TMR1 = 15536; // 50000 to interrupt (1 second = 250000)
    PIE1bits.TMR1IE=1;
    IPR1bits.TMR1IP=1;
    PIR1bits.TMR1IF=0;
    INTCONbits.GIE=1;
    
    TRISA = 0; // set as output
    TRISD = 0; // set as output
    LATD  = 0;
    LATA  = 0;
    mode1();
//    int num0[6] = NUM(0);
    while(1);
    mode1();
}

void display_clock(){
    long int display_cycle = -1;
    while(display_cycle--){
        
    }
}

void update_time(){
    if((minutes += 5)==60)
        if(++hours == 24)
            hours = 0;
    display_clock();
}

void interrupt tc_int(void)     // High priority interrupt
{
    if(TMR1IE && TMR1IF){
        TMR1 = 15535;
        TMR1IF=0;
        count++;
        if(count == 25){ // (value/5) seconds
            count = 0;
            update_time();
        }
    }  
    
}
