/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: FreeRTOS的串口调试方法（打印任务执行情况）
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F1Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "led/bsp_led.h"
#include "usart/bsp_debug_usart.h"
#include "key/bsp_key.h"
#include "BasicTIM/bsp_BasicTIM.h" 

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
static TaskHandle_t xHandleTaskUserIF = NULL;
static TaskHandle_t xHandleTaskLED1 = NULL;
static TaskHandle_t xHandleTaskLED2 = NULL;
static TaskHandle_t xHandleTaskLED3 = NULL;
static TaskHandle_t xHandleTaskLED4 = NULL;

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
void SystemClock_Config(void);
static void vTaskTaskUserIF(void *pvParameters);
static void vTaskLED1(void *pvParameters);
static void vTaskLED2(void *pvParameters);
static void vTaskLED3(void *pvParameters);
static void AppTaskCreate (void);

/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 主函数.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
int main(void)
{

  HAL_Init();

  /* 初始化系统时钟 */
  SystemClock_Config();
  
  MX_DEBUG_USART_Init();
  /* 初始化LED */
  LED_GPIO_Init();
  /* 板子按键初始化 */
  KEY_GPIO_Init();
  /* 基本定时器初始化：100us中断一次 */
  BASIC_TIMx_Init();

	/* 创建任务 */
	AppTaskCreate();
  /* 启动调度，开始执行任务 */
  vTaskStartScheduler();
 
  while (1)
  {
  }
}

/**
  * 函数功能: 系统时钟
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/**
  * 函数功能: 接口消息处理
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
static void vTaskTaskUserIF(void *pvParameters)
{
	uint8_t pcWriteBuffer[500];
    while(1)
    {
       if(KEY1_StateRead()==KEY_DOWN)
       {
					printf("=================================================\r\n");
					printf("TaskName      Status Priority   RemainThreadSize TaskNumber\r\n");
					vTaskList((char *)&pcWriteBuffer);
					printf("%s\r\n", pcWriteBuffer);
         
					printf("\r\nTaskName       Counter         Usage\r\n");
					vTaskGetRunTimeStats((char *)&pcWriteBuffer);
					printf("%s\r\n", pcWriteBuffer);
       }
       vTaskDelay(20);
		}
		
}

/**
  * 函数功能: LED1任务
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
static void vTaskLED1(void *pvParameters)
{
    while(1)
    {
      LED1_TOGGLE;
      vTaskDelay(100);
	  }
}

/**
  * 函数功能: LED2任务
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
static void vTaskLED2(void *pvParameters)
{
    while(1)
    {
      LED2_TOGGLE;
      vTaskDelay(100);
    }
}

/**
  * 函数功能: LED3任务
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
static void vTaskLED3(void *pvParameters)
{
    while(1)
    {
      LED3_TOGGLE;
      vTaskDelay(1000);
    }
}

static void vTaskLED4(void *pvParameters)
{
	while(1)
	{
		LED4_TOGGLE;
		vTaskDelay(1000);
	}
	
}

/**
  * 函数功能: 创建任务应用
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
static void AppTaskCreate (void)
{

    xTaskCreate( vTaskTaskUserIF,   	/* 任务函数  */
                 "vTaskUserIF",     	/* 任务名    */
                 512,               	/* 任务栈大小，单位word，也就是4字节 */
                 NULL,              	/* 任务参数  */
                 1,                 	/* 任务优先级*/
                 &xHandleTaskUserIF );  /* 任务句柄  */
	
    xTaskCreate( vTaskLED1,   	      /* 任务函数  */
                 "vTaskLED1",     	  /* 任务名    */
                 512,               	/* 任务栈大小，单位word，也就是4字节 */
                 NULL,              	/* 任务参数  */
                 2,                 	/* 任务优先级*/
                 &xHandleTaskLED1 );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskLED2,    		      /* 任务函数  */
                 "vTaskLED2",  		    /* 任务名    */
                 512,         		    /* 任务栈大小，单位word，也就是4字节 */
                 NULL,        		    /* 任务参数  */
                 3,           		    /* 任务优先级*/
                 &xHandleTaskLED2 );  /* 任务句柄  */
	
	xTaskCreate( vTaskLED3,     		    /* 任务函数  */
                 "vTaskLED3",   		  /* 任务名    */
                 512,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		  /* 任务参数  */
                 4,               		/* 任务优先级*/
                 &xHandleTaskLED3 );  /* 任务句柄  */
								 
	xTaskCreate( vTaskLED4,     		    /* 任务函数  */
                 "vTaskLED4",   		  /* 任务名    */
                 512,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		  /* 任务参数  */
                 4,               		/* 任务优先级*/
                 &xHandleTaskLED4 );  /* 任务句柄  */
	
}

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/

