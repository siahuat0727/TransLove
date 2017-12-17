#include <xc.h>

// CONFIG1H
#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
// CONFIG3H
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
// CONFIG4L
#pragma config LVP = ON        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)

typedef int bool;
#define true 1
#define false 0
#define BRIGHT_INTERVAL 60
#define DARK_INTERVAL 60

// for timer
int count=0;

// time
int seconds;
int minutes;
int hours;

void mode_walker(){
#define QUATER_CYCLE 2072  //順時要減少(指令太多)，逆時要增加
#define STATE_CHANGE 160
#define MOVE_START 992 // 2072 - 1080 //QUATER_CYCLE - (9*(BRIGHT_INTERVAL + DARK_INTERVAL)))    
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
#undef MOVE_START
#undef WALKER_PIC
#undef WALKER_COLUMN
}
/*
void mode2(){
#define QUATER_CYCLE 2072  //順時要減少(指令太多)，逆時要增加
#define STATE_CHANGE 160
#define BRIGHT_INTERVAL 60
#define DARK_INTERVAL 60
#define MOVE_START 1472 // 2072 - 600 //QUATER_CYCLE - (9*(BRIGHT_INTERVAL + DARK_INTERVAL)))
#define TREE_PIC 2
#define TREE_COLUMN 5

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

    
    bool four[4] = {false, false, true, false};
    int state = 0;
    int state_change_cycle = STATE_CHANGE;
    long int move = MOVE_START; 
    bool update = false;
    const int pic_cycle = 3;
    int pic_cycle_i = 0;
    int pic_i = 0;
    unsigned int* show;
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
//            int delay;
//            switch(state){
//                case 0:
//                    delay = 28;
//                    while(delay--);
//                    break;
//                case 1:
//                    delay = 15;
//                    while(delay--);
//                    break;
//                case 2:
//                    delay = 0;
//                    while(delay--);
//                    break;
//                case 3:
//                    delay = 10;
//                    while(delay--);
//                    break;
//                case 4:
//                    delay = 16;
//                    while(delay--);
//                    break;
//            }
            
            show = (unsigned int*)tree[pic_i];
            for(int column = 0; column < TREE_COLUMN; ++column, ++show){
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
                    four[0] = four[3] = 1;
                case 1:
                case 4:
                    four[1] = 1;
                case 0:
                    four[2] = 1;
					break;
            }
        }
    }
#undef QUATER_CYCLE
#undef STATE_CHANGE
#undef BRIGHT_INTERVAL
#undef DARK_INTERVAL
#undef MOVE_START
#undef TREE_PIC
#undef TREE_COLUMN
}
*/

void mode_TransLove(){
#define ARROW_COLUMN 16
    unsigned char love[10] = {
                                0b00000000,
                                0b00001100,
                                0b00011110,
                                0b00111110,
                                0b01111100,
                                0b00111110,
                                0b00011110,
                                0b00001100,
                                0b00000000
                            };
    unsigned int arrow[ARROW_COLUMN] =  {
                                    0b00110010,
                                    0b00110110,
                                    0b00011100,
                                    0b00001000,
                                    0b00001000,
                                    0b00001000,
                                    0b00001000,
                                    0b00001000,
                                    0b00001000,
                                    0b00001000,
                                    0b00001000,
                                    0b00001000,
                                    0b00001000,
                                    0b00101010,
                                    0b00011100,
                                    0b00001000
                              };
    
    unsigned char T[5] = {
                            0b00011,
                            0b00001,
                            0b11111,
                            0b00001,
                            0b00011
                         };
    unsigned char R[5] = {
                            0b11111,
                            0b00101,
                            0b00101,
                            0b01101,
                            0b10010
                         };
    unsigned char A[5] = {
                            0b11100,
                            0b01010,
                            0b01001,
                            0b01010,
                            0b11100
                         };
    unsigned char N[5] = {
                            0b11111,
                            0b00010,
                            0b00100,
                            0b01000,
                            0b11111
                         };
    unsigned char S[5] = {
                            0b10010,
                            0b10101,
                            0b10101,
                            0b10101,
                            0b01001
                         };
    unsigned char L[5] = {
                            0b11111,
                            0b10000,
                            0b10000,
                            0b10000,
                            0b10000
                         };
    unsigned char O[5] = {
                            0b01110,
                            0b10001,
                            0b10001,
                            0b10001,
                            0b01110
                         };
    unsigned char V[5] = {
                            0b00111,
                            0b01100,
                            0b10000,
                            0b01100,
                            0b00111
                         };
    unsigned char E[5] = {
                            0b11111,
                            0b10101,
                            0b10101,
                            0b10101,
                            0b10101
                         };
    
#define LOVE_START 87
#define LOVE_END 97
#define FOR 99
#define STOP_ARROW 120
    int stop_arrow = STOP_ARROW;
    int arrow_i = -15;
    long int cycle = -1;
    while(cycle--){
        for(int i = 0; i < FOR; ++i){
            int delay = 0;
            switch(i){
                case 10: delay += 4;
                case 16: delay += 4;
                case 22: delay += 4;
                case 28: delay += 4;
                case 34: delay += 4;
                case 40: delay += 4;
                case 46: delay += 4;
                case 52: delay += 4;
                case 58: delay += 4;
                while(delay--);
            }
            if(i < 20 && i >= 15)
                LATA |= T[i - 15];
            if(i < 26 && i >= 21)
                LATA |= R[i - 21];
            if(i < 32 && i >= 27)
                LATA |= A[i - 27];
            if(i >= 33  && i < 38)
                LATA |= N[i - 33];
            if(i < 44 && i >= 39)
                LATA |= S[i - 39];
            if(i >= 45  && i < 50)
                LATA |= L[i - 45];
            if(i >= 51 && i < 56)
                LATA |= O[i - 51];
            if(i >= 57 && i < 62)
                LATA |= V[i - 57];
            if(i >= 63 && i < 68)
                LATA |= E[i - 63];
            
//            if(i >= 15  && i < 20)
//                LATA |= T[i - 15];
//            if(i < 26 && i >= 21)
//                LATA |= R[i - 21];
//            if(i >= 27 && i < 32)
//                LATA |= A[i - 27];
//            if(i < 38 && i >= 33)
//                LATA |= N[i - 33];
//            if(i >= 39 && i < 44)
//                LATA |= S[i - 39];
//            if(i < 50 && i >= 45)
//                LATA |= L[i - 45];
//            if(i >= 51 && i < 56)
//                LATA |= O[i - 51];
//            if(i < 62 && i >= 57)
//                LATA |= V[i - 57];
//            if(i >= 63 && i < 68)
//                LATA |= E[i - 63];
            if(i >= LOVE_START && i < LOVE_END)
                LATA |= love[i-LOVE_START];
            if(i >= arrow_i && i < arrow_i + ARROW_COLUMN)
                LATA |= arrow[i-arrow_i];
            if(i >= arrow_i + FOR && i < arrow_i + FOR + ARROW_COLUMN)
                LATA |= arrow[i-arrow_i-FOR];
            int bright = 40;
            while(bright--);
            LATA = 0;
            int dark = 40;
            while(dark--);
        }
        
        if(arrow_i == LOVE_START-3)
            if(--stop_arrow)
                --arrow_i;
        
        if(++arrow_i > FOR-15){
            stop_arrow = STOP_ARROW;
            arrow_i = -15;
        }
        
    }
#undef LOVE_START
#undef LOVE_END
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
//    int dark_i = 1;
//    while(dark_i--);
}

void mode_clock(){
    int state = 0;
    int XMAS[6][10] = // inverse face/M/A/S/'/X
    {
        {
            0b0000000000,
            0b1111111100,
            0b1110110010,
            0b1100100101,
            0b1100110011,
            0b1110001011,
            0b1100110011,
            0b1100100101,
            0b1110110010,
            0b1111111100
        },
        {
            0b1110001100,
            0b0110011110,
            0b1110110011,
            0b1100100011,
            0b1000100001,
            0b1000010001,
            0b1100010011,
            0b1100110011,
            0b0111100110,
            0b0011000111
        },
        {
            0b0000000011,
            0b0000111111,
            0b0011111001,
            0b0111010000,
            0b1100010000,
            0b1100010000,
            0b0111010000,
            0b0011111001,
            0b0000111111,
            0b0000000011
        },
        {
            0b1100000011,
            0b1111111111,
            0b0111000000,
            0b0001100000,
            0b0000110000,
            0b0000110000,
            0b0001100000,
            0b0111000000,
            0b1111111111,
            0b1100000011
        },
        {
            0b0000000000,
            0b0000000000,
            0b0000000000,
            0b0000000000,
            0b1111100000,
            0b1110010000,
            0b1110000000,
            0b0000000000,
            0b0000000000,
            0b0000000000
        },
        {
            0b1100000001,
            0b1000000011,
            0b1111001111,
            0b0011011100,
            0b0001110000,
            0b0000111000,
            0b0011101100,
            0b1111001111,
            0b1000000011,
            0b1100000001
        }
    };
    
    long int display_cycle = -1;
//    while(display_cycle--){
    while(1){
        // HH : MM : SS
        // HH
        display(hours/10);
        display(hours%10);
        
        // :
        int delay = 80;
        while(delay--);
        if(++state & 0b1000)
            LATA = 0b00001010;
        else
            LATA = 0;
        int minH = minutes / 10;
        LATA = 0;
        int minL = minutes % 10;
        delay = 80;
        while(delay--);
        
        // MM
        display(minH);
        display(minL);
        
        // :
        delay = 80;
        while(delay--);
        if(state & 0b1000)
            LATA = 0b00001010;
        else
            LATA = 0;
        int secH = seconds / 10;
        LATA = 0;
        int secL = seconds % 10;
        delay = 80;
        while(delay--);
     
        // SS
        display(secH);
        display(secL);
        
		delay = 1120;
		while(delay--);
		
		for(int i = 0; i < 6; ++i){
			unsigned int *show = (unsigned int*)XMAS[i];
            for(int j = 0; j < 10; ++j, ++show){
                LATA = *show;
                LATD = *show >> 8;
                int bright = 40;
                while(bright--);
                LATA = 0;
                LATD = 0;
                int dark = 40;
                while(dark--);
            }
            delay = 100;
            while(delay--);
		}
		delay = 1120;
		while(delay--);
    }
}

void update_time(){
    if(++seconds == 60){
        seconds = 0;
        if(++minutes==60){
            minutes = 0;
            if(++hours == 24)
                hours = 0;
        }
    }
}

void interrupt tc_int(void)     // High priority interrupt
{
    if(TMR1IE && TMR1IF){
        TMR1 = 15535;
        TMR1IF=0;
        count++;
        if(count == 5){ // (value/5) seconds
            count = 0;
            update_time();
        }
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
    
    // set initial time
    hours = 21;
    minutes = 0;
    
    TRISA = 0; // set as output
    TRISD = 0; // set as output
    LATD  = 0;
    LATA  = 0;
    mode_TransLove();
    mode_clock();
    mode_walker();
    
//    int num0[6] = NUM(0);
    while(1);
}
