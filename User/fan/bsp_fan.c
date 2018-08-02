#include "./fan/bsp_fan.h"

void FAN_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;                /*定义一个GPIO_InitTypeDef类型的结构体*/
	
	/*开启LED相关的GPIO外设时钟*/
	RCC_APB2PeriphClockCmd( FAN_GPIO_CLK|LED_GPIO_CLK, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = FAN_GPIO_PIN;	        /*选择要控制的GPIO引脚*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    /*设置引脚模式为通用推挽输出*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   /*设置引脚速率为50MHz */ 

	GPIO_Init(FAN_GPIO_PORT, &GPIO_InitStructure);	    /*调用库函数，初始化GPIO*/
	
	/*开启LED闪烁的外设*/
	GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN;	        /*选择要控制的GPIO引脚*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    /*设置引脚模式为通用推挽输出*/
	
	GPIO_Init(FAN_GPIO_PORT, &GPIO_InitStructure);
} 

