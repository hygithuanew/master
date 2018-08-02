#ifndef __BSP_FAN_H
#define	__BSP_FAN_H

#include "stm32f10x.h"

#define FAN_GPIO_PORT     GPIOC
#define FAN_GPIO_CLK      RCC_APB2Periph_GPIOC
#define FAN_GPIO_PIN      GPIO_Pin_2

#define LED_GPIO_PORT    	GPIOC			              /* GPIO端口 */
#define LED_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
#define LED_GPIO_PIN		  GPIO_Pin_3	



/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 //输出低电平
#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态


/* 定义控制IO的宏 */
#define LED_TOGGLE		 digitalToggle(LED_GPIO_PORT,LED_GPIO_PIN)
#define LED_OFF		   digitalHi(LED_GPIO_PORT,LED_GPIO_PIN)
#define LED_ON			   digitalLo(LED_GPIO_PORT,LED_GPIO_PIN)


void FAN_GPIO_Init(void);


#endif /* __LED_H */


