//fegyizoli @ 2013.10.19.





#define RGB_LED_PERIPH           SYSCTL_PERIPH_GPIOF
#define RGB_LED_PORT             GPIO_PORTF_BASE
#define RGB_LED_RED_PIN          GPIO_PIN_1
#define RGB_LED_BLUE_PIN        GPIO_PIN_2
#define RGB_LED_GREEN_PIN         GPIO_PIN_3

//FUNCTION PROTOTYPES
void rgb_init(void);