#ifndef __BSP_FAN_H
#define	__BSP_FAN_H

#include "stm32f10x.h"

#define FAN_GPIO_PORT     GPIOC
#define FAN_GPIO_CLK      RCC_APB2Periph_GPIOC
#define FAN_GPIO_PIN      GPIO_Pin_2

#define LED_GPIO_PORT    	GPIOC			              /* GPIO�˿� */
#define LED_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO�˿�ʱ�� */
#define LED_GPIO_PIN		  GPIO_Pin_3	



/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ
#define digitalToggle(p,i) {p->ODR ^=i;} //�����ת״̬


/* �������IO�ĺ� */
#define LED_TOGGLE		 digitalToggle(LED_GPIO_PORT,LED_GPIO_PIN)
#define LED_OFF		   digitalHi(LED_GPIO_PORT,LED_GPIO_PIN)
#define LED_ON			   digitalLo(LED_GPIO_PORT,LED_GPIO_PIN)


void FAN_GPIO_Init(void);


#endif /* __LED_H */


