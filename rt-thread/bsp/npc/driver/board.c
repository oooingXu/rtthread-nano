/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-24                  the first version
 */

#include <rthw.h>
#include <rtthread.h>
#include <am.h>
#include <klib.h>
#include <klib-macros.h>

//static volatile uint32_t led;
//#define LED_ADDR 0x80200040
//
//void led_init() {
//	*(volatile uint32_t *)LED_ADDR = 0x00000001;
//}
//
//void led_on() {
//	uint32_t tmp = *(volatile uint32_t *)LED_ADDR;
//	*(volatile uint32_t *)LED_ADDR = tmp << 1 | ((tmp & 0x80000000) >> 31);
//}

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
/*
 * Please modify RT_HEAP_SIZE if you enable RT_USING_HEAP
 * the RT_HEAP_SIZE max value = (sram size - ZI size), 1024 means 1024 bytes
 */
#define RT_HEAP_SIZE (50*1024)
static rt_uint8_t rt_heap[RT_HEAP_SIZE];

RT_WEAK void *rt_heap_begin_get(void)
{
    //return heap.start;
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
		//return heap.end;
    return rt_heap + RT_HEAP_SIZE;
}
#endif

void rt_os_tick_callback(void)
{
    rt_interrupt_enter();
    
    rt_tick_increase();

    rt_interrupt_leave();
}

/**
 * This function will initial your board.
 */
void rt_hw_board_init(void)
{
//#error "TODO 1: OS Tick Configuration."
    /* 
     * TODO 1: OS Tick Configuration
     * Enable the hardware timer and call the rt_os_tick_callback function
     * periodically with the frequency RT_TICK_PER_SECOND. 
     */

		//led_on();
    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
		printf("rt_system_heap_init: heap_start = 0x%08x, heap_end = 0x%08x\n", rt_heap_begin_get(), rt_heap_end_get());
#endif

//#ifdef RT_USING_CONSOLE
//		rt_console_set_device("uart");
//#endif

#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
		//printf("rt_components_board_init\n");
#endif
		//led_on();

}

#ifdef RT_USING_CONSOLE

static int uart_init(void)
{
//#error "TODO 2: Enable the hardware uart and config baudrate."
    return 0;
}
INIT_BOARD_EXPORT(uart_init);

void rt_hw_console_output(const char *str)
{
	putstr(str);
//#error "TODO 3: Output the string 'str' through the uart."
}

#endif

int main() {
	ioe_init();
	extern void __am_cte_init();
	__am_cte_init();
	//led_init();
	//led_init();
	//while(1) {
	//	led_on();
	//	rt_thread_mdelay(500);
	//	printf("rt_thread_mdelay(500)\n");
	//}
	extern int entry(void);
	entry();
	return 0;
}

void clear() {
	printf("\033[H\033[J");
}

MSH_CMD_EXPORT(clear, clear);
