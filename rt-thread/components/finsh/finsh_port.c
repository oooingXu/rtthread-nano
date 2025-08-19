/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */

#include <rthw.h>
#include <rtconfig.h>

#ifndef RT_USING_FINSH
#error Please uncomment the line <#include "finsh_config.h"> in the rtconfig.h 
#endif

#ifdef RT_USING_FINSH

//RT_WEAK char rt_hw_console_getchar(void)
//{
//  static const char *p = "help\nlist_thread\ncoremark\nlist_thread\n";
//  static int builtin = 1;
//  if(builtin){
//    if(*p != '\0'){
//      return *(p++);
//    }else{
//      builtin = 0;
//    }
//  }
//    return -1;
//}
#define UART_RX		 0x10000008
#define UART_PMPTY 0x1000000c

static const char *p = "ps\ncoremark\nlist_thread\n";
static int builtin = 1;

RT_WEAK char rt_hw_console_getchar(void)
{
  if(builtin){
    if(*p != '\0'){
      return *(p++);
    }else{
      builtin = 0;
    }
  }
    /* Note: the initial value of ch must < 0 */
		char empty = *(volatile char *)UART_PMPTY;
		while(empty == 1) {
			empty = *(volatile char *)UART_PMPTY;
		}
		char ch = *(volatile char *)UART_RX;

//#error "TODO 4: Read a char from the uart and assign it to 'ch'."

    return ch;
}

#endif /* RT_USING_FINSH */

