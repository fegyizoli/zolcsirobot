#include <modules.h>
#include <string.h>
#include <ctl_api.h>

CTL_TASK_t main_task, start_task;

#define START_STACKSIZE (3000/sizeof(unsigned)) 
#define START_PRIORITY 13         

unsigned start_task_stack[1+START_STACKSIZE+1];

#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif
extern CTL_EVENT_SET_t button_events, motor_cmds;
extern int32u g_period, g_pulse_time;
extern servo_info servo;
//START TASK
int8u i, gc, pc;
void start_task_code(void *p)
{  
  int32u ev;
  rgb_init();
  buttons_init();
//  tracks_control_init();
//  pwm_init(50);
//  pwm_clk_enable();
  servo_init();
  com_init(COM2,19200);
  while (1)
  {        
   ev = ctl_events_wait(CTL_EVENT_WAIT_ANY_EVENTS,&button_events,(L_BTN_PUSHED | R_BTN_PUSHED),CTL_TIMEOUT_DELAY,3000);
   if(ev & L_BTN_PUSHED)
   {
    pc = 0xcd;
    i = com_put_char(COM2,&pc);
//    move_to_pos(LEFT_POS);
//    GPIOPinWrite(RGB_LED_PORT,RGB_LED_GREEN_PIN,0);
//    GPIOPinWrite(RGB_LED_PORT,RGB_LED_RED_PIN,0);
//    GPIOPinWrite(RGB_LED_PORT,RGB_LED_BLUE_PIN,RGB_LED_BLUE_PIN);
   }
   if(ev & R_BTN_PUSHED)
   {
    pc = 0xab;
    i = com_put_char(COM2,&pc);
//    move_to_pos(RIGHT_POS);
//    GPIOPinWrite(RGB_LED_PORT,RGB_LED_GREEN_PIN,0);
//    GPIOPinWrite(RGB_LED_PORT,RGB_LED_RED_PIN,RGB_LED_RED_PIN);
//    GPIOPinWrite(RGB_LED_PORT,RGB_LED_BLUE_PIN,0);
   }
   if(ev == 0)
   {
    com_get_char(COM2,&gc);
//    move_to_pos(MIDDLE_POS);
//    GPIOPinWrite(RGB_LED_PORT,RGB_LED_GREEN_PIN,RGB_LED_GREEN_PIN);
//    GPIOPinWrite(RGB_LED_PORT,RGB_LED_RED_PIN,0);
//    GPIOPinWrite(RGB_LED_PORT,RGB_LED_BLUE_PIN,0);
   }
  }  
}

void ctl_handle_error(CTL_ERROR_CODE_t e)
{
 while (1);
}

int main(void)
{
  unsigned int v=0;
  ctl_task_init(&main_task, 255, "main"); // create subsequent tasks whilst running at the highest priority.
  ctl_start_timer(ctl_increment_tick_from_isr); // start the timer 
  memset(start_task_stack, 0xcd, sizeof(start_task_stack));  // write known values into the stack
  start_task_stack[0]=start_task_stack[1+START_STACKSIZE]=0xfacefeed; // put marker values at the words before/after the stack
  ctl_task_run(&start_task, START_PRIORITY, start_task_code, 0, "start_task", START_STACKSIZE, start_task_stack+1, 0);
  ctl_task_set_priority(&main_task, 0); // drop to lowest priority to start created tasks running.
  while (1)
    {    
      // power down can go here if supported      
      v++;
    }
  return 0;
}
