/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "taskQueue.h"
#include "math.h"
#include "stdlib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
static const uint16_t MAX_SIZE = 16;
static struct taskQueue main_que;
static struct taskQueue delayed_que;

volatile uint16_t ticks = 0;
volatile char timerFlag = 0;

char h[2];
float temp;
int thr=55555;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
uint8_t hexToAscii(uint8_t n)//4-bit hex value converted to an ascii character
{
 if (n>=0 && n<=9) n = n + '0';
 else n = n - 10 + 'A';
 return n;
}	
float concat(uint8_t a, uint8_t b){
    float c;
    c = (float)b;
    while( c > 1.0f ) c *= 0.1f; //moving the decimal point (.) to left most
    c = (float)a + c;
    return c; 
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Enqueue task in the main queue
static void QueTask(funcPtr f, uint16_t priority)
{
    enqueue(&main_que, f, priority, 0);
}

// dispatch a task and run it if available, otherwise display 'IDLE'

static void dispatch()
{
    // handling the tasks in the delayed Queue first
    decrementDelay(&delayed_que, ticks);
    // enqueue the ready ones to the main Queue
    pushReady(&delayed_que, &main_que);
    // new dispatch --> ticks = 0 for the rest of the tasks
    ticks = 0;
    // dispatch tasks if available
    if (main_que.currentSize > 0)
        dequeue(&main_que).f();
    else
        //printf("IDLE"); // for test purposes
		HAL_UART_Transmit(&huart1 , "IDLE\r\n" , sizeof("IDLE\r\n") , HAL_MAX_DELAY ) ;
}

static void ReRunMe(funcPtr f, uint16_t priority, uint16_t delay)
{
    //enqueue to the main queue if the task is not delayed, otherwise enqueue in the delayed Queue
    if (delay == 0)
        enqueue(&main_que, f, priority, delay);
    else
        enqueue(&delayed_que, f, priority, delay);
}

// initialize Hardware peripherals and Queues
static void init()
{
    // initialize Hardware
    // hardware_init();

    // initialize queues
    que_init(&main_que, MAX_SIZE);
    que_init(&delayed_que, MAX_SIZE);
}

//example tasks
static void ReadThreshold()
{
		int oper1; 
		char c = '\n';
		int oper2; 
	HAL_UART_Transmit(&huart1,"READTH\r\n", sizeof("READTH\r\n"), 10);

		HAL_UART_Receive(&huart1,(uint8_t *) &h[0], 1, HAL_MAX_DELAY);	
		HAL_Delay(100);
		HAL_UART_Receive(&huart1,(uint8_t *) &h[1], 1, HAL_MAX_DELAY);	
		//HAL_Delay(20);
		//HAL_UART_Transmit(&huart1,(uint8_t *) &h[0], 1, 10);	
		//HAL_UART_Transmit(&huart1,(uint8_t *) &h[1],1, 10);	
		//	if (h[2] == '\r')
		//	{
				//HAL_UART_Transmit(&huart1,(uint8_t *) &c, 1, 10);
				oper1 = h[0] - '0';
				oper2 = h[1] - '0';
				thr = oper1 * pow(10, (int)log10(oper2)+1) + oper2;
				HAL_Delay(100);

		//}
		//ReRunMe(&ReadThreshold, 1, 1);

}
static void BlinkingLED()
{
   	HAL_UART_Transmit(&huart1,"LED\r\n", sizeof("LED\r\n"), 10);

		for (int i = 1; i<=50;i++) // indicator of ready device
		{
		 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
		}

}
static void GettingTemperature()
{
  HAL_UART_Transmit(&huart1,"GETTM\r\n", sizeof("GETTM\r\n"), 10);
  
  uint8_t z;
	uint8_t f;
	float temp;
	uint8_t out[] = {0,0,'.',0,0,'\r','\n'};
	
if (HAL_I2C_IsDeviceReady(&hi2c1, 0xD0, 10, 10) == HAL_OK)
 {
		 for (int i = 1; i<=10;i++) // indicator of ready device
		 {
		 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
		 HAL_Delay(250);
 }
 }
	 //Transmit via I2C to set clock
	 uint8_t LSB [2], MSB [2];
	 //control reg and status reg
	 uint8_t EOSC [2];
	 EOSC[0]= 0x0E;
	 EOSC[1]= 0x3C;
	 HAL_I2C_Master_Transmit(&hi2c1, 0xD0, EOSC, 2, 10);

	 uint8_t OSF [2];
	 OSF[0]= 0x0F;
	 OSF[1]= 0x88;
	 HAL_I2C_Master_Transmit(&hi2c1, 0xD0, OSF, 2, 10);
	 LSB[0] = 0x11; //register address
   HAL_I2C_Master_Transmit(&hi2c1, 0xD0, LSB, 1, 10);
 	 HAL_I2C_Master_Receive(&hi2c1, 0xD1, LSB+1, 1, 10);
	 out[0] = hexToAscii(LSB[1] >> 4 );
	 out[1] = hexToAscii(LSB[1] & 0x0F );

   MSB[0] = 0x12; //register address
   HAL_I2C_Master_Transmit(&hi2c1, 0xD0, MSB, 2, 10);
 	 HAL_I2C_Master_Receive(&hi2c1, 0xD1, LSB+1, 1, 10);
	 out[3] = hexToAscii(((MSB[1]>>6)/4)>> 4);
	 out[4] = hexToAscii(((MSB[1]>>6)/4) & 0x0F );
	
   // transmit Data to UART
	 HAL_UART_Transmit(&huart1,out, sizeof(out), 10);
	 int num = atoi((const char*) out);

	 //z = out[3] * pow(10, (int)log10(out[4])+1) + out[4];
	 //f = out[0] * pow(10, (int)log10(out[1])+1) + out[1];
	 //temp= concat(z, f);
	 
		if(num >thr)
		{
			QueTask(&BlinkingLED,1);
		}

	ReRunMe(&GettingTemperature, 1, 100);
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	init();
	QueTask(&ReadThreshold, 1);
	QueTask(&GettingTemperature, 2);
	//QueTask(&BlinkingLED, 3);

	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		 if (timerFlag)
        {
            dispatch();
            timerFlag = 0;
        }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00000E14;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB3 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
