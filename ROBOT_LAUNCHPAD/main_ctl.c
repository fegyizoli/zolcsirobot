#include <modules.h>
#include <string.h>
#include <ctl_api.h>

CTL_TASK_t main_task, start_task;

#define START_STACKSIZE (100/sizeof(unsigned)) 
#define START_PRIORITY 1         

unsigned start_task_stack[1+START_STACKSIZE+1];

#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

//START TASK
void start_task_code(void *p)
{  
  rgb_init();
  buttons_init();
  while (1)
  {      
   GPIOPinWrite(RGB_LED_PORT,RGB_LED_RED_PIN,RGB_LED_RED_PIN);
   ctl_timeout_wait(ctl_get_current_time()+3000);
   GPIOPinWrite(RGB_LED_PORT,RGB_LED_RED_PIN,0);
   ctl_timeout_wait(ctl_get_current_time()+3000);
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
