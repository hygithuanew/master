#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f10x.h"



// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
/********************ADC1����ͨ�������ţ�����**************************/
#define    ADCx                          ADC1
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_CLK                       RCC_APB2Periph_ADC1

#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOC  
#define    ADC_PORT                      GPIOC

// PC1-ͨ��11 ����IO
#define    ADC_PIN                       GPIO_Pin_0
#define    ADC_CHANNEL                   ADC_Channel_10//PC1��ӦADC1��ͨ��11
#define    ADC_DMA_CHANNEL               DMA1_Channel1//��ӦADC��DMA��ͨ����

void ADCx_Init(void);

#endif /* __BSP_ADC_H */
