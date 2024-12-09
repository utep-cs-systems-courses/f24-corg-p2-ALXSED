#include <msp430.h>
#include "led.h"

static int time_count = 0;
static int time_count_2 = 0;
static int blink_limit = 0;
static int blink_count = 0;

void led_init(){
    P1DIR |= LEDS;
    P1OUT &= ~LEDS;
}

void bright_to_dark(){
    time_count++;
    blink_count++;

    if(time_count >= 250){
        blink_limit++;
        time_count = 0;
        if(blink_limit >=8){
            blink_limit = 0;
        }
    }
    if (blink_count >= blink_limit) {
        blink_count = 0;
        P1OUT |= LED_RED;
    
    } else{
        P1OUT &= ~LED_RED;
    }
}

void dark_to_bright(){
    time_count++;  
    blink_count++;  


    if(time_count >= 250){
        blink_limit++;  
        time_count = 0;  

    
        if(blink_limit >= 8){
            blink_limit = 0;
        }
    }

    if (blink_count < blink_limit) {
        P1OUT |= LED_RED;  
    } else {
        P1OUT &= ~LED_RED;  
    }

    if (blink_count >= 8) {
        blink_count = 0;
    }
}