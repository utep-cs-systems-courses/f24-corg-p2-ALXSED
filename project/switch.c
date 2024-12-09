#include <msp430.h>
#include "switch.h"
#include "buzzer.h"


void switch_init(){
    P2REN |= SWITCHES;
    P2IE |= SWITCHES;
    P2OUT |= SWITCHES;
    P2DIR &= ~SWITCHES;
    switch_update_interrupt_sense();
    switch_interupt_handler();
}

char switch_update_interrupt_sense(){
    char p2val = P2IN;
    P2IES |= (p2val & SWITCHES);
    P2IES |= (p2val | ~SWITCHES);
    return p2val;
}

void switch_interupt_handler(){
    char p2val = switch_update_interrupt_sense();

    char sw_1 = (p2val & SW1) ? 0 : SW1; //sw_1 will reset the state machine back to nothing aka state 0
    char sw_2 = (p2val & SW2) ? 0 : SW2; //sw_2 will advance the state machine to the next state
    char sw_3 = (p2val & SW3) ? 0 : SW3; //sw_3 will play a sound aka wee
    char sw_4 = (p2val & SW4) ? 0 : SW4; //sw_4 will play a different sound aka woo

    if(sw_1){
        state = 0;

    }else if (sw_2){
        state = (state == 2) ? 0 : state + 1;
    }else if (sw_3){
        buzzer_set_period(400);
    }else if (sw_4){
        buzzer_set_period(800);
    }
}


