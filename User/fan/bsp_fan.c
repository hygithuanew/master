#include "./fan/bsp_fan.h"

void FAN_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;                /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	
	/*����LED��ص�GPIO����ʱ��*/
	RCC_APB2PeriphClockCmd( FAN_GPIO_CLK|LED_GPIO_CLK, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = FAN_GPIO_PIN;	        /*ѡ��Ҫ���Ƶ�GPIO����*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    /*��������ģʽΪͨ���������*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   /*������������Ϊ50MHz */ 

	GPIO_Init(FAN_GPIO_PORT, &GPIO_InitStructure);	    /*���ÿ⺯������ʼ��GPIO*/
	
	/*����LED��˸������*/
	GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN;	        /*ѡ��Ҫ���Ƶ�GPIO����*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    /*��������ģʽΪͨ���������*/
	
	GPIO_Init(FAN_GPIO_PORT, &GPIO_InitStructure);
} 

