//fegyizoli @ 2014.11.04.

//SONAR DRIVER

//1. TRIG -et felhúzzuk magasba 10 usec-ig
//2. ECHO inputot figyeljük ISR-rel, ha jön a RISING_EDGE indítjuk a timert (max 80 msec-ig számoljon)
//3. ECHO-n ha jön a FALLING_EDGE leállítjuk a timert, vagy ha eltelt a 80 msec
//4. timer_usec / 58 =  xxx cm

#include <modules.h>

static CTL_ISR_FN_t sonar_echo_isr;

CTL_SEMAPHORE_t echo_sem;


//CTL_EVENT_SET_t  sonar_events;

static int32u g_obstacle_cm;

void sonar_echo_isr_init(CTL_ISR_FN_t fn);

//ECHO PIN interrupt
void GPIO_Port_A_ISR(void)
{
 ctl_enter_isr();
 sonar_echo_isr();
 ctl_exit_isr();
}

void sonar_echo_isr_init(CTL_ISR_FN_t fn)
{
 int en;
 en = ctl_global_interrupts_set(0);
 sonar_echo_isr = fn;
 SysCtlPeripheralEnable(SONAR_PERIPH);
 GPIODirModeSet(SONAR_PORT, SONAR_ECHO_PIN, GPIO_DIR_MODE_IN);
 GPIOPadConfigSet(SONAR_PORT,SONAR_ECHO_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
 GPIOPinIntDisable(SONAR_PORT, SONAR_ECHO_PIN);
 if((GPIOPinIntStatus(SONAR_PORT, 1)) == SONAR_ECHO_PIN )
 {
  GPIOPinIntClear(SONAR_PORT, SONAR_ECHO_PIN );
 }
 ctl_set_priority(SONAR_ECHO_IRQ_PRIORITY,1);
 ctl_unmask_isr(SONAR_ECHO_IRQ_PRIORITY);
 ctl_global_interrupts_set(en);
 GPIOIntTypeSet(SONAR_PORT, SONAR_ECHO_PIN, GPIO_BOTH_EDGES);
 GPIOPinIntEnable(SONAR_PORT, SONAR_ECHO_PIN);
}

CTL_ISR_FN_t sonar_echo_isr_handler(void)
{
 int32u status, timer_val,timer_period;
 status = GPIOPinIntStatus( SONAR_PORT, 1 );
 GPIOPinIntClear( SONAR_PORT, status );
 if(status & SONAR_ECHO_PIN)
 {
  if(GPIOPinRead(SONAR_PORT, SONAR_ECHO_PIN) == SONAR_ECHO_PIN)
  {
//  timer_period = ((SysCtlClockGet() / 1000000) * SONAR_TIMER_MAX_VALUE); 
//  TimerLoadSet(SONAR_TIMER_BASE, TIMER_A, timer_period);
  TimerEnable(SONAR_TIMER_BASE, TIMER_A);
  }
  else
  {
  TimerDisable(SONAR_TIMER_BASE, TIMER_A);
  timer_val = TimerValueGet(SONAR_TIMER_BASE,TIMER_A);
  g_obstacle_cm = timer_val / 58;
  ctl_semaphore_signal(&echo_sem);
  }
 }
}

int32u sonar_ping(void)
{
 GPIOPinWrite(SONAR_PORT,SONAR_TRIG_PIN,0);
 SysCtlDelay(5);
 GPIOPinWrite(SONAR_PORT,SONAR_TRIG_PIN,SONAR_TRIG_PIN);
 SysCtlDelay(10);
 GPIOPinWrite(SONAR_PORT,SONAR_TRIG_PIN,0);
 ctl_semaphore_wait(&echo_sem,CTL_TIMEOUT_NONE,0);
 return g_obstacle_cm;
}


void sonar_init(void)
{
 int32u timer_period;
 ctl_semaphore_init(&echo_sem,0);
 SysCtlPeripheralEnable(SONAR_PERIPH);
 GPIOPinTypeGPIOOutput(SONAR_PORT,SONAR_TRIG_PIN);

 SysCtlPeripheralEnable(SONAR_TIMER);
 TimerConfigure(SONAR_TIMER_BASE,TIMER_CFG_PERIODIC_UP);
 timer_period = ((SysCtlClockGet() / 1000000) * SONAR_TIMER_MAX_VALUE); 
 TimerLoadSet(SONAR_TIMER_BASE, TIMER_A, timer_period);
 sonar_echo_isr_init((CTL_ISR_FN_t)sonar_echo_isr_handler);
 
}