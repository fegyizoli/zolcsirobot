//fegyizoli @ 2014.11.04.

//SONAR DRIVER

#define SONAR_TIMER					SYSCTL_PERIPH_TIMER4
#define SONAR_TIMER_BASE      				TIMER4_BASE
#define SONAR_TIMER_CFG       				GPIO_PC0_T4CCP0
#define SONAR_TIMER_IRQ_PRIORITY                        70

//TIMER VALUES
#define SONAR_INIT_VALUE                                5 //usec
#define SONAR_TRIG_VALUE                                10 //usec
#define SONAR_TIMER_MAX_VALUE 				60000 // 60 msec



#define SONAR_PERIPH					SYSCTL_PERIPH_GPIOA
#define SONAR_PORT           				GPIO_PORTA_BASE

#define SONAR_TRIG_PIN    				GPIO_PIN_6
#define SONAR_ECHO_PIN  				GPIO_PIN_7
#define SONAR_ECHO_IRQ_PRIORITY  			0


#define SONAR_ECHO          (1<<0)


#define SONAR_BUSY_CANNOT_PINGED 0x01

fp32 sonar_ping_median_cm(int8u sample_cnt);
fp32 sonar_ping_cm(void);
int32u sonar_ping(void);

void sonar_init(void);