//fegyizoli @ 2014.09.27.

#define UART0_ISR_PRIORITY                                       5 //lm4fh5qr.h alapján


#define UART0   0x00

#define UART_GET_OK  0
#define UART_PUT_OK  0

#define UART_TIMEOUT 1
#define UART_BUFF_EMPTY 2
#define UART_BUFF_OVERFLOW 3

#define UART_RX_BUFFSIZE                                      RING_BUFFER_SIZE
#define UART_TX_BUFFSIZE                                       RING_BUFFER_SIZE

struct uart_struct_
{
 ring_buffer rx_buffer;
 ring_buffer tx_buffer;
 CTL_SEMAPHORE_t rx_sem;
 CTL_SEMAPHORE_t tx_sem;
 int32u framing_error_interrupts;
 int32u overrun_error_interrupts;
 int32u rx_interrupts;
 int16u received;
 int32u tx_interrupts;
 int32u buffer_overflow;
} __attribute__ ((packed, aligned (1)));

typedef struct uart_struct_ uart_struct;


//PUBLIC FUNCTIONS
int8u uart0_get_char(int8u* c, int32u timeout_ms);
int8u uart0_put_char(int8u c);
void uart0_init(int32u baud);