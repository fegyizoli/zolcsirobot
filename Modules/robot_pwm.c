//fegyizoli @ 2014.01.15.
//PWM module with ISR
#include <modules.h>

int32u g_period_ticks;


void pwm_clk_enable(void)
{
 TimerEnable(PWM_TIMER_BASE, TIMER_A | TIMER_B);
}

void pwm_clk_disable(void)
{
 TimerDisable(PWM_TIMER_BASE,TIMER_A | TIMER_B);
}

void pwm_period_set(int32u hz)
{
 int32u proc_clock;
 int16u period_val; 
 TimerConfigure(PWM_TIMER_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM);
 TimerLoadSet(PWM_TIMER_BASE, TIMER_A | TIMER_B, hz);
}


void pwm_duty_cycle_set(int32u period_percent, int8u which)
{
 switch(which)
 {
  case LEFT:
  TimerMatchSet(PWM_TIMER_BASE, TIMER_A, period_percent); // PWM
  break;
  case RIGHT:
  TimerMatchSet(PWM_TIMER_BASE, TIMER_B, period_percent); // PWM
  break;
  case BOTH:
  TimerMatchSet(PWM_TIMER_BASE, TIMER_A | TIMER_B, period_percent); // PWM
  break;
 }
 SysCtlDelay(50000);
}

void pwm_init(int32u freq)
{
 SysCtlPeripheralEnable(PWM_PERIPH);
//mindkét 16 bites timer-t használom, mindkét oldalra egyet
 GPIOPinWrite(PWM_PORT, PWM_PIN_R | PWM_PIN_L, 0);
 GPIOPinConfigure(GPIO_PB6_T0CCP0);
 GPIOPinConfigure(GPIO_PB7_T0CCP1);
 GPIOPinTypeTimer(PWM_PORT, PWM_PIN_R);
 GPIOPinTypeTimer(PWM_PORT, PWM_PIN_L);
 
 SysCtlPeripheralEnable(PWM_TIMER);
 //invert output
 HWREG(PWM_TIMER_BASE + TIMER_O_CTL)   |= 0x40;
 HWREG(PWM_TIMER_BASE + TIMER_O_CTL)   |= 0x4000;
 pwm_period_set(freq);
 
 }