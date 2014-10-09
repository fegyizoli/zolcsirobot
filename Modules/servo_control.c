//fegyizoli @ 2014.09.10.
#include <modules.h>
/**************************************************************************
*                        SERVO CONTROLLER MODULE                  *  
**************************************************************************/
static CTL_ISR_FN_t servo_tmr_isr;

servo_info servo;

int32u g_period, g_pulse_time;
int16u g_servo_max_val; //ezt kell változtatni a pozíció váltáshoz





/**************************************************************************
*                                      TIMER ISR                                     *
**************************************************************************/
void Timer0A_ISR(void)
{
ctl_enter_isr();
servo_tmr_isr();
ctl_exit_isr();
}


void servo_tmr_isr_init(CTL_ISR_FN_t fn)
{
int en;
en = ctl_global_interrupts_set(0);
servo_tmr_isr = fn;
SysCtlPeripheralEnable(SERVO_TIMER_PERIPH);
TimerConfigure(SERVO_TIMER_BASE, TIMER_CFG_PERIODIC);
g_period = ((SysCtlClockGet() / 1000000) * SERVO_TIMER_RESOLUTION); 
TimerLoadSet(SERVO_TIMER_BASE, SERVO_TIMER_SPLIT, g_period);
TimerIntDisable(SERVO_TIMER_BASE, SERVO_TIMER_TRIGGER);
if(TimerIntStatus(SERVO_TIMER_BASE,1) == SERVO_TIMER_TRIGGER)
{
 TimerIntClear(SERVO_TIMER_BASE,SERVO_TIMER_TRIGGER);
}
ctl_set_priority(SERVO_TIMER_ISR_PRIO,1);
ctl_unmask_isr(SERVO_TIMER_ISR_PRIO);
ctl_global_interrupts_set(en);
TimerIntEnable(SERVO_TIMER_BASE, SERVO_TIMER_TRIGGER);
TimerEnable(SERVO_TIMER_BASE, SERVO_TIMER_SPLIT);
}

CTL_ISR_FN_t servo_tmr_isr_handler(void)
{
 TimerIntClear(SERVO_TIMER_BASE, SERVO_TIMER_TRIGGER);
 g_pulse_time += SERVO_TIMER_RESOLUTION; 
 if(g_pulse_time > SERVO_PERIOD)
 {
  g_pulse_time = 0;
 }
 if(g_pulse_time > servo.current_pos)
 {
  GPIOPinWrite(SERVO_PORT,SERVO_PIN, 0); //vegyük el a feszt
 }
 else
 {
  GPIOPinWrite(SERVO_PORT, SERVO_PIN, SERVO_PIN); //adjuk rá a feszt
 }
}

int8u move_to_pos(int16u pos)
{
 if(pos == servo.target_pos)
 {
  servo.moving = false;
  return ALREADY_THERE;
 }
 else
 {
  if( (pos  <= servo.max_pos) && (pos >= servo.min_pos) )
  {
   servo.current_pos = pos;
   servo.moving = true;
   return MOVE_CMD_SUCCESS;
  }
  if( pos > servo.max_pos)
  {
   servo.current_pos = servo.max_pos;
   servo.moving = true;
   return OUT_OF_DOMAIN;
  }
  if(pos < servo.min_pos)
  {
   servo.current_pos = servo.min_pos;
   servo.moving = true;
   return OUT_OF_DOMAIN;
  }
 }
}

void servo_enable(void)
{
 TimerEnable(SERVO_TIMER_BASE, SERVO_TIMER_SPLIT);
}

void servo_disable(void)
{
 TimerDisable(SERVO_TIMER_BASE, SERVO_TIMER_SPLIT);
}

/******************************************
*         SERVO CONTROL INIT        *
******************************************/
void servo_init(void)
{
 SysCtlPeripheralEnable(SERVO_PERIPH);
 GPIOPinTypeGPIOOutput(SERVO_PORT,SERVO_PIN);
 GPIOPinWrite(SERVO_PORT,SERVO_PIN,SERVO_PIN); //világítsá'
 servo_tmr_isr_init((CTL_ISR_FN_t)servo_tmr_isr_handler); //itt inicializálja a timert is
 servo.min_pos = SERVO_MIN_VAL;
 servo.max_pos = SERVO_MAX_VAL;
 servo.current_pos = ((SERVO_MAX_VAL - SERVO_MIN_VAL) / 2) + SERVO_MIN_VAL; //initkor legyen középen
 servo.target_pos =  0;
 servo.moving = false;
}