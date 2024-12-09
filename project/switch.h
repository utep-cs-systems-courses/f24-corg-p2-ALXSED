#ifndef switch_included
#define switch_included

#define SW1 BIT0
#define SW2 BIT1
#define SW3 BIT2
#define SW4 BIT3
#define SWITCHES (SW1 | SW2 | SW3 | SW4)
extern int state;

void switch_init();
void switch_interupt_handler();
char switch_update_interrupt_sense();

#endif
