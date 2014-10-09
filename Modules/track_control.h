//fegyizoli @ 2013.12.05

#define TRACK_CONTROL_PRIORITY 5
#define TRACK_CONTROL_STACKSIZE (1000/sizeof(unsigned))

#define MOTOR_PERIPH                   SYSCTL_PERIPH_GPIOB
#define MOTOR_INPUT_PORT               GPIO_PORTB_BASE

#define L_MOTOR_A                    GPIO_PIN_0
#define L_MOTOR_B                    GPIO_PIN_1

#define R_MOTOR_A                    GPIO_PIN_2
#define R_MOTOR_B                    GPIO_PIN_3

//command codes
#define STOP_LEFT                    0x01
#define STOP_RIGHT                   0x02
#define TURN_LEFT                    0x03
#define TURN_RIGHT                   0x04
#define MOVE_FORWARD                 0x05
#define MOVE_BACKWARD                0x06
#define RIGHT_ABOUT_TURN             0x07 //ez kérdõjeles... kellene hozzá valami segéd érzékelõ

//control events
#define L_STOP                       (1<<0)
#define R_STOP                       (1<<1)
#define L_TURN_FW                    (1<<2)
#define L_TURN_BW                    (1<<3)
#define R_TURN_FW                    (1<<4)
#define R_TURN_BW                    (1<<5)


//typedef struct 
//{
//
//}mot_struct;

//FUNCTION PROROTYPES
void tracks_control_init(void);