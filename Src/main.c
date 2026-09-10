/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "mpu6050.h"
#include "IIC.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

#include <string.h>
#include "stdlib.h" 
int MAX_LEN=7;
int LEN=7;
int kkk;
int count=0;
extern int jug_yaw[3];
float Data[9];
int jug=500;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//int fputc(int ch, FILE *stream)
//{
//	HAL_UART_Transmit (&huart1, (unsigned char *) &ch, 1, 1000 );
//	return 1;
//}

//HAL_UART_Transmit (&huart1, (unsigned char *) &ch, 1, 1000 );

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  float pitch[3]={0},roll[3]={0},yaw[3]={0}; 		    //欧拉角
  short gyrox[3],gyroy[3],gyroz[3];		//陀螺仪原始数据
  float pitch_last[3]={0},roll_last[3]={0},yaw_last[3]={0}; 		    //欧拉角
	float pitch_new[3]={0},roll_new[3]={0},yaw_new[3]={0}; 		    //欧拉角
	float pitch_more_new[3]={0},roll_more_new[3]={0},yaw_more_new[3]={0}; 		    //欧拉角
	float pitch_init[3];
	float roll_init[3];
	float yaw_init[3];
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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
	MPU_Init(&IIC1);
	mpu_dmp_init(&IIC1);		//dmp初始化
	MPU_Init(&IIC2);
	mpu_dmp_init(&IIC2);		//dmp初始化
	MPU_Init(&IIC3);
	mpu_dmp_init(&IIC3);		//dmp初始化
	
	for (int i = 0; i < 3; i++){MPU6050[i].total_pitch=0;}
	for (int i = 0; i < 3; i++){MPU6050[i].total_yaw=0;}	
	for (int i = 0; i < 3; i++){MPU6050[i].total_roll=0;}

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		count++;
		MPU_Get_Gyroscope(&IIC1,&gyrox[0],&gyroy[0],&gyroz[0]);
		while(mpu_dmp_get_data(&IIC1,&pitch[0], &roll[0], &yaw[0]));	//必须要用while等待，才能读取成功

		MPU_Get_Gyroscope( &IIC2,&gyrox[1],&gyroy[1],&gyroz[1]);
		while(mpu_dmp_get_data(&IIC2,&pitch[1], &roll[1], &yaw[1]));	//必须要用while等待，才能读取成功

		MPU_Get_Gyroscope(&IIC3,&gyrox[2],&gyroy[2],&gyroz[2]);
		while(mpu_dmp_get_data(&IIC3,&pitch[2], &roll[2], &yaw[2]));	//必须要用while等待，才能读取成功
		
		for(int i=0;i<3;i++)
		{
			MPU6050[i].pitch=pitch[i];
			MPU6050[i].roll=roll[i];
			MPU6050[i].yaw=yaw[i];
			get_total_angle_MPU6050(&MPU6050[i],i);			
		}	
//			if(jug_yaw[2]>=500)
//	{
//		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // LED亮
//	}
		char str[LEN*9];
		char str1[LEN];
		char str2[LEN];
		char str3[LEN];
		char str4[LEN];
		char str5[LEN];
		char str6[LEN];
		char str7[LEN];
		char str8[LEN];
		char str9[LEN];

 		sprintf(str1, "%.2f", pitch[0]);
		sprintf(str2, "%.2f", roll[0]);
		sprintf(str3, "%.2f", yaw[0]);
		sprintf(str4, "%.2f", pitch[1]);
		sprintf(str5, "%.2f", roll[1]);
		sprintf(str6, "%.2f", yaw[1]);
		sprintf(str7, "%.2f", pitch[2]);
		sprintf(str8, "%.2f", roll[2]);
		sprintf(str9, "%.2f", yaw[2]);
		char string1[MAX_LEN];  
		char string2[MAX_LEN]; 
		char string3[MAX_LEN]; 
		char string4[MAX_LEN];  
		char string5[MAX_LEN]; 
		char string6[MAX_LEN]; 
		char string7[MAX_LEN];  
		char string8[MAX_LEN]; 
		char string9[MAX_LEN]; 

    char *input1 = str1;  // 输入的字符串
		char *input2 = str2;  // 输入的字符串
		char *input3 = str3;  // 输入的字符串
    char *input4 = str4;  // 输入的字符串
		char *input5 = str5;  // 输入的字符串
		char *input6 = str6;  // 输入的字符串
    char *input7 = str7;  // 输入的字符串
		char *input8 = str8;  // 输入的字符串
		char *input9 = str9;  // 输入的字符串
		
    int len1 = strlen(input1);
		int len2 = strlen(input2);
		int len3 = strlen(input3);
    int len4 = strlen(input4);
		int len5 = strlen(input5);
		int len6 = strlen(input6);
    int len7 = strlen(input7);
		int len8 = strlen(input8);
		int len9 = strlen(input9);
		
    // 复制输入的字符串到str
    for (int i = 0; i < len1; i++) 
		{
        string1[i] = input1[i];
    }
    for (int i = 0; i < len2; i++) 
		{
        string2[i] = input2[i];
    }		
    for (int i = 0; i < len3; i++) 
		{
        string3[i] = input3[i];
    }	
    for (int i = 0; i < len4; i++) 
		{
        string4[i] = input4[i];
    }
    for (int i = 0; i < len5; i++) 
		{
        string5[i] = input5[i];
    }		
    for (int i = 0; i < len6; i++) 
		{
        string6[i] = input6[i];
    }	
    for (int i = 0; i < len7; i++) 
		{   
        string7[i] = input7[i];
    }
    for (int i = 0; i < len8; i++) 
		{
        string8[i] = input8[i];
    }		
    for (int i = 0; i < len9; i++) 
		{
        string9[i] = input9[i];
    }	
		
    // 在剩余的位置补0
    for (int i; i < MAX_LEN; i++) {
        string1[i] = '0';
				string2[i] = '0';
				string3[i] = '0';
        string4[i] = '0';
				string5[i] = '0';
				string6[i] = '0';
        string7[i] = '0';
				string8[i] = '0';
				string9[i] = '0';
    }
		Data[0]=atof(string1);
		
    sprintf(str,"%s%s%s%s%s%s%s%s%s",string1,string2,string3,string4,string5, string6,string7,string8, string9);
		HAL_UART_Transmit (&huart1,(const uint8_t *)str,LEN*9,10000);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
