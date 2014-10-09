//fegyizoli @ 2013.11.29.

#define PUSHBUTTON_IRQ_PRIORITY   30 //lm4f232h5qd.h
#define PUSHBUTTON_PERIPH                     SYSCTL_PERIPH_GPIOF
#define PUSHBUTTON_PORT                       GPIO_PORTF_BASE
#define LEFT_SWITCH                           GPIO_PIN_4
#define RIGHT_SWITCH                          GPIO_PIN_0 

#define L_BTN_PUSHED (1<<0)
#define R_BTN_PUSHED (1<<1)

//FUNCTION PROTOTYPES
void buttons_init(void);
