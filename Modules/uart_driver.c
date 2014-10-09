//fegyizoli @ 2014.09.27.
//alap UART a raspivel kommunikÃ¡lÃ¡sra
#include <modules.h>

#ifdef COM0_EN
static CTL_ISR_FN_t com0_isr;

CTL_EVENT_SET_t com0_event;

int8u g_com0_rx_buff[COM0_RX_BUFFSIZE];
int8u g_com0_tx_buff[COM0_TX_BUFFSIZE];

int32u g_com0_framing_error;
int32u g_com0_overrun_error;
int32u g_com0_rx_timeout;
int32u g_com0_received;
int32u g_com0_transfered;

void UART0_ISR(void)
{
ctl_enter_isr();
com0_isr();
ctl_exit_isr();
}
#endif

#ifdef COM2_EN
static CTL_ISR_FN_t com2_isr;

CTL_EVENT_SET_t com2_event;

int8u g_com2_rx_buff[COM2_RX_BUFFSIZE];
int8u g_com2_tx_buff[COM2_TX_BUFFSIZE];

int32u g_com2_framing_error;
int32u g_com2_overrun_error;
int32u g_com2_rx_timeout;
int32u g_com2_received;
int32u g_com2_transfered;

void UART2_ISR(void)
{
ctl_enter_isr();
com2_isr();
ctl_exit_isr();
}
#endif


//mindig:
// 1 stop bit
// 0 paritás bit
// 8 bit adat
//tetszõleges baud rate
void uart_isr_init(int8u com, CTL_ISR_FN_t fn)
{
int32u com_base;
int en, com_irq;
en = ctl_global_interrupts_set(0);
com2_isr = fn;
switch(com)
{
 case COM0:
 com_base = COM0_BASE;
 break;
 case COM2:
 com_base = COM2_BASE;
 break;
}
UARTIntDisable(com_base, (UART_INT_OE | UART_INT_FE | UART_INT_RT | UART_INT_TX | UART_INT_RX) ); //biztosra megyek, minden false értéket kitörlök
if(UARTIntStatus(com_base,1) == UART_INT_OE)
{
 UARTIntClear(com_base,UART_INT_OE);
}
if(UARTIntStatus(com_base,1) == UART_INT_FE)
{
 UARTIntClear(com_base,UART_INT_FE);
}
if(UARTIntStatus(com_base,1) == UART_INT_RT)
{
 UARTIntClear(com_base,UART_INT_RT);
}
if(UARTIntStatus(com_base,1) == UART_INT_TX)
{
 UARTIntClear(com_base,UART_INT_TX);
}
if(UARTIntStatus(com_base,1) == UART_INT_RX)
{
 UARTIntClear(com_base,UART_INT_RX);
}
switch(com)
{
 case COM0:
 com_irq = COM0_ISR_PRIORITY;
 break;
 case COM2:
 com_irq = COM2_ISR_PRIORITY;
 break;
}
ctl_set_priority(com_irq,1);
ctl_unmask_isr(com_irq);
ctl_global_interrupts_set(en);
UARTIntEnable(com_base, (UART_INT_OE | UART_INT_FE | UART_INT_RT | UART_INT_TX | UART_INT_RX) );
UARTEnable(com_base);
}

#ifdef COM0_EN
CTL_ISR_FN_t uart0_isr_handler(void)
{
 int32u uart_status;
 
 uart_status = UARTIntStatus(COM0_BASE,true);
 UARTIntClear(COM0_BASE,uart_status);
 
 if(uart_status & UART_INT_TX) //küldeni akarnánk
 {
  g_com0_transfered++;
 }
 if(uart_status & UART_INT_RX) //jött valami
 {
  g_com0_received++;
 }
 if(uart_status & UART_INT_RT) //rx timeout
 {
  g_com0_rx_timeout++;
 }
 if(uart_status & UART_INT_FE) //framing error
 {
  g_com0_framing_error++;
 }
 if(uart_status & UART_INT_OE) //overrun error
 {
  g_com0_overrun_error++;
 }
}
#endif

#ifdef COM2_EN
CTL_ISR_FN_t uart2_isr_handler(void)
{
 int32u uart_status;
 
 uart_status = UARTIntStatus(COM2_BASE,true);
 UARTIntClear(COM2_BASE,uart_status);
 
 if(uart_status & UART_INT_TX) //küldeni akarnánk
 {
  GPIOPinWrite(RGB_LED_PORT,RGB_LED_BLUE_PIN,RGB_LED_BLUE_PIN);
  g_com2_transfered++;
  SysCtlDelay(SysCtlClockGet() / (1000 * 3)); //1ms delay
  GPIOPinWrite(RGB_LED_PORT,RGB_LED_BLUE_PIN,0);
 }
 if(uart_status & UART_INT_RX) //jött valami
 {
  GPIOPinWrite(RGB_LED_PORT,RGB_LED_GREEN_PIN,RGB_LED_GREEN_PIN);
  g_com2_received++;
  SysCtlDelay(SysCtlClockGet() / (1000 * 3)); //1ms delay
  GPIOPinWrite(RGB_LED_PORT,RGB_LED_GREEN_PIN,0);
 }
 if(uart_status & UART_INT_RT) //rx timeout
 {
  g_com2_rx_timeout++;
 }
 if(uart_status & UART_INT_FE) //framing error
 {
  g_com2_framing_error++;
 }
 if(uart_status & UART_INT_OE) //overrun error
 {
  g_com2_overrun_error++;
 }
}
#endif

//alap UART függvények
int8u com_put_char(int8u com, int8u* tx_c)
{
 int32u com_base;

 switch(com)
 {
  case COM0:
  com_base = COM0_BASE;
  break;
  case COM2:
  com_base = COM2_BASE;
  break;
 }
 if(UARTSpaceAvail(com_base))
 {
  if(UARTCharPutNonBlocking(com_base, *tx_c)) //karakter küldés
   return NO_ERROR;
  else
   return NO_SPACE_AVAILABLE;
 }
 else
 {
  return NO_SPACE_AVAILABLE;
 }
}

int8u com_get_char(int8u com, int8u* rx_c)
{
 int32s char_l, com_base;

 switch(com)
 {
  case COM0:
  com_base = COM0_BASE;
  break;
  case COM2:
  com_base = COM2_BASE;
  break;
 }

 if(UARTCharsAvail(com_base))
 {
  char_l = UARTCharGetNonBlocking(com_base); //itt már nem lehet minuszt kapni, de azért lekérjük csak leszarjuk :)
  rx_c = (int8u*)(char_l & 0xff);
  return NO_ERROR;
 }
 else
  return EMPTY_FIFO;
}

void com_flush(int8u com, int8u what)
{
 switch(com)
 {
  case COM0:
  {
   if(what & TX)
    memset(&g_com0_tx_buff,0,sizeof(g_com0_tx_buff));
   if(what & RX)
    memset(&g_com0_rx_buff,0,sizeof(g_com0_rx_buff));
  }
  break;

  case COM2:
  {
   if(what & TX)
    memset(&g_com2_tx_buff,0,sizeof(g_com2_tx_buff));
   if(what & RX)
    memset(&g_com2_rx_buff,0,sizeof(g_com2_rx_buff));
  }
  break;
 }
}

//com init
void com_init(int8u com, int32u baud)
{
 int32u com_base, com_periph, com_module;
 switch(com)
 {
  case COM0:
  {
   g_com0_framing_error = 0;
   g_com0_overrun_error = 0;
   g_com0_rx_timeout = 0;
   g_com0_received = 0;
   g_com0_transfered = 0;
   ctl_events_init(&com0_event,0);
   com_base = COM0_BASE;
   com_periph = COM0_PERIPH;
   com_module = COM0_MODULE;
  }
  break;
  case COM2:
  {
   g_com2_framing_error = 0;
   g_com2_overrun_error = 0;
   g_com2_rx_timeout = 0;
   g_com2_received = 0;
   g_com2_transfered = 0;
   ctl_events_init(&com2_event,0);
   com_base = COM2_BASE;
   com_periph = COM2_PERIPH;
   com_module = COM2_MODULE;
  }
  break;
 }
 com_flush(com, (RX | TX));
 
 SysCtlPeripheralEnable(com_periph);
 SysCtlPeripheralEnable(com_module);
 switch(com)
 {
  case COM0:
  {
   GPIOPinConfigure(CONFIGURE_TO_COM0_RX); 
   GPIOPinConfigure(CONFIGURE_TO_COM0_TX);
   GPIOPinTypeUART(com_periph,COM0_RX_PIN | COM0_TX_PIN);
  }
  break;
  case COM2:
  {
   GPIOPinConfigure(CONFIGURE_TO_COM2_RX); 
   GPIOPinConfigure(CONFIGURE_TO_COM2_TX);
   GPIOPinTypeUART(com_periph,COM2_RX_PIN);
   GPIOPinTypeUART(com_periph,COM2_TX_PIN);
  }
  break;
 }
// UARTFIFOLevelSet(com_base, UART_FIFO_TX1_8,UART_FIFO_RX4_8);
 UARTConfigSetExpClk(com_base, SysCtlClockGet(), baud, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
 switch(com)
 {
  case COM0:
  uart_isr_init(com, (CTL_ISR_FN_t)uart0_isr_handler);
  break;
  case COM2:
  uart_isr_init(com, (CTL_ISR_FN_t)uart2_isr_handler);
  break;
 }
}