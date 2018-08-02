/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "./TIMbase/TIMbase.h"
#include "./usart/usart.h"


extern volatile uint32_t time;

uint8_t uctemp;
uint8_t UART_RD_DATA[40]={0};
uint8_t UART_RD_temp[40]={0};

int usart_num;
int entexn_len;
u8 usart_rx_enbleflag =0 ;  //����״̬��� ��Ϊ0��ʾ��û�н���
u8 usart_rd_len =0;         //������Ϣ�����ݳ���
u8 usart_rd_lentemp=0;      //������¼�Ѷ�ȡ�����ݳ���
u8 usart_rx_lenrightflag=0; //���ݳ���У��λ��ȷ��־
u8 usart_rx_successflag=0;  //�ɹ����յ�������Ϣ
u8 check_temp = 0;
u8 time_enbleflag=0;//�ж�����ʱ���Ƿ񳬹���Χ

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}


void  BASIC_TIM_IRQHandler (void)
{
	if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ) //�ж϶�ʱ���Ƿ������£�������ж�
	{	
		time++;
		TIM_ClearITPendingBit(BASIC_TIM , TIM_FLAG_Update);////��������ж�����λ  		 
	}		 	
}

/*��������У��            (��ͷ0xee)(���ݳ���)(...����...)(���ݳ��ȷ���)(��β0xef) */
void DEBUG_USART_IRQHandler(void)
{

	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET) //ÿ�ν���һ���ֽڣ�8λ��
	{
		 uctemp = USART_ReceiveData(DEBUG_USARTx);
			   //printf("\r\n%d\r\n",usart_rx_enbleflag);
			if(usart_rx_enbleflag==1)                  //���������ݱ�־��λʱ����������
			{
			//  uctemp = USART_ReceiveData(DEBUG_USARTx);
				if(usart_rd_lentemp==0) 
				{
					usart_rd_len = uctemp; 				  	//��ȡ���ݵĳ���  
					entexn_len = usart_rd_len;
					
					if(usart_rd_len>=40)
					{
						usart_rx_lenrightflag=0;   
            usart_rx_successflag=0;
            usart_rx_enbleflag=0;	
            usart_rd_len=0; 
            usart_rd_lentemp=0;
						usart_num=0;
            printf("\r\n��������ݳ�����41��\r\n"); 						
						
					}
				}
        else if(usart_rd_lentemp==usart_rd_len+1)//����ȡ����usart_rd_lentemp+1������ʱ��У���Ƿ��ǳ�����Ϣ�ķ���
         {
					  check_temp=~usart_rd_len; //ȡ���ݳ���У��λ�ķ���
					 if(uctemp==check_temp)//�����ݳ���У����ȷʱ
					 {
						 usart_rx_lenrightflag=1;//���ݳ���У���־��һ
					 }
           else  //�����ݳ���У�����ʱ
					 {
						 usart_rx_lenrightflag=0;           //���ݳ���У������
             usart_rx_successflag=0;            //���ݽ��ճɹ���־����
             usart_rx_enbleflag=0;             //�����ݳ���У�����ʱ�����ݽ���������־����
             usart_rd_len=0;                   //���ݳ�������
             usart_rd_lentemp=0;               //���ݳ����ݴ������� 
						 usart_num=0;
						 printf("\r\n���ݳ��ȷ��벻���\r\n"); 
					 }						 
				 }
         else if(usart_rd_lentemp==usart_rd_len+2)//����ȡ����usart_rd_lentemp+2������ʱ��У���β�Ƿ���ȷ
				 {
					  if((uctemp==0xef)&&(usart_rx_lenrightflag==1))                //�����β�����볤��У�鶼��ȷ
						{
						 usart_rx_lenrightflag=0;           //���ݳ���У������
             usart_rx_successflag=1;            //���ݽ��ճɹ�
             usart_rx_enbleflag=0;             //�����ݳ���У�����ʱ�����ݽ���������־����
             usart_rd_lentemp=0;  							//���ݳ����ݴ������� 
								
						}
            else
						{
						 usart_rx_successflag=0;
						 usart_rx_enbleflag =0 ;  //����״̬��� ��Ϊ0��ʾ��û�н���
						 usart_rd_lentemp=0;      //������¼�Ѷ�ȡ�����ݳ���
						 usart_rx_lenrightflag=0; //���ݳ���У��λ��ȷ��־
						 usart_rx_successflag=0;  //�ɹ����յ�������Ϣ
						 check_temp = 0;		 
						 usart_num = 0;
						 usart_rd_len =0; 
						 printf("\r\n��β����0xef\r\n");
						}							
				 }
         else
				 {
					UART_RD_DATA[usart_rd_lentemp-1] = uctemp; //��usart_rd_lentempΪ���ݶ�ʱ�������ݴ浽�������ݽ��ռĴ�����
//					  printf("\r\n%d\r\n",uctemp);
				 }
				 
				 usart_rd_lentemp++;//ÿ�μ�¼���ݣ����ݳ����ݴ����Լ�
 			}
	if((uctemp==0xee)&&(usart_rx_enbleflag==0))//�����ܵ���ͷ(0xee)���ݲ��һ�û�гɹ�������������Ϣ
		                                         //�����ͷ���ֱ�ӽ�����һ��if��ʹ���ݳ���ҲΪ0xEE���ʹ�����
			{
				 usart_rx_enbleflag=1;                   //˵�����ǰ�ͷ�������������ݱ�־���������ݽ��ս׶�
			} 		
			if(usart_rx_successflag ==1)
			 { 
				 for(usart_num=0;usart_num<usart_rd_len;usart_num++)
				 {
					 UART_RD_temp[usart_num] = UART_RD_DATA[usart_num];
				 }
				 switch(UART_RD_temp[0])
				{
					case 1:
						break;
					case 2:
						
						if( UART_RD_temp[1] <60) //����ʹ��i< x <j�����ʽ
						{
						printf("\r\n��key1ȷ���޸�����Ϊ%d\r\n",UART_RD_temp[1]);
					  time_enbleflag = 1;
						}
						else
						printf("\r\n�����ʱ�䲻�ԣ�����������\r\n");
					break;
					case 3: 
						if( UART_RD_temp[1] <60)
						{
						printf("\r\n��key1ȷ���޸ķ���Ϊ%d\r\n",UART_RD_temp[1]);
					  time_enbleflag = 1;
		        }
						else
						printf("\r\n�����ʱ�䲻�ԣ�����������\r\n");
					break;
					case 4:
						if( UART_RD_temp[1] <24)
						{
						printf("\r\n��key1ȷ���޸�ʱ��Ϊ%d\r\n",UART_RD_temp[1]);
					  time_enbleflag = 1;
						}
						else
						printf("\r\n�����ʱ�䲻�ԣ�����������\r\n");
					break;
				}
         usart_rx_successflag=0;
				 usart_rx_enbleflag =0 ;  //����״̬��� ��Ϊ0��ʾ��û�н���
				 usart_rd_lentemp=0;      //������¼�Ѷ�ȡ�����ݳ���
				 usart_rx_lenrightflag=0; //���ݳ���У��λ��ȷ��־
				 usart_rx_successflag=0;  //�ɹ����յ�������Ϣ
				 check_temp = 0;		 
				 usart_rd_len =0; 
			 }
    
	}
	USART_ClearITPendingBit(DEBUG_USARTx, USART_IT_RXNE);//�ж����
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
