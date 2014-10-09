//fegyizoli @ 2014.01.15.
//PWM module with ISR


#define PWM_PERIPH      SYSCTL_PERIPH_GPIOB
#define PWM_PORT        GPIO_PORTB_BASE

#define PWM_PIN_R       GPIO_PIN_6
#define PWM_PIN_L       GPIO_PIN_7

#define PWM_TIMER        SYSCTL_PERIPH_TIMER0

#define PWM_TIMER_BASE   TIMER0_BASE

#define LEFT  0
#define RIGHT 1
#define BOTH  2

#define PWM_GRID 5 //the increment/decrement value

void pwm_clk_enable(void);
void pwm_clk_disable(void);
void pwm_period_set(int32u hz);
void pwm_duty_cycle_set(int32u period_percent, int8u which);
void pwm_init(int32u freq);