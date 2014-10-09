//fegyizoli @ 2014.09.27.

//ezekkel a defineokkal tudom megadni, hogy melyik com portos kódrészleteket forgassa be
/*********************************************/
#define COM0_EN 0
#define COM1_EN 0 //ez nincs implementálva, nem vesszük figyelembe
#define COM2_EN 1
/********************************************/
//virtual com port: COM0
#ifdef COM0_EN

#define COM0_ISR_PRIORITY                                       5 //lm4fh5qr.h alapján

#define COM0_PERIPH                                                 SYSCTL_PERIPH_GPIOA //fizikai pin beállítások
#define COM0_PORT                                                    GPIO_PORTA_BASE
#define COM0_RX_PIN                                                  GPIO_PIN_0
#define COM0_TX_PIN                                                  GPIO_PIN_1

#define COM0_MODULE                                                    SYSCTL_PERIPH2_UART0  //modul

#define CONFIGURE_TO_COM0_RX                              GPIO_PA0_U0RX
#define CONFIGURE_TO_COM0_TX                              GPIO_PA1_U0TX   

#define COM0_BASE                                               UART0_BASE  //vezérlõ 

#endif

//a COM2 modult használjuk mert azok a pinek szabadok
#ifdef COM2_EN

#define COM2_ISR_PRIORITY                                       33 //lm4fh5qr.h alapján

#define COM2_PERIPH                                                 SYSCTL_PERIPH_GPIOD //fizikai pin beállítások
#define COM2_PORT                                                    GPIO_PORTD_BASE
#define COM2_RX_PIN                                                  GPIO_PIN_6
#define COM2_TX_PIN                                                  GPIO_PIN_7

#define COM2_MODULE                                                    SYSCTL_PERIPH2_UART2  //modul

#define CONFIGURE_TO_COM2_RX                              GPIO_PD6_U2RX
#define CONFIGURE_TO_COM2_TX                              GPIO_PD7_U2TX   

#define COM2_BASE                                               UART2_BASE  //vezérlõ

#endif

//-------------------------------------------------------------------------

#ifdef COM0_EN
#define COM0_RX_BUFFSIZE                                       16
#define COM0_TX_BUFFSIZE                                       16
#endif

#ifdef COM2_EN
#define COM2_RX_BUFFSIZE                                       16
#define COM2_TX_BUFFSIZE                                       16
#endif

#define RX (1<<0)
#define TX (1<<1)

#define NO_ERROR 0
#define NO_SPACE_AVAILABLE 1
#define EMPTY_FIFO 2

#define COM0    0
#define COM1    1
#define COM2    2


void com_init(int8u com, int32u baud);

void com_flush(int8u com, int8u what);
int8u com_put_char(int8u com, int8u* tx_c);
int8u com_get_char(int8u com, int8u* rx_c);