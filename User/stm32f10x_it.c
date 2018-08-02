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
u8 usart_rx_enbleflag =0 ;  //接收状态标记 ，为0表示还没有接收
u8 usart_rd_len =0;         //有用信息的数据长度
u8 usart_rd_lentemp=0;      //用来记录已读取的数据长度
u8 usart_rx_lenrightflag=0; //数据长度校验位正确标志
u8 usart_rx_successflag=0;  //成功接收到数据信息
u8 check_temp = 0;
u8 time_enbleflag=0;//判断输入时间是否超过范围

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
	if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ) //判断定时器是否发生更新（溢出）中断
	{	
		time++;
		TIM_ClearITPendingBit(BASIC_TIM , TIM_FLAG_Update);////清除更新中断请求位  		 
	}		 	
}

/*串口数据校验            (包头0xee)(数据长度)(...数据...)(数据长度反码)(包尾0xef) */
void DEBUG_USART_IRQHandler(void)
{

	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET) //每次接收一个字节（8位）
	{
		 uctemp = USART_ReceiveData(DEBUG_USARTx);
			   //printf("\r\n%d\r\n",usart_rx_enbleflag);
			if(usart_rx_enbleflag==1)                  //到接受数据标志置位时，接受数据
			{
			//  uctemp = USART_ReceiveData(DEBUG_USARTx);
				if(usart_rd_lentemp==0) 
				{
					usart_rd_len = uctemp; 				  	//读取数据的长度  
					entexn_len = usart_rd_len;
					
					if(usart_rd_len>=40)
					{
						usart_rx_lenrightflag=0;   
            usart_rx_successflag=0;
            usart_rx_enbleflag=0;	
            usart_rd_len=0; 
            usart_rd_lentemp=0;
						usart_num=0;
            printf("\r\n输入的数据超过了41个\r\n"); 						
						
					}
				}
        else if(usart_rd_lentemp==usart_rd_len+1)//当读取到第usart_rd_lentemp+1个数据时，校验是否是长度信息的反码
         {
					  check_temp=~usart_rd_len; //取数据长度校验位的反码
					 if(uctemp==check_temp)//当数据长度校验正确时
					 {
						 usart_rx_lenrightflag=1;//数据长度校验标志置一
					 }
           else  //当数据长度校验错误时
					 {
						 usart_rx_lenrightflag=0;           //数据长度校验清零
             usart_rx_successflag=0;            //数据接收成功标志清零
             usart_rx_enbleflag=0;             //当数据长度校验错误时，数据接收启动标志清零
             usart_rd_len=0;                   //数据长度清零
             usart_rd_lentemp=0;               //数据长度暂存器清零 
						 usart_num=0;
						 printf("\r\n数据长度反码不相等\r\n"); 
					 }						 
				 }
         else if(usart_rd_lentemp==usart_rd_len+2)//当读取到第usart_rd_lentemp+2个数据时，校验包尾是否正确
				 {
					  if((uctemp==0xef)&&(usart_rx_lenrightflag==1))                //如果包尾数据与长度校验都正确
						{
						 usart_rx_lenrightflag=0;           //数据长度校验清零
             usart_rx_successflag=1;            //数据接收成功
             usart_rx_enbleflag=0;             //当数据长度校验错误时，数据接收启动标志清零
             usart_rd_lentemp=0;  							//数据长度暂存器清零 
								
						}
            else
						{
						 usart_rx_successflag=0;
						 usart_rx_enbleflag =0 ;  //接收状态标记 ，为0表示还没有接收
						 usart_rd_lentemp=0;      //用来记录已读取的数据长度
						 usart_rx_lenrightflag=0; //数据长度校验位正确标志
						 usart_rx_successflag=0;  //成功接收到数据信息
						 check_temp = 0;		 
						 usart_num = 0;
						 usart_rd_len =0; 
						 printf("\r\n包尾不是0xef\r\n");
						}							
				 }
         else
				 {
					UART_RD_DATA[usart_rd_lentemp-1] = uctemp; //当usart_rd_lentemp为数据段时，将数据存到串口数据接收寄存器中
//					  printf("\r\n%d\r\n",uctemp);
				 }
				 
				 usart_rd_lentemp++;//每次记录数据，数据长度暂存器自加
 			}
	if((uctemp==0xee)&&(usart_rx_enbleflag==0))//当接受到包头(0xee)数据并且还没有成功接收完数据信息
		                                         //如果在头则会直接进入下一个if，使数据长度也为0xEE，就错啦。
			{
				 usart_rx_enbleflag=1;                   //说明这是包头，启动接收数据标志，进入数据接收阶段
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
						
						if( UART_RD_temp[1] <60) //不能使用i< x <j这类格式
						{
						printf("\r\n按key1确认修改秒数为%d\r\n",UART_RD_temp[1]);
					  time_enbleflag = 1;
						}
						else
						printf("\r\n输入的时间不对，请重新输入\r\n");
					break;
					case 3: 
						if( UART_RD_temp[1] <60)
						{
						printf("\r\n按key1确认修改分钟为%d\r\n",UART_RD_temp[1]);
					  time_enbleflag = 1;
		        }
						else
						printf("\r\n输入的时间不对，请重新输入\r\n");
					break;
					case 4:
						if( UART_RD_temp[1] <24)
						{
						printf("\r\n按key1确认修改时钟为%d\r\n",UART_RD_temp[1]);
					  time_enbleflag = 1;
						}
						else
						printf("\r\n输入的时间不对，请重新输入\r\n");
					break;
				}
         usart_rx_successflag=0;
				 usart_rx_enbleflag =0 ;  //接收状态标记 ，为0表示还没有接收
				 usart_rd_lentemp=0;      //用来记录已读取的数据长度
				 usart_rx_lenrightflag=0; //数据长度校验位正确标志
				 usart_rx_successflag=0;  //成功接收到数据信息
				 check_temp = 0;		 
				 usart_rd_len =0; 
			 }
    
	}
	USART_ClearITPendingBit(DEBUG_USARTx, USART_IT_RXNE);//中断清除
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
