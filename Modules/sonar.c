//fegyizoli @ 2014.11.04.

//SONAR DRIVER

//1. TRIG -et felhúzzuk magasba 10 usec-ig
//2. ECHO inputot figyeljük ISR-rel, ha jön a RISING_EDGE indítjuk a timert (max 80 msec-ig számoljon)
//3. ECHO-n ha jön a FALLING_EDGE leállítjuk a timert, vagy ha eltelt a 80 msec
//4. timer_usec / 58 =  xxx cm

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#include <modules.h>

static CTL_ISR_FN_t sonar_echo_isr;

//CTL_SEMAPHORE_t echo_sem;


CTL_EVENT_SET_t  sonar_events;

static int32u g_obstacle_cm;

void sonar_echo_isr_init(CTL_ISR_FN_t fn);
void delay_us(int32u t);


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

int32u g_sonar_tmr_cnt;
CTL_ISR_FN_t sonar_echo_isr_handler(void)
{
 int32u status, timer_val,timer_period;
 status = GPIOPinIntStatus( SONAR_PORT, 1 );
 GPIOPinIntClear( SONAR_PORT, status );
 if(status & SONAR_ECHO_PIN)
 {
  if(GPIOPinRead(SONAR_PORT, SONAR_ECHO_PIN) == SONAR_ECHO_PIN)
  {
  GPIOPinWrite(RGB_LED_PORT,RGB_LED_RED_PIN,RGB_LED_RED_PIN);
  TimerEnable(SONAR_TIMER_BASE, TIMER_A);
  }
  if(GPIOPinRead(SONAR_PORT, SONAR_ECHO_PIN) == 0)
  {
  GPIOPinWrite(RGB_LED_PORT,RGB_LED_RED_PIN,0);
  g_sonar_tmr_cnt = TimerValueGet(SONAR_TIMER_BASE,TIMER_A);
  ctl_events_set_clear(&sonar_events,SONAR_ECHO,0);
  }
 }
}

int32u sonar_ping(void)
{
 int32u obj_distance = 0;
 GPIOPinWrite(SONAR_PORT,SONAR_TRIG_PIN,0);
 delay_us(5);
 GPIOPinWrite(SONAR_PORT,SONAR_TRIG_PIN,SONAR_TRIG_PIN);
 delay_us(10);
 GPIOPinWrite(SONAR_PORT,SONAR_TRIG_PIN,0);
 ctl_events_wait(CTL_EVENT_WAIT_ANY_EVENTS_WITH_AUTO_CLEAR,&sonar_events,SONAR_ECHO,CTL_TIMEOUT_NONE,0);
 obj_distance = g_sonar_tmr_cnt / 58;
 return obj_distance;
}

fp32 sonar_ping_cm(void)
{
 int32u val;
 val = sonar_ping();
 return ( (fp32)val / 100.0 );
}

//fp32 sonar_ping_median_cm(int8u sample_cnt)
//{
// fp32 samples[sample_cnt], tmp;
// int8u i,j;
// for(i=0;i<sample_cnt;i++) //beolvasom a sample_cnt db. mintavételt
// {
//  samples[i] = sonar_ping_cm();
//  ctl_timeout_wait(ctl_get_current_time()+100); //
// }
// //rendezem a tömbömet
// for(i=1;i<sample_cnt;i++)
// {
//  tmp = samples[i];
//  j = i - 1;
//  while(tmp < samples[j] && j >= 0)
//  {
//   samples[j+1] = samples[j];
//   j--;
//  }
//  samples[j+1] = tmp;
// }
// //megkeresem a középső elemet -> medián
// i = sample_cnt / 2;
// return samples[i];
//}


void delay_us(int32u t)
{
 SysCtlDelay((SysCtlClockGet() / 1000000) * t );
}


void sonar_init(void)
{
 int32u timer_period;
 ctl_events_init(&sonar_events,0);
 SysCtlPeripheralEnable(SONAR_PERIPH);
 GPIOPinTypeGPIOOutput(SONAR_PORT,SONAR_TRIG_PIN);
 SysCtlPeripheralEnable(SONAR_TIMER);
 TimerConfigure(SONAR_TIMER_BASE,TIMER_CFG_A_ONE_SHOT_UP);  //felfele számolós, eventek közti időt számítja elvileg
 timer_period = ((SysCtlClockGet() / 1000000) * SONAR_TIMER_MAX_VALUE); 
 TimerLoadSet(SONAR_TIMER_BASE, TIMER_A, timer_period);
 sonar_echo_isr_init((CTL_ISR_FN_t)sonar_echo_isr_handler);
 
}