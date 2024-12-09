#include <msp430.h>
#include "libTimer.h"
#include "led.h"


#define LED_RED BIT0               // P1.0
#define LED_GREEN BIT6             // P1.6
#define LEDS (BIT0 | BIT6)


int main(void){  
  configureClocks();

  P1DIR |= LEDS;
  P1OUT &= ~LEDS;		/* leds initially off */
  

  or_sr(0x18);  // CPU off, GIE on
} 

static int curr_state = -1;
static int time_count = 0;
static int blink_limit = 5;
static int blink_count = 0;

#pragma vector=PORT2_VECTOR

void dark_to_bright(){
    time_count++;
    if(time_count >= 250){
        blink_limit++;
        time_count = 0;
        if(blink_limit >=8){
            blink_limit = 0;
        }
    }
    if (blink_count >= blink_limit) {
        blink_count = 0;
        P1OUT |= LED_GREEN;
    
    } else{
        P1OUT &= ~LED_GREEN;
    }
}

void bright_to_dark() {
    time_count++;

    if (time_count >= 250) {  // Gradually adjust brightness every 250 cycles
        if (blink_limit > 0) {
            blink_limit--;  // Decrease brightness level
        } else {
            blink_limit = 8;  // Reset to full brightness when completely dark
        }
        time_count = 0;
    }

    // Control LED based on brightness level
    if (blink_count >= blink_limit) {
        blink_count = 0;
        P1OUT |= LED_GREEN;  // Turn LED on
    } else {
        P1OUT &= ~LED_GREEN; // Turn LED off
    }

    blink_count++;
}

void sound_one(){}

void sound_two(){}



/* we need to transition on button press 2 press button 1 resets the state machine on button 3 & 4  
    we need to play 2 different sound so we get it to change
*/


__interrupt void Port_2_ISR(void) {
    // Check which pin triggered the interrupt
    if (P2IFG & BIT0) { // Button 1 (P2.0)
        curr_state = 0; // Go to the next state
    }
    if (P2IFG & BIT1) { // Button 2 (P2.1)
        if(curr_state == 3)
        { 
            curr_state = 1;
        }else{
            curr_state++;
        }
    }
    if (P2IFG & BIT2) { // Button 3 (P2.2)
        sound_one(); // Play sound 1
    }
    if (P2IFG & BIT3) { // Button 4 (P2.3)
        sound_two(); // Play sound 2
    }

    // Clear the interrupt flags for P2.0 - P2.3
    P2IFG &= ~0x0F; // Clear flags for the relevant pins
}

void __interrupt_vec(WDT_VECTOR) WDT(){



    switch(curr_state){ // case 1 restarts case 2 advances the state
        case 0:
            P1OUT &= ~LED_GREEN;//original off state
            curr_state++;
            break;

        case 1:
            dark_to_bright();
            break;

        case 2: 
            bright_to_dark();
            break;

        case 3:
            if(time_count >= 250){
                blink();
            }
            break;

    }
}
