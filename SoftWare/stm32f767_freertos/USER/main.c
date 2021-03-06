#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/************************************************
 ALIENTEK 阿波罗STM32F7开发板 实验1
 跑马灯实验-HAL库版本
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define LED0_TASK_PRIO		2
//任务堆栈大小	
#define LED0_STK_SIZE 		128  
//任务句柄
TaskHandle_t LED0Task_Handler;
//任务函数
void led0_task(void *pvParameters);

//任务优先级
#define LED1_TASK_PRIO		3
//任务堆栈大小	
#define LED1_STK_SIZE 		128  
//任务句柄
TaskHandle_t LED1Task_Handler;
//任务函数
void led1_task(void *pvParameters);

QueueHandle_t xQueue_Uart1 = NULL;

int main(void)
{
    Cache_Enable();                 //打开L1-Cache
    HAL_Init();				        //初始化HAL库
    Stm32_Clock_Init(432,16,2,9);   //设置时钟,216Mhz 
    delay_init(216);                //延时初始化
		uart_init(115200);		        //串口初始化
		printf("stm32f767-------\r\n");
    LED_Init();                     //初始化LED
	
		vTraceEnable(TRC_INIT);
	
		xQueue_Uart1 = xQueueCreate(500, sizeof(uint8_t));
		if( xQueue_Uart1 == 0 )
		{
		}	
    //创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}

//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
    //创建LED0任务
    xTaskCreate((TaskFunction_t )led0_task,     	
                (const char*    )"led0_task",   	
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&LED0Task_Handler);   
    //创建LED1任务
    xTaskCreate((TaskFunction_t )led1_task,     
                (const char*    )"led1_task",   
                (uint16_t       )LED1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t*  )&LED1Task_Handler);        
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

//LED0任务函数 
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

//LED1任务函数
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

