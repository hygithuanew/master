	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "./fan/bsp_fan.h"
#include "./TIMbase/TIMbase.h"
#include "./key/key.h"
#include "./IWDG/IWDG.h"
#include "./ADC/adc.h"
#include "./usart/usart.h"
#include "./flash/bsp_ili9341_lcd.h"
#include "./xianshi/xianshi.h"
#include "./SPI_Flash/bsp_spi_flash.h"
#include "./i2c/bsp_i2c_gpio.h"
#include "./i2c/bsp_i2c_ee.h"
#include "stdio.h"

// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue;

// ���ݰ�����Ч���ݺ����ݳ���
extern u8 UART_RD_temp[40];
extern int entexn_len;
extern u8 time_enbleflag;

// �ֲ����������ڱ���ת�������ĵ�ѹֵ 	 
float ADC_ConvertedValueLocal; 

/*�������¶�ֵ*/
 u16 Current_Temp;  
int shi = 0,fen = 0,miao = 0;
uint8_t T_GeWei,T_ShiWei;
uint8_t write_temp[4096] = {0};
uint8_t read_temp[4096]={0};

volatile uint32_t time = 0; // ms ��ʱ���� 
void delay(unsigned int count)
{
  for(;count!=0;count--);
}

void Init(void);
void Time_Init(void);
void Time_ShiFenMiao(void);
void Read_TimeRecord(void);
void Calculate_Temperature(void);
void LCD_Show_Temperature(void);
void Write_Temperature_Data(void);
void Usart_Control(u8 * U_data);
void read_eeprom(void);




uint8_t Int [] =
{
	'0','1','2','3','4',
	'5','6','7','8','9'
};

int GeWei,ShiWei;
uint8_t i=0,j=0,g=0;
int z=0;
uint8_t a,c,d,num;
uint8_t b[100] = {0};
char *temp_getchar;
u8 usart_temp = 0;
u8 i2c_shifenmiao_j[4] = {0};
u8 i2c_read_shifenmiao_j[4] ;
//	
///*������*/
//		union 
//	{
//		double a;
//		uint8_t tmep1[sizeof(temp_union)];
//	}FS2_sep;
	

int main(void)
{	
	a=0;
	Init();/*�ܳ�ʼ��*/
	
/**************************************************/
	
	GPIO_ResetBits(FAN_GPIO_PORT,FAN_GPIO_PIN);
	
	read_eeprom();
	
	while(1)
	{
//		IWDG_Config(IWDG_Prescaler_64 ,1875);/* IWDG 3s ��ʱ���*/
//		IWDG_Feed();
		if ( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON &&UART_RD_temp[0]!=0&&UART_RD_temp[0]!=1&&(time_enbleflag == 1))
		{	
			  num = time;
        Usart_Control(UART_RD_temp);	
			  printf("\r\n�޸ĳɹ�\r\n");
			  time_enbleflag = 0;
			 while( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON);
			  time = num;
			} 
		else if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON)
		{
			num = time;
			while(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON);
			time = num;
		}
		
			if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON)
			{
				num = time;
		
				SPI_FLASH_Erase_Sector(0);/*����Flash*/;
				j = 0;
				printf("\r\n�Ѳ���Flash\r\n");
				time = num;
			}
			
		if ( time == 1000) /* 1000 * 1 ms = 1s ʱ�䵽 */
			{	
				Calculate_Temperature();//�ѵ�ѹת������ֵ
				LCD_Show_Temperature();//LCD��ʾ�¶�
			}
			
		Write_Temperature_Data();//���¶ȼ�¼����д��flash
			
		if(UART_RD_temp[0] == 1)
		{
		Read_TimeRecord();//Flash��ȡ������¶�
		UART_RD_temp[usart_temp]  = 0;
		}
				
	}
}

/*********************************************END OF FILE**********************/

void Init(void)
{
	FAN_GPIO_Init();/* LED �˿ڳ�ʼ�� */	
	BASIC_TIM_Init();/*������ʱ����ʼ�� */	
	Key_GPIO_Config();/* �����˿ڳ�ʼ�� */	
	ADCx_Init();    /* ADC�˿ڳ�ʼ�� */
	USART_Config(); /* ���ڳ�ʼ�� */
	ILI9341_Init(); /* Һ����ʼ�� */
	delay(0xffff);
	ILI9341_Open_Window(0,0,240,320);/*Һ��ȫ������ʼ�� */
	delay(0xff);
	LCD_XianShi_Init();/*Һ����Ļ��ʾ��ʼ��*/
  delay(0xff);
	Time_Init();
	delay(0xff);
	SPI_GPIO_Config();/*SPI��ʼ��*/
  i2c_GPIO_Config();/*��ʼ��i2c*/
	//SPI_FLASH_Erase_Sector(0);/*����Flash*/
}

void Time_Init(void)
{
	shi = 0, fen = 0, miao = 0;
	T_GeWei = shi % 10;
	T_ShiWei = shi / 10;
	LCD_According_8_16(65,56,BLUE,Int[T_ShiWei]);
  LCD_According_8_16(73,56,BLUE,Int[T_GeWei]);//ʱ
	
	T_GeWei = fen % 10;
	T_ShiWei = fen / 10;
	LCD_According_8_16(89,56,BLUE,Int[T_ShiWei]);
  LCD_According_8_16(97,56,BLUE,Int[T_GeWei]);//��
	
	T_GeWei = miao % 10;
	T_ShiWei = miao / 10;
	LCD_According_8_16(113,56,BLUE,Int[T_ShiWei]);
  LCD_According_8_16(121,56,BLUE,Int[T_GeWei]);//��
}

void Time_ShiFenMiao(void)
{
	miao++;
	if(miao == 60)
	{
		miao = 0;
		LCD_According_8_16(113,56,BLUE,Int[0]);
    LCD_According_8_16(121,56,BLUE,Int[0]);
		fen++;
		if(fen == 60)
		{
			fen = 0;
			LCD_According_8_16(89,56,BLUE,Int[0]);
      LCD_According_8_16(97,56,BLUE,Int[0]);
			shi++;
			if(shi == 24)
			{
				shi = 0;
				LCD_According_8_16(65,56,BLUE,Int[0]);
        LCD_According_8_16(73,56,BLUE,Int[0]);
			}
			else
			{
				T_GeWei = shi % 10;
	      T_ShiWei = shi / 10;
				ILI9341_Open_Window(65,56,16,16);
				LCD_According_8_16(65,56,BLUE,Int[T_ShiWei]);
        LCD_According_8_16(73,56,BLUE,Int[T_GeWei]);
			}
		}
		else
		{
			T_GeWei = fen % 10;
	    T_ShiWei = fen / 10;
			ILI9341_Open_Window(89,56,16,16);
			LCD_According_8_16(89,56,BLUE,Int[T_ShiWei]);
      LCD_According_8_16(97,56,BLUE,Int[T_GeWei]);
		}
	}
	else
	{
	T_GeWei = miao % 10;
	T_ShiWei = miao / 10;
	ILI9341_Open_Window(113,56,16,16);
	LCD_According_8_16(113,56,BLUE,Int[T_ShiWei]);
  LCD_According_8_16(121,56,BLUE,Int[T_GeWei]);
	}
}

void Read_TimeRecord(void)
{
	num = time;
					if(j == 0)
						{
							printf("\r\n�¶�û�г���40���ʱ���¼\r\n");
						}
						else
						{
							printf("\r\n�¶ȼ�¼ʱ���\r\n");
							SPI_FLASH_Read_Buffer(0,read_temp,4*j);
							for(i=0;i<4*j;i += 4)
							{
								printf("\r\nʱ��:%02d-%02d-%02d\r\n",read_temp[i+3],read_temp[i+2],read_temp[i+1]-2);
								printf("�¶�:%2d��\r\n",read_temp[i]);
							}
						}
					time = num;
}

void Calculate_Temperature(void)
{
				time = 0;
				ILI9341_Open_Window(65,32,16,16);
				/* LED1 ȡ�� */      
				LED_TOGGLE;
				
				Time_ShiFenMiao();
	      i2c_shifenmiao_j[0] = miao ;
				i2c_shifenmiao_j[1] = fen ;
				i2c_shifenmiao_j[2] = shi ;
	      i2c_shifenmiao_j[3] = j;
	      ee_WRITE_BYTES(0,i2c_shifenmiao_j,4);

				Current_Temp= (2.69 - ADC_ConvertedValue*3.3/4096)*1000 / 9.3+ 25 ;
        				
        ShiWei = Current_Temp/10;
				GeWei  = Current_Temp%10;
}

void LCD_Show_Temperature(void)
{
	if(ShiWei>=4)
		{
		LCD_According_8_16(65,32,RED,Int[ShiWei]);
		LCD_According_8_16(73,32,RED,Int[GeWei]);
		}
		else
		{
		LCD_According_8_16(65,32,BLUE,Int[ShiWei]);
		LCD_According_8_16(73,32,BLUE,Int[GeWei]);
		}
		
		a = Current_Temp;
		if(Current_Temp >= 40)
		{
			z++;
			b[z] = Current_Temp;
			d = z;
		}
}

void Write_Temperature_Data(void)
{
	if( a<40&&b[z]>=40)
				{
					c= 0;
					delay(10);
				for(z=1;z < d+1;z++)
				{
					delay(10);
					c = c>b[z]?c:b[z];// �Ƚ�c��b�ĸ�������
				}
				z = 0;
				b[z] = 0;
				for(i=4*j;i<4*j+1;i++)
				{
					write_temp[i] = c;
					write_temp[i+1] = miao;
					write_temp[i+2] = fen;
					write_temp[i+3] = shi;
				}
				j++;
				SPI_FLASH_Write_Buff(4*(j-1),&write_temp[4*(j-1)],4);


				}	
}

void Usart_Control(u8 * U_data)
{
	//ͨ�����ݰ�д������
			switch(U_data[usart_temp])
			{
				case 2:
					miao = U_data[usart_temp+1];
				  U_data[usart_temp]  = 0;
				  break;
				case 3:
					fen = U_data[usart_temp+1];
				  U_data[usart_temp]  = 0;
				  T_GeWei = fen % 10;
					T_ShiWei = fen / 10;
					ILI9341_Open_Window(89,56,16,16);
					LCD_According_8_16(89,56,BLUE,Int[T_ShiWei]);
					LCD_According_8_16(97,56,BLUE,Int[T_GeWei]);
				  break;		
				case 4:
					shi = U_data[usart_temp+1];
				
				  U_data[usart_temp]  = 0;
					T_GeWei = shi % 10;
					T_ShiWei = shi / 10;
					ILI9341_Open_Window(65,56,16,16);
					LCD_According_8_16(65,56,BLUE,Int[T_ShiWei]);
					LCD_According_8_16(73,56,BLUE,Int[T_GeWei]);
				  break;			
			}
}

void read_eeprom(void)
{
	ee_READ_BYTES(0,i2c_read_shifenmiao_j,4);
	
	miao = i2c_read_shifenmiao_j[0];
	T_GeWei = miao % 10;
	T_ShiWei = miao / 10;
	LCD_According_8_16(113,56,BLUE,Int[T_ShiWei]);
  LCD_According_8_16(121,56,BLUE,Int[T_GeWei]);
	fen  = i2c_read_shifenmiao_j[1];
	T_GeWei = fen % 10;
	T_ShiWei = fen / 10;
	ILI9341_Open_Window(89,56,16,16);
	LCD_According_8_16(89,56,BLUE,Int[T_ShiWei]);
	LCD_According_8_16(97,56,BLUE,Int[T_GeWei]);
	shi  = i2c_read_shifenmiao_j[2];
	T_GeWei = shi % 10;
	T_ShiWei = shi / 10;
	ILI9341_Open_Window(65,56,16,16);
	LCD_According_8_16(65,56,BLUE,Int[T_ShiWei]);
	LCD_According_8_16(73,56,BLUE,Int[T_GeWei]);
	
	j    = i2c_read_shifenmiao_j[3];
	time =0 ;
}
