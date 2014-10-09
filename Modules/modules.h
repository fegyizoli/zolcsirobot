//fegyizoli @ 2013.10.19.


/*******************************************************************************************
 *To use this file you need to make a new macro in Rowley Crossworks.
 *Go to Project/Macros and create a new one, with the path of this file named Modules
 *Then you have to assing it to the Solution Properties / User Include Directiories like this:
 *$(Modules)/ 
 *******************************************************************************************/

//INCLUDE THE NECESSARY STELLARISWARE MODULES

#include <string.h>
#include <ctl_api.h>
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <inc/hw_timer.h>
#include <inc/hw_hibernate.h>
#include <inc/hw_gpio.h>
#include <inc/hw_sysctl.h>
#include <inc/hw_pwm.h>
#include <inc/hw_uart.h>

#include <driverlib/fpu.h>
#include <driverlib/sysctl.h>
#include <driverlib/rom.h>
#include <driverlib/pin_map.h>
#include <driverlib/gpio.h>
#include <driverlib/systick.h>
#include <driverlib/interrupt.h>
#include <driverlib/hibernate.h>
#include <driverlib/timer.h>
#include <driverlib/pwm.h>
#include <driverlib/uart.h>


//TYPEDEFINITIONS

typedef unsigned char boolean;

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

typedef unsigned char                                   int8u;
typedef signed char                                     int8s;
typedef unsigned int                                    int16u;
typedef signed int                                      int16s;
typedef unsigned long int                               int32u;
typedef signed long int                                 int32s;
typedef unsigned long long int                          int64u;
typedef signed long long int                            int64s;
typedef float                                           fp32;
typedef double                                          fp64;
typedef long double                                     fp80;

#include "rgb_led.h"
#include "robot_buttons.h"
#include "track_control.h"
#include "robot_pwm.h"
#include "servo_control.h"
#include "uart_driver.h"
