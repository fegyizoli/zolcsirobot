//fegyizoli @ 2014.09.10.
#define SERVO_TIMER_RESOLUTION      10 //microseconds
#define SERVO_PERIOD                20000//20000 //microseconds
#define SERVO_MIN_VAL               1270//14000
#define SERVO_MAX_VAL               5000
//#define SERVO_TIMER_PHYS_PERIPH                SYSCTL_PERIPH_GPIOB //SYSCTL_PERIPH_GPIOF
#define SERVO_TIMER_PORT                  GPIO_PORTB_BASE  //GPIO_PORTF_BASE
#define SERVO_TIMER_PIN                   GPIO_PIN_4 //GPIO_PIN_2
#define SERVO_TIMER_CFG             GPIO_PB4_T1CCP0 //GPIO_PF2_T1CCP0
#define SERVO_TIMER_PERIPH          SYSCTL_PERIPH_TIMER0 //SYSCTL_PERIPH_TIMER1
#define SERVO_TIMER_BASE                TIMER0_BASE//TIMER1_BASE
#define SERVO_TIMER_SPLIT              TIMER_A
#define SERVO_TIMER_TRIGGER        TIMER_TIMA_TIMEOUT

#define SERVO_TIMER_ISR_PRIO      19 //lm4f232h5qr.h-ból  


#define SERVO_PERIPH    SYSCTL_PERIPH_GPIOA
#define SERVO_PORT       GPIO_PORTA_BASE
#define SERVO_PIN            GPIO_PIN_2


#define MOVE_CMD_SUCCESS 0
#define OUT_OF_DOMAIN 1
#define ALREADY_THERE 2

#define LEFT_POS        SERVO_MAX_VAL
#define RIGHT_POS       SERVO_MIN_VAL

#define MIDDLE_POS     (((SERVO_MAX_VAL-SERVO_MIN_VAL)/2)+SERVO_MIN_VAL)


typedef struct
{
 int16u min_pos;
 int16u max_pos;
 int16u current_pos;
 int16u target_pos;
 boolean moving;
 } servo_info;


int8u move_to_pos(int16u pos);
void servo_enable(void);
void servo_disable(void);
void servo_init(void);