// Copyright (c) 2009 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#include <ctl_api.h>
#if 0
#include "system_LM4F.c"
#else
#include "LM4F120H5QR.h"
void SystemInit (void) __attribute__ ((section (".init")));
extern uint32_t SystemCoreClock  __attribute__ ((section(".non_init")));

uint32_t SystemCoreClock;

#define delay2(count) { \
  volatile int i=0; \
  while (i++<count); \
}

#define RCC_MOSCDIS_MASK 0x1
#define RCC_MOSCDIS 0x1
#define RCC_MOSCDIS_BIT 0
#define RCC_IOSCDIS_MASK 0x2
#define RCC_IOSCDIS 0x2
#define RCC_IOSCDIS_BIT 1
#define RCC_OSCSRC_MASK 0x30
#define RCC_OSCSRC_BIT 4
#define RCC_XTAL_MASK 0x7C0
#define RCC_XTAL_BIT 6
#define RCC_BYPASS_MASK 0x800
#define RCC_BYPASS 0x800
#define RCC_BYPASS_BIT 11
#define RCC_PWRDN_MASK 0x2000
#define RCC_PWRDN 0x2000
#define RCC_PWRDN_BIT 13
#define RCC_PWMDIV_MASK 0xE0000
#define RCC_PWMDIV_BIT 17
#define RCC_USERPWMDIV_MASK 0x100000
#define RCC_USERPWMDIV 0x100000
#define RCC_USERPWMDIV_BIT 20
#define RCC_USERSYSDIV_MASK 0x400000
#define RCC_USERSYSDIV 0x400000
#define RCC_USERSYSDIV_BIT 22
#define RCC_SYSDIV_MASK 0x7800000
#define RCC_SYSDIV_BIT 23
#define RCC_ACG_MASK 0x8000000
#define RCC_ACG 0x8000000
#define RCC_ACG_BIT 27

#define RCC2_OSCSRC2_MASK 0x70
#define RCC2_OSCSRC2_BIT 4
#define RCC2_BYPASS2_MASK 0x800
#define RCC2_BYPASS2 0x800
#define RCC2_BYPASS2_BIT 11
#define RCC2_PWRDN2_MASK 0x2000
#define RCC2_PWRDN2 0x2000
#define RCC2_PWRDN2_BIT 13
#define RCC2_USBPWRDN_MASK 0x4000
#define RCC2_USBPWRDN 0x4000
#define RCC2_USBPWRDN_BIT 14
#define RCC2_SYSDIV2LSB_MASK 0x400000
#define RCC2_SYSDIV2LSB 0x400000
#define RCC2_SYSDIV2LSB_BIT 22
#define RCC2_SYSDIV2_MASK 0x1F800000
#define RCC2_SYSDIV2_BIT 23
#define RCC2_DIV400_MASK 0x40000000
#define RCC2_DIV400 0x40000000
#define RCC2_DIV400_BIT 30
#define RCC2_USERCC2_MASK 0x80000000
#define RCC2_USERCC2 0x80000000
#define RCC2_USERCC2_BIT 31

#define RIS_BORRIS_MASK 0x2
#define RIS_BORRIS 0x2
#define RIS_BORRIS_BIT 1
#define RIS_PLLRIS_MASK 0x40
#define RIS_PLLRIS 0x40
#define RIS_PLLRIS_BIT 6
#define RIS_USBPLLRIS_MASK 0x80
#define RIS_USBPLLRIS 0x80
#define RIS_USBPLLRIS_BIT 7
#define RIS_MOSCPUPRIS_MASK 0x100
#define RIS_MOSCPUPRIS 0x100
#define RIS_MOSCPUPRIS_BIT 8

void SystemInit()
{
  unsigned rcc = SYSCTL->RCC;
  // enable MOSC and wait for a time for it to stablise
  rcc &= ~RCC_MOSCDIS_MASK;
  SYSCTL->RCC = rcc;
  delay2(524288);
  // select 16mhz xtal, MOSC oscsrc and clear PLL pwrdn
  rcc &= ~(RCC_XTAL_MASK|RCC_OSCSRC_MASK|RCC_PWRDN_MASK);
  rcc |= (0x15<<RCC_XTAL_BIT)|(0<<RCC_OSCSRC_BIT)|(0<<RCC_PWRDN_BIT);
  SYSCTL->RCC = rcc;

  unsigned rcc2 =  SYSCTL->RCC2;
  rcc2 &= ~(0x7F << 22);
  rcc2 |= RCC2_USERCC2 | 0x40000000 | (4 << 22);  // 4 = 80MHz

  rcc2 &= ~(7<<4);   // select main oscillator
  rcc2 &= ~(1<<13);  // clear PWRDN2

  SYSCTL->RCC2 = rcc2;
  // wait for PLL to lock
  while (!(SYSCTL->RIS & RIS_PLLRIS)); 
  // clear bypass
  rcc2 &= ~(RCC2_BYPASS2_MASK);   
  SYSCTL->RCC2 = rcc2;

  SystemCoreClock = 80000000; // 80 Mhz
}
#endif

void
delay(int count)
{
  volatile int i=0;
  while (i++<count);
}

void 
ctl_board_init(void)
{
  SYSCTL->RCGCGPIO |= 1<<5; // clock GPIOF
  delay(1);
  GPIOF->DIR |= 1<<1; // PF1 is output
  GPIOF->DEN |= 1<<1; // PF1 is digital
}

void 
ctl_board_set_leds(unsigned set)
{
  if (set)
    *((unsigned *)(GPIOF)+(1<<1)) = 0xff;
  else
    *((unsigned *)(GPIOF)+(1<<1)) = 0;
}

static CTL_ISR_FN_t userButtonISR;

//void
//GPIO_Port_F_ISR(void)
//{
//  ctl_enter_isr();
//  userButtonISR();  
//  GPIOF->ICR = (1<<4);
//  ctl_exit_isr();
//}

void 
ctl_board_on_button_pressed(CTL_ISR_FN_t buttonFn)
{
  userButtonISR = buttonFn;  
  delay(1);
  GPIOF->DEN |= 1<<4; // PM4 is digital
  GPIOF->PUR |= 1<<4; // enable pull up
  GPIOF->IM = 1<<4; // interrupt on PB4 edge
  ctl_set_priority(GPIOF_IRQn, 1);
  ctl_unmask_isr(GPIOF_IRQn);
}
