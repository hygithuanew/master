
#include "./flash/bsp_ili9341_lcd.h"

#include "stdio.h"
/*1.��ʼ����Һ����ص�����
  2.ģ��8080ʱ��
	   д����
		 д����
		 ������
  3.ͨ����ȡILI9341��ID���Ժ���
	4.��LCD����XYдһ���ض���ɫ���ص�
*/

//����Һ��ɨ�跽����仯��XY���ؿ���
//����ILI9341_GramScan�������÷���ʱ���Զ�����
uint16_t LCD_X_LENGTH = ILI9341_LESS_PIXEL;
uint16_t LCD_Y_LENGTH = ILI9341_MORE_PIXEL;

//Һ����ɨ��ģʽ����������Ҫ���ڷ���ѡ�������ļ������
//������ѡֵΪ0-7
//����ILI9341_GramScan�������÷���ʱ���Զ�����
//LCD�ճ�ʼ�����ʱ��ʹ�ñ�Ĭ��ֵ
uint8_t LCD_SCAN_MODE = 6;

static void ILI9341_Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
	
}

#define ILI9341_DELAY()  //ILI9341_Delay(10)

void ILI9341_GPIO_Config (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*ʹ�ܸ���ʱ��AFIO*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	/*��PB3��PB4����������ͨ�ӿ�*/
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	
	//��λ����ֱ��ʹ��NRST�������帴λ��ʱ���ʹҺ����λ

	/* ʹ��FSMC��Ӧ��Ӧ�ܽ�ʱ��*/
	RCC_APB2PeriphClockCmd ( 	
													/*�����ź�*/
													ILI9341_CS_CLK|ILI9341_DC_CLK|ILI9341_WR_CLK|
													ILI9341_RD_CLK	|ILI9341_BK_CLK|
													/*�����ź�*/
													ILI9341_DATA_CLK, ENABLE );
	
	//����SWD��ʧ��JTAG (����PB����������jtag�ӿڣ��ĳ�SWD�ӿھͲ����и���)
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	
		
	
	/* ����Һ�����Ӧ��������,PORT-D0~D15 */	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_DATA_PIN;
	GPIO_Init ( ILI9341_DATA_PORT, &GPIO_InitStructure );	
		
	/* ����Һ�����Ӧ�Ŀ�����
	 * ��   			:LCD-RD
	 * д   			:LCD-WR
	 * Ƭѡ   		:LCD-CS
	 * ����/���� 	:LCD-DC
	 */
	GPIO_InitStructure.GPIO_Pin = ILI9341_RD_PIN; 
	GPIO_Init (ILI9341_RD_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_WR_PIN; 
	GPIO_Init (ILI9341_WR_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_CS_PIN; 
	GPIO_Init ( ILI9341_CS_PORT, & GPIO_InitStructure );  
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_DC_PIN; 
	GPIO_Init ( ILI9341_DC_PORT, & GPIO_InitStructure );
			
	/* ����LCD������ƹܽ�BK*/	
	GPIO_InitStructure.GPIO_Pin = ILI9341_BK_PIN; 
	GPIO_Init ( ILI9341_BK_PORT, &GPIO_InitStructure );
	
	LCD_BK_ON();
}
	


//д����
static void ILI9341_Write_CMD(uint16_t data)
{
	LCD_CS_LOW();
	ILI9341_DELAY();
	
	LCD_DC_LOW();//�͵�ƽд����ߵ�ƽд����
	ILI9341_DELAY();
	
	LCD_RD_HIGH();//�ߵ�ƽ��ʾд���͵�ƽ��ʾ��
	ILI9341_DELAY();
	
	LCD_DATA_OUT(data);
	ILI9341_DELAY();
	
	LCD_WR_LOW();
	ILI9341_DELAY();
	
	LCD_WR_HIGH();
	ILI9341_DELAY();
	
	LCD_CS_HIGH();
	
}

//д����
static void ILI9341_Write_Date(uint16_t data)
{
	LCD_CS_LOW();
	ILI9341_DELAY();
	
	LCD_DC_HIGH();//�͵�ƽд����ߵ�ƽд����
	ILI9341_DELAY();
	
	LCD_RD_HIGH();//�ߵ�ƽ��ʾд���͵�ƽ��ʾ��
	ILI9341_DELAY();
	
	LCD_DATA_OUT(data);
	ILI9341_DELAY();
	
	LCD_WR_LOW();
	ILI9341_DELAY();
	
	LCD_WR_HIGH();
	ILI9341_DELAY();
	
	LCD_CS_HIGH();
	ILI9341_DELAY();
	
}

//������
uint16_t ILI9341_Read_Date(void)
{
	uint16_t data;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU ;
	GPIO_InitStructure.GPIO_Pin = ILI9341_DATA_PIN;
	GPIO_Init ( ILI9341_DATA_PORT, &GPIO_InitStructure );	
	
	LCD_CS_LOW();
	ILI9341_DELAY();
	
	LCD_DC_HIGH();//�͵�ƽд����ߵ�ƽд����
	ILI9341_DELAY();
	
		
	LCD_WR_HIGH();
	ILI9341_DELAY();
	
	LCD_RD_LOW() ;//�ߵ�ƽ��ʾд���͵�ƽ��ʾ��
	ILI9341_DELAY();
	
	data = LCD_DATA_IN();	
	ILI9341_DELAY();
	
	LCD_RD_HIGH() ;//�ߵ�ƽ��ʾд���͵�ƽ��ʾ��
	ILI9341_DELAY();
	
	LCD_CS_HIGH();

	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = ILI9341_DATA_PIN;
	GPIO_Init ( ILI9341_DATA_PORT, &GPIO_InitStructure );
	
	return data;
}


uint16_t ILI9341_Read_ID(void)
{
	uint16_t temp=0;
	
	ILI9341_Write_CMD(0xD3);
	
	ILI9341_Read_Date();
	
	ILI9341_Read_Date();

	temp = ILI9341_Read_Date();
	
	temp<<=8;
	
	temp |= ILI9341_Read_Date();

  return temp;
	
}

static void ILI9341_REG_Config ( void )
{	
	/*  Power control B (CFh)  */
	DEBUG_DELAY  ();
	ILI9341_Write_CMD ( 0xCF  );
	ILI9341_Write_Date ( 0x00  );
	ILI9341_Write_Date ( 0x81  );
	ILI9341_Write_Date ( 0x30  );
	
	/*  Power on sequence control (EDh) */
	DEBUG_DELAY ();
	ILI9341_Write_CMD ( 0xED );
	ILI9341_Write_Date ( 0x64 );
	ILI9341_Write_Date ( 0x03 );
	ILI9341_Write_Date ( 0x12 );
	ILI9341_Write_Date ( 0x81 );
	
	/*  Driver timing control A (E8h) */
	DEBUG_DELAY ();
	ILI9341_Write_CMD ( 0xE8 );
	ILI9341_Write_Date ( 0x85 );
	ILI9341_Write_Date ( 0x10 );
	ILI9341_Write_Date ( 0x78 );
	
	/*  Power control A (CBh) */
	DEBUG_DELAY ();
	ILI9341_Write_CMD ( 0xCB );
	ILI9341_Write_Date ( 0x39 );
	ILI9341_Write_Date ( 0x2C );
	ILI9341_Write_Date ( 0x00 );
	ILI9341_Write_Date ( 0x34 );
	ILI9341_Write_Date ( 0x02 );
	
	/* Pump ratio control (F7h) */
	DEBUG_DELAY ();
	ILI9341_Write_CMD ( 0xF7 );
	ILI9341_Write_Date ( 0x20 );
	
	/* Driver timing control B */
	DEBUG_DELAY ();
	ILI9341_Write_CMD ( 0xEA );
	ILI9341_Write_Date ( 0x00 );
	ILI9341_Write_Date ( 0x00 );
	
	/* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
	DEBUG_DELAY ();
	ILI9341_Write_CMD ( 0xB1 );
	ILI9341_Write_Date ( 0x00 );
	ILI9341_Write_Date ( 0x1B );
	
	/*  Display Function Control (B6h) */
	DEBUG_DELAY ();
	ILI9341_Write_CMD ( 0xB6 );
	ILI9341_Write_Date ( 0x0A );
	ILI9341_Write_Date ( 0xA2 );
	
	/* Power Control 1 (C0h) */
	DEBUG_DELAY ();
	ILI9341_Write_CMD ( 0xC0 );
	ILI9341_Write_Date ( 0x35 );
	
	/* Power Control 2 (C1h) */
	DEBUG_DELAY ();
	ILI9341_Write_CMD ( 0xC1 );
	ILI9341_Write_Date ( 0x11 );
	
	/* VCOM Control 1 (C5h) */
	ILI9341_Write_CMD ( 0xC5 );
	ILI9341_Write_Date ( 0x45 );
	ILI9341_Write_Date ( 0x45 );
	
	/*  VCOM Control 2 (C7h)  */
	ILI9341_Write_CMD ( 0xC7 );
	ILI9341_Write_Date ( 0xA2 );
	
	/* Enable 3G (F2h) */
	ILI9341_Write_CMD ( 0xF2 );
	ILI9341_Write_Date ( 0x00 );
	
	/* Gamma Set (26h) */
	ILI9341_Write_CMD ( 0x26 );
	ILI9341_Write_Date ( 0x01 );
	DEBUG_DELAY ();
	
	/* Positive Gamma Correction */
	ILI9341_Write_CMD ( 0xE0 ); //Set Gamma
	ILI9341_Write_Date ( 0x0F );
	ILI9341_Write_Date ( 0x26 );
	ILI9341_Write_Date ( 0x24 );
	ILI9341_Write_Date ( 0x0B );
	ILI9341_Write_Date ( 0x0E );
	ILI9341_Write_Date ( 0x09 );
	ILI9341_Write_Date ( 0x54 );
	ILI9341_Write_Date ( 0xA8 );
	ILI9341_Write_Date ( 0x46 );
	ILI9341_Write_Date ( 0x0C );
	ILI9341_Write_Date ( 0x17 );
	ILI9341_Write_Date ( 0x09 );
	ILI9341_Write_Date ( 0x0F );
	ILI9341_Write_Date ( 0x07 );
	ILI9341_Write_Date ( 0x00 );
	
	/* Negative Gamma Correction (E1h) */
	ILI9341_Write_CMD ( 0XE1 ); //Set Gamma
	ILI9341_Write_Date ( 0x00 );
	ILI9341_Write_Date ( 0x19 );
	ILI9341_Write_Date ( 0x1B );
	ILI9341_Write_Date ( 0x04 );
	ILI9341_Write_Date ( 0x10 );
	ILI9341_Write_Date ( 0x07 );
	ILI9341_Write_Date ( 0x2A );
	ILI9341_Write_Date ( 0x47 );
	ILI9341_Write_Date ( 0x39 );
	ILI9341_Write_Date ( 0x03 );
	ILI9341_Write_Date ( 0x06 );
	ILI9341_Write_Date ( 0x06 );
	ILI9341_Write_Date ( 0x30 );
	ILI9341_Write_Date ( 0x38 );
	ILI9341_Write_Date ( 0x0F );
	
	/* memory access control set */
	DEBUG_DELAY ();
	ILI9341_Write_CMD ( 0x36 ); 	
	ILI9341_Write_Date ( 0xC8 );    /*����  ���Ͻǵ� (���)�����½� (�յ�)ɨ�跽ʽ*/
	DEBUG_DELAY ();
	
	/* column address control set */
	ILI9341_Write_CMD ( CMD_SetCoordinateX ); 
	ILI9341_Write_Date ( 0x00 );
	ILI9341_Write_Date ( 0x00 );
	ILI9341_Write_Date ( 0x00 );
	ILI9341_Write_Date ( 0xEF );
	
	/* page address control set */
	DEBUG_DELAY ();
	ILI9341_Write_CMD ( CMD_SetCoordinateY ); 
	ILI9341_Write_Date ( 0x00 );
	ILI9341_Write_Date ( 0x00 );
	ILI9341_Write_Date ( 0x01 );
	ILI9341_Write_Date ( 0x3F );
	
	/*  Pixel Format Set (3Ah)  */
	DEBUG_DELAY ();
	ILI9341_Write_CMD ( 0x3a ); 
	ILI9341_Write_Date ( 0x55 );
	
	/* Sleep Out (11h)  */
	ILI9341_Write_CMD ( 0x11 );	
	ILI9341_Delay ( 0xAFFf<<2 );
	DEBUG_DELAY ();
	
	/* Display ON (29h) */
	ILI9341_Write_CMD ( 0x29 ); 
		
}


/**
 * @brief  ����ILI9341��GRAM��ɨ�跽�� 
 * @param  ucOption ��ѡ��GRAM��ɨ�跽�� 
 *     @arg 0-7 :������ѡֵΪ0-7��˸�����
 *
 *	����������0��3��5��6 ģʽ�ʺϴ���������ʾ���֣�
 *				���Ƽ�ʹ������ģʽ��ʾ����	����ģʽ��ʾ���ֻ��о���Ч��			
 *		
 *	����0��2��4��6 ģʽ��X��������Ϊ240��Y��������Ϊ320
 *	����1��3��5��7 ģʽ��X��������Ϊ320��Y��������Ϊ240
 *
 *	���� 6 ģʽΪ�󲿷�Һ�����̵�Ĭ����ʾ����
 *	���� 3 ģʽΪ����ͷ����ʹ�õķ���
 *	���� 0 ģʽΪBMPͼƬ��ʾ����ʹ�õķ���
 *
 * @retval ��
 * @note  ����ͼ����A��ʾ���ϣ�V��ʾ���£�<��ʾ����>��ʾ����
					X��ʾX�ᣬY��ʾY��

------------------------------------------------------------
ģʽ0��				.		ģʽ1��		.	ģʽ2��			.	ģʽ3��					
					A		.					A		.		A					.		A									
					|		.					|		.		|					.		|							
					Y		.					X		.		Y					.		X					
					0		.					1		.		2					.		3					
	<--- X0 o		.	<----Y1	o		.		o 2X--->  .		o 3Y--->	
------------------------------------------------------------	
ģʽ4��				.	ģʽ5��			.	ģʽ6��			.	ģʽ7��					
	<--- X4 o		.	<--- Y5 o		.		o 6X--->  .		o 7Y--->	
					4		.					5		.		6					.		7	
					Y		.					X		.		Y					.		X						
					|		.					|		.		|					.		|							
					V		.					V		.		V					.		V		
---------------------------------------------------------				
											 LCD��ʾ��
								|-----------------|
								|			����Logo		|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|-----------------|
								��Ļ���棨��240����320��

 *******************************************************/
void ILI9341_GramScan ( uint8_t ucOption )
{	
	//������飬ֻ������0-7
	if(ucOption >7 )
		return;
	
	//����ģʽ����LCD_SCAN_MODE��ֵ����Ҫ���ڴ�����ѡ��������
	LCD_SCAN_MODE = ucOption;
	
	//����ģʽ����XY��������ؿ���
	if(ucOption%2 == 0)	
	{
		//0 2 4 6ģʽ��X�������ؿ���Ϊ240��Y����Ϊ320
		LCD_X_LENGTH = ILI9341_LESS_PIXEL;
		LCD_Y_LENGTH =	ILI9341_MORE_PIXEL;
	}
	else				
	{
		//1 3 5 7ģʽ��X�������ؿ���Ϊ320��Y����Ϊ240
		LCD_X_LENGTH = ILI9341_MORE_PIXEL;
		LCD_Y_LENGTH =	ILI9341_LESS_PIXEL; 
	}

	//0x36��������ĸ�3λ����������GRAMɨ�跽��	
	ILI9341_Write_CMD ( 0x36 ); 
	ILI9341_Write_Date ( 0x08 |(ucOption<<5));//����ucOption��ֵ����LCD��������0-7��ģʽ
	ILI9341_Write_CMD ( CMD_SetCoordinateX ); 
	ILI9341_Write_Date ( 0x00 );		/* x ��ʼ�����8λ */
	ILI9341_Write_Date ( 0x00 );		/* x ��ʼ�����8λ */
	ILI9341_Write_Date ( ((LCD_X_LENGTH-1)>>8)&0xFF ); /* x ���������8λ */	
	ILI9341_Write_Date ( (LCD_X_LENGTH-1)&0xFF );				/* x ���������8λ */

	ILI9341_Write_CMD ( CMD_SetCoordinateY ); 
	ILI9341_Write_Date ( 0x00 );		/* y ��ʼ�����8λ */
	ILI9341_Write_Date ( 0x00 );		/* y ��ʼ�����8λ */
	ILI9341_Write_Date ( ((LCD_Y_LENGTH-1)>>8)&0xFF );	/* y ���������8λ */	 
	ILI9341_Write_Date ( (LCD_Y_LENGTH-1)&0xFF );				/* y ���������8λ */

	/* write gram start */
	ILI9341_Write_CMD ( CMD_SetPixel );	
}


void ILI9341_Init(void)
{
	ILI9341_GPIO_Config();
	ILI9341_REG_Config();
  ILI9341_GramScan(6);//������ʾ���ַ���ģʽ
	ILI9341_Draw_Rect(0,0,ILI9341_LESS_PIXEL,ILI9341_MORE_PIXEL,BLACK);
}

//����Ҫ��ʾ�ľ�������
void ILI9341_Open_Window(uint16_t x,uint16_t y,uint16_t width,uint16_t heigh)
{
	ILI9341_Write_CMD(CMD_SetCoordinateX);
	ILI9341_Write_Date((x&0xFF00) >>8);//sc��8λ
	ILI9341_Write_Date(x&0xFF);//sc��8λ
	ILI9341_Write_Date(((x+width-1)&0xFF00) >>8);//ec��8λ
	ILI9341_Write_Date((x+width-1)&0xFF);//ec��8λ
	
	
	ILI9341_Write_CMD(CMD_SetCoordinateY);
	ILI9341_Write_Date((y&0xFF00) >>8);//sp��8λ
	ILI9341_Write_Date(y&0xFF);//sp��8λ
	ILI9341_Write_Date(((y+heigh-1)&0xFF00) >>8);//ep��8λ
	ILI9341_Write_Date((y+heigh-1)&0xFF);//ep��8λ
	
	{
		uint32_t i;
		ILI9341_Write_CMD(CMD_SetPixel);
		for(i=0;i<width*heigh;i++)
		{
			ILI9341_Write_Date(BLACK);
		}
  }
}

//������
void ILI9341_Draw_Rect(uint16_t x,uint16_t y,uint16_t width,uint16_t heigh,uint16_t color)
{
	uint32_t i;
	
	ILI9341_Open_Window(x,y,width,heigh);
	
	ILI9341_Write_CMD(CMD_SetPixel);
	for(i=0;i<width*heigh;i++)
	{
		ILI9341_Write_Date(color);
	}
}

//����
void ILI9341_Draw_Point(uint16_t x,uint16_t y,uint16_t color)
{
	ILI9341_Open_Window(x,y,1,1);
	
  ILI9341_Write_CMD(CMD_SetPixel);
	ILI9341_Write_Date(color);
}




