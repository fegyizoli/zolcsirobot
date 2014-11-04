//fegyizoli @ 2014.09.27.
//alap UART a raspivel kommunikálásra
#include <modules.h>


static CTL_ISR_FN_t uart0_isr;

uart_struct uart0_stat;


void UART0_ISR(void)
{
ctl_enter_isr();
uart0_isr();
ctl_exit_isr();
}

void uart0_isr_init(CTL_ISR_FN_t fn)
{
int en;
en = ctl_global_interrupts_set(0);
uart0_isr = fn;

UARTIntDisable(UART0_BASE, (UART_INT_OE | UART_INT_FE | UART_INT_RT | UART_INT_TX | UART_INT_RX) ); //biztosra megyek, minden false értéket kitörlök
if(UARTIntStatus(UART0_BASE,1) == UART_INT_OE)
{
 UARTIntClear(UART0_BASE,UART_INT_OE);
}
if(UARTIntStatus(UART0_BASE,1) == UART_INT_FE)
{
 UARTIntClear(UART0_BASE,UART_INT_FE);
}
if(UARTIntStatus(UART0_BASE,1) == UART_INT_RT)
{
 UARTIntClear(UART0_BASE,UART_INT_RT);
}
if(UARTIntStatus(UART0_BASE,1) == UART_INT_TX)
{
 UARTIntClear(UART0_BASE,UART_INT_TX);
}
if(UARTIntStatus(UART0_BASE,1) == UART_INT_RX)
{
 UARTIntClear(UART0_BASE,UART_INT_RX);
}

ctl_set_priority(UART0_ISR_PRIORITY,1);
ctl_unmask_isr(UART0_ISR_PRIORITY);
ctl_global_interrupts_set(en);
UARTIntEnable(UART0_BASE, (UART_INT_OE | UART_INT_FE | UART_INT_RT | /*UART_INT_TX |*/ UART_INT_RX) );
UARTEnable(UART0_BASE);
}

//TODO
CTL_ISR_FN_t uart0_isr_handler(void)
{
 int32u uart_status, lchar;
 int8u i, sending_char;
 uart_status = UARTIntStatus(UART0_BASE,true);
 UARTIntClear(UART0_BASE,uart_status);
 
 if(uart_status & UART_INT_TX)
 {
  uart0_stat.tx_interrupts++;
  while(UARTSpaceAvail(UART0_BASE))
  {
   if(ring_buffer_pop(&uart0_stat.tx_buffer,&sending_char) == NO_ERROR)
   {
    GPIOPinWrite(RGB_LED_PORT,RGB_LED_BLUE_PIN,RGB_LED_BLUE_PIN); 
    UARTCharPutNonBlocking(UART0_BASE,sending_char);
    GPIOPinWrite(RGB_LED_PORT,RGB_LED_BLUE_PIN,RGB_LED_BLUE_PIN); 
   }
   else
   {
    UARTIntDisable(UART0_BASE, UART_INT_TX); //Ã¼res a TX buffer, letiltjuk a TX interruptot addig
    break;
   }
  }
 }
 if(uart_status & (UART_INT_RX | UART_INT_RT)) //jött valami
 {
  uart0_stat.rx_interrupts++;
  i = 0;
  while(UARTCharsAvail(UART0_BASE)) //szedjÃ¼k ki
  {
   GPIOPinWrite(RGB_LED_PORT,RGB_LED_RED_PIN,RGB_LED_RED_PIN);
   lchar = UARTCharGetNonBlocking(UART0_BASE);
   GPIOPinWrite(RGB_LED_PORT,RGB_LED_RED_PIN,0);
   
   if(ring_buffer_push(&uart0_stat.rx_buffer,(int8u)lchar))
   {
    uart0_stat.buffer_overflow++;
    break;
   }
   else
   {
    uart0_stat.received++; //debughoz való csak
	ctl_semaphore_signal(&uart0_stat.rx_sem);
   }
   if(ring_buffer_is_full(&uart0_stat.rx_buffer)) //tele van az rx puffer?
    break;
  }
  
 }
 if(uart_status & UART_INT_FE) //framing error
 {
  uart0_stat.framing_error_interrupts++;
 }
 if(uart_status & UART_INT_OE) //overrun error
 {
  uart0_stat.overrun_error_interrupts++;
 }
}

//BASIC UART FUNCTIONS
int8u uart0_get_char(int8u* c, int32u timeout_ms)
{
 int en;
 if(ctl_semaphore_wait(&uart0_stat.rx_sem,CTL_TIMEOUT_DELAY, timeout_ms) != 0)
 {
  en = ctl_global_interrupts_set(0);
  if(ring_buffer_pop(&uart0_stat.rx_buffer, c) == NO_ERROR)
  {  
  ctl_global_interrupts_set(en);
  return UART_GET_OK;
  }
  else
  {  
  ctl_global_interrupts_set(en);
  return UART_BUFF_EMPTY; //ide soha nem szabad befutnia
  }
 }
 else
 {
  ctl_global_interrupts_set(en);
  return UART_TIMEOUT; //timeout
 }
}

int8u uart0_put_char(int8u c)
{
 int en;
 int8u tx_char;
 ctl_semaphore_wait(&uart0_stat.tx_sem,CTL_TIMEOUT_NONE,0); //addig várakoztatjuk a taskot míg küldhetünk
 en = ctl_global_interrupts_set(0);
 if(ring_buffer_push(&uart0_stat.tx_buffer, c) == NO_ERROR)
 {
  while(UARTSpaceAvail(UART0_BASE))
  {
   if(ring_buffer_pop(&uart0_stat.tx_buffer,&tx_char) == NO_ERROR)
   {
    ctl_semaphore_signal(&uart0_stat.tx_sem); //sikerült kiszedni a tx pufferbõl a karaktert
	UARTCharPutNonBlocking(UART0_BASE,tx_char);
	UARTIntEnable(UART0_BASE, UART_INT_TX); //be kell kapcsolni ezt, mert ugye még az interruptok le vannak tiltva...
   }
   else //RING_BUFF_EMPTY
   {
	UARTIntDisable(UART0_BASE,UART_INT_TX);
        break;
   }
  }
  ctl_global_interrupts_set(en);
  return UART_PUT_OK;
 }
 else
 {
  ctl_global_interrupts_set(en);
  return UART_BUFF_OVERFLOW;  //ide nem futhat sose elméletileg...
 }
}


//INIT
void uart0_init(int32u baud)
{
  memset(&uart0_stat,0,sizeof(uart_struct));
  ctl_semaphore_init(&uart0_stat.rx_sem, 0);
  ctl_semaphore_init(&uart0_stat.tx_sem, UART_TX_BUFFSIZE);
  ring_buffer_init(&uart0_stat.rx_buffer);
  ring_buffer_init(&uart0_stat.tx_buffer);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  GPIOPinConfigure(GPIO_PA0_U0RX); 
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE,(GPIO_PIN_0 | GPIO_PIN_1));
  UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8,UART_FIFO_RX4_8);
  UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(),baud, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
  uart0_isr_init((CTL_ISR_FN_t)uart0_isr_handler);
}

