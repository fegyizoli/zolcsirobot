//fegyizoli @ 2013.10.19.
#include <modules.h>

//TODO: paramterezett RGB fuggveny




///////////////////////////////////////////////////////////////
void rgb_init(void)
{
 SysCtlPeripheralEnable(RGB_LED_PERIPH);
 GPIOPinTypeGPIOOutput(RGB_LED_PORT, (RGB_LED_RED_PIN | RGB_LED_GREEN_PIN | RGB_LED_BLUE_PIN));
}