//fegyizoli @ 2013.12.05
#include <modules.h>
CTL_TASK_t track_control_task;

CTL_EVENT_SET_t track_cmds;

unsigned track_control_stack[1+TRACK_CONTROL_STACKSIZE+1];

int32u event1 = 0;
void tracks_task_code(void *p)
{
 int32u event, test=0;
 while(1)
 {
  switch(event1)
  {
  case 1:
   GPIOPinWrite(MOTOR_INPUT_PORT,R_MOTOR_B, R_MOTOR_B);
   GPIOPinWrite(MOTOR_INPUT_PORT,R_MOTOR_A, 0);
  break;
  case 2:
   GPIOPinWrite(MOTOR_INPUT_PORT,R_MOTOR_B, 0);
   GPIOPinWrite(MOTOR_INPUT_PORT,R_MOTOR_A, 0);
  break;
  case 3:
   GPIOPinWrite(MOTOR_INPUT_PORT,R_MOTOR_A, R_MOTOR_A);
   GPIOPinWrite(MOTOR_INPUT_PORT,R_MOTOR_B, 0);
  break;
  case 4:
   GPIOPinWrite(MOTOR_INPUT_PORT,L_MOTOR_B, L_MOTOR_B);
   GPIOPinWrite(MOTOR_INPUT_PORT,L_MOTOR_A, 0);
  break;
  case 5:
   GPIOPinWrite(MOTOR_INPUT_PORT,L_MOTOR_B, 0);
   GPIOPinWrite(MOTOR_INPUT_PORT,L_MOTOR_A, 0);
  break;
  case 6:
   GPIOPinWrite(MOTOR_INPUT_PORT,L_MOTOR_A, L_MOTOR_A);
   GPIOPinWrite(MOTOR_INPUT_PORT,L_MOTOR_B, 0);
  break;
  }
  ctl_timeout_wait(ctl_get_current_time()+10);
 }
}

void tracks_control_init(void)
{
 SysCtlPeripheralEnable(MOTOR_PERIPH);
 GPIOPinTypeGPIOOutput(MOTOR_INPUT_PORT, L_MOTOR_A | L_MOTOR_B | R_MOTOR_A | R_MOTOR_B);
 ctl_events_init(&track_cmds, 0); //kezdetben allnak a mocik (L_STOP | R_STOP)
 ctl_task_run(&track_control_task,TRACK_CONTROL_PRIORITY,tracks_task_code, 0, "track_control", TRACK_CONTROL_STACKSIZE, track_control_stack+1, 0);
}