#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/************************************************
 ALIENTEK ������STM32F7������ ʵ��1
 �����ʵ��-HAL��汾
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		128  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define LED0_TASK_PRIO		2
//�����ջ��С	
#define LED0_STK_SIZE 		128  
//������
TaskHandle_t LED0Task_Handler;
//������
void led0_task(void *pvParameters);

//�������ȼ�
#define LED1_TASK_PRIO		3
//�����ջ��С	
#define LED1_STK_SIZE 		128  
//������
TaskHandle_t LED1Task_Handler;
//������
void led1_task(void *pvParameters);

QueueHandle_t xQueue_Uart1 = NULL;

int main(void)
{
    Cache_Enable();                 //��L1-Cache
    HAL_Init();				        //��ʼ��HAL��
    Stm32_Clock_Init(432,16,2,9);   //����ʱ��,216Mhz 
    delay_init(216);                //��ʱ��ʼ��
		uart_init(115200);		        //���ڳ�ʼ��
		printf("stm32f767-------\r\n");
    LED_Init();                     //��ʼ��LED
	
		vTraceEnable(TRC_INIT);
	
		xQueue_Uart1 = xQueueCreate(500, sizeof(uint8_t));
		if( xQueue_Uart1 == 0 )
		{
		}	
    //������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}

//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
    //����LED0����
    xTaskCreate((TaskFunction_t )led0_task,     	
                (const char*    )"led0_task",   	
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&LED0Task_Handler);   
    //����LED1����
    xTaskCreate((TaskFunction_t )led1_task,     
                (const char*    )"led1_task",   
                (uint16_t       )LED1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t*  )&LED1Task_Handler);        
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

//LED0������ 
void led0_task(void *pvParameters)
{
    while(1)
    {
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);   	
				vTaskDelay(500);				
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET); 
        vTaskDelay(500);		
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
				vTaskDelay(500);				
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
				vTaskDelay(500);				
    }
}   

//LED1������
void led1_task(void *pvParameters)
{
	traceString Task1_uechannel;
	unsigned int cnt=0;
	BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(1000); 
	uint8_t ucQueueMsgValue;
	Task1_uechannel = xTraceRegisterString("Task1_log");
	
	while(1)
	{
		xResult = xQueueReceive(xQueue_Uart1,
		(void *)&ucQueueMsgValue,
		(TickType_t)xMaxBlockTime);
		if(xResult == pdPASS)
		{
				vTracePrintF(Task1_uechannel,	"rx%x:", ucQueueMsgValue);		
		}
		else
		{
		}
	}
}

