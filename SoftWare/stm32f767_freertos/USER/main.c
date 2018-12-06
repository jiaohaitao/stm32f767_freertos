#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

/************************************************
 ALIENTEK ������STM32F7������ ʵ��1
 �����ʵ��-HAL��汾
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/


int main(void)
{
    Cache_Enable();                 //��L1-Cache
    HAL_Init();				        			//��ʼ��HAL��
    Stm32_Clock_Init(432,25,2,9);   //����ʱ��,216Mhz 
    delay_init(216);                //��ʱ��ʼ��
		uart_init(115200);		        	//���ڳ�ʼ��
    LED_Init();                     //��ʼ��LED
    while(1)
    {
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET); 
        delay_ms(500);										
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);   
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET); 
        delay_ms(500);                                      
    }
}


/*ʹ�üĴ�������ʵ�������
int main(void)
{ 
    Cache_Enable();                 //��L1-Cache
    HAL_Init();				        //��ʼ��HAL��
    Stm32_Clock_Init(432,25,2,9);   //����ʱ��,216Mhz 
    delay_init(216);                //��ʱ��ʼ��
	uart_init(115200);		        //���ڳ�ʼ��
    LED_Init();      
	while(1)
	{
      GPIOB->BSRR=GPIO_PIN_1;      //LED0��
	  GPIOB->BSRR=GPIO_PIN_0<<16;  //LED1��
	  delay_ms(500);               //��ʱ500ms
      GPIOB->BSRR=GPIO_PIN_1<<16;  //LED0��
	  GPIOB->BSRR=GPIO_PIN_0;      //LED1��
	  delay_ms(500);               //��ʱ500ms
	 }
 }	

*/


