//fegyizoli @ 2013.11.29

#include <modules.h>

static CTL_ISR_FN_t buttons_isr;
CTL_EVENT_SET_t button_events;


void GPIO_Port_F_ISR(void)
{
ctl_enter_isr();
buttons_isr();
ctl_exit_isr();
}

//ISR INIT
void ctl_buttons_isr_init(CTL_ISR_FN_t fn)
{
int en; int32u proba;
en=ctl_global_interrupts_set(0);
buttons_isr=fn;
SysCtlPeripheralEnable(PUSHBUTTON_PERIPH);
//UNLOCKOLNI KELL A PF0 REGISZTERT MERT NMI-RE VAN ALLITVA
HWREG(PUSHBUTTON_PORT + GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
HWREG(PUSHBUTTON_PORT + GPIO_O_CR) |= 0x01;
HWREG(PUSHBUTTON_PORT + GPIO_O_LOCK) = 0;

GPIODirModeSet(PUSHBUTTON_PORT, LEFT_SWITCH | RIGHT_SWITCH , GPIO_DIR_MODE_IN);
GPIOPadConfigSet(PUSHBUTTON_PORT,LEFT_SWITCH | RIGHT_SWITCH , GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
GPIOPinIntDisable(PUSHBUTTON_PORT, LEFT_SWITCH | RIGHT_SWITCH);
if((GPIOPinIntStatus(PUSHBUTTON_PORT, 1)) == LEFT_SWITCH )
{
 GPIOPinIntClear(PUSHBUTTON_PORT, LEFT_SWITCH );
}
if((GPIOPinIntStatus(PUSHBUTTON_PORT, 1)) == RIGHT_SWITCH )
{
 GPIOPinIntClear(PUSHBUTTON_PORT, RIGHT_SWITCH );
}
ctl_set_priority(PUSHBUTTON_IRQ_PRIORITY, 1);
ctl_unmask_isr(PUSHBUTTON_IRQ_PRIORITY);
ctl_global_interrupts_set(en);
GPIOIntTypeSet(PUSHBUTTON_PORT, LEFT_SWITCH | RIGHT_SWITCH , GPIO_BOTH_EDGES); //GPIO_BOTH_EDGES
GPIOPinIntEnable(PUSHBUTTON_PORT, LEFT_SWITCH | RIGHT_SWITCH );
}

//ISR HANDLER
CTL_ISR_FN_t buttons_isr_handler(void)
{
int32u int_status;
int_status = GPIOPinIntStatus( PUSHBUTTON_PORT, 1 );
GPIOPinIntClear( PUSHBUTTON_PORT, int_status );
if(int_status & LEFT_SWITCH)
{
 if(GPIOPinRead(PUSHBUTTON_PORT, LEFT_SWITCH) == LEFT_SWITCH)
  ctl_events_set_clear(&button_events,0,L_BTN_PUSHED);
 else
  ctl_events_set_clear(&button_events,L_BTN_PUSHED,0);
}
if(int_status & RIGHT_SWITCH)
{
 if(GPIOPinRead(PUSHBUTTON_PORT, RIGHT_SWITCH) == RIGHT_SWITCH)
  ctl_events_set_clear(&button_events,0, R_BTN_PUSHED);
 else
  ctl_events_set_clear(&button_events,R_BTN_PUSHED,0);
}

}

//INIT
void  buttons_init(void)
{
ctl_events_init(&button_events,0);
ctl_buttons_isr_init((CTL_ISR_FN_t)buttons_isr_handler);
}