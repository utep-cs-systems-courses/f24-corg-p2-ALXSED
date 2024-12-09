#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include "switch.h"
#include "buzzer.h"

int state = 0;

//My name is Alejandro Sedeno-Gonzalez i thought i would put that here since my name on github is not my name <3
int main(void){
  
  configureClocks();
  enableWDTInterrupts();
  led_init();
  switch_init();
  buzzer_init();

  or_sr(0x18);

}

void __interrupt_vec(PORT2_VECTOR) Port2(){
    if(P2IFG & SWITCHES){
        P2IFG &= ~SWITCHES;
        switch_interupt_handler();
    }
}


void __interrupt_vec(WDT_VECTOR) WDT(){

    switch(state){
        case 0:
            P1OUT &= ~LED_RED;
            break;
        case 1:
            bright_to_dark();
            break;
        case 2:
            dark_to_bright();
            break;
    }
}


