/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include <stdlib.h>
#include "lcd.h"
#include <ctype.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
FATFS fs;
FIL fil;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
 * @brief Custom implementation of _write function for printf redirection.
 * @param file: File descriptor (not used)
 * @param ptr: Pointer to the data buffer
 * @param len: Length of the data buffer
 * @retval Number of bytes transmitted
 */
int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
	return len;
}

/**
 * @brief Function to transmit a string over UART2.
 * @param string: Pointer to the string to be transmitted
 */

void transmit_uart(char *string){
  	uint8_t len = strlen(string);
  	HAL_UART_Transmit(&huart2, (uint8_t*) string ,len , 200);
  }

/**
 * @brief Function to remove non-alphabetic characters from a string.
 * @param str: Input string
 * @retval Pointer to the new string with only alphabetic characters
 */
char* removeNonAlphabetChars(const char *str) {
    int length = strlen(str);
    char* newString = (char*)malloc(length + 1);  // Allocate memory for the new string

    if (newString == NULL) {
        return NULL;  // Memory allocation failed
    }

    int newLength = 0;  // Length of the new string

    for (int i = 0; i < length; i++) {
        if (isalpha((uint32_t)str[i])) {
            newString[newLength] = str[i];
            newLength++;
        }
    }

    newString[newLength] = '\0';  // Null-terminate the new string
    return newString;
}

/**
 * @brief Function to read a character from a 4x4 keypad.
 * @retval The character read from the keypad
 */
char read_from_keypad()
{
    char character_defined ;
    // Scan the first row
	HAL_GPIO_WritePin (ROW1_GPIO_Port, ROW1_Pin, GPIO_PIN_RESET);  //Pull the R1 low
	HAL_GPIO_WritePin (ROW2_GPIO_Port, ROW2_Pin, GPIO_PIN_SET);  // Pull the R2 High
	HAL_GPIO_WritePin (ROW3_GPIO_Port, ROW3_Pin, GPIO_PIN_SET);  // Pull the R3 High
	HAL_GPIO_WritePin (ROW4_GPIO_Port, ROW4_Pin, GPIO_PIN_SET);  // Pull the R4 High
	if (!(HAL_GPIO_ReadPin (COLUMN1_GPIO_Port, COLUMN1_Pin)))   // if the Col 1 is low
	{
		while (!(HAL_GPIO_ReadPin (COLUMN1_GPIO_Port, COLUMN1_Pin)));   // wait till the button is pressed
		character_defined = '1';
	}

	if (!(HAL_GPIO_ReadPin (COLUMN2_GPIO_Port, COLUMN2_Pin)))   // if the Col 2 is low
	{
		while (!(HAL_GPIO_ReadPin (COLUMN2_GPIO_Port, COLUMN2_Pin)));   // wait till the button is pressed
		character_defined = '2';
	}

	if (!(HAL_GPIO_ReadPin (COLUMN3_GPIO_Port, COLUMN3_Pin)))   // if the Col 3 is low
	{
		while (!(HAL_GPIO_ReadPin (COLUMN3_GPIO_Port, COLUMN3_Pin)));   // wait till the button is pressed
		character_defined = '3';
	}

	if (!(HAL_GPIO_ReadPin (COLUMN4_GPIO_Port, COLUMN4_Pin)))   // if the Col 4 is low
	{
		while (!(HAL_GPIO_ReadPin (COLUMN4_GPIO_Port, COLUMN4_Pin)));   // wait till the button is pressed
		character_defined = 'A';
	}
	 // Scan the second row
	HAL_GPIO_WritePin (ROW1_GPIO_Port, ROW1_Pin, GPIO_PIN_SET);  //Pull the R1 low
	HAL_GPIO_WritePin (ROW2_GPIO_Port, ROW2_Pin, GPIO_PIN_RESET);  // Pull the R2 High
	HAL_GPIO_WritePin (ROW3_GPIO_Port, ROW3_Pin, GPIO_PIN_SET);  // Pull the R3 High
	HAL_GPIO_WritePin (ROW4_GPIO_Port, ROW4_Pin, GPIO_PIN_SET);  // Pull the R4 High
	if (!(HAL_GPIO_ReadPin (COLUMN1_GPIO_Port, COLUMN1_Pin)))   // if the Col 1 is low
	{
		while (!(HAL_GPIO_ReadPin (COLUMN1_GPIO_Port, COLUMN1_Pin)));   // wait till the button is pressed
		character_defined = '4';
	}

	if (!(HAL_GPIO_ReadPin (COLUMN2_GPIO_Port, COLUMN2_Pin)))   // if the Col 2 is low
	{
		while (!(HAL_GPIO_ReadPin (COLUMN2_GPIO_Port, COLUMN2_Pin)));   // wait till the button is pressed
		character_defined = '5';
	}

	if (!(HAL_GPIO_ReadPin (COLUMN3_GPIO_Port, COLUMN3_Pin)))   // if the Col 3 is low
	{
		while (!(HAL_GPIO_ReadPin (COLUMN3_GPIO_Port, COLUMN3_Pin)));   // wait till the button is pressed
		character_defined = '6';
	}

	if (!(HAL_GPIO_ReadPin (COLUMN4_GPIO_Port, COLUMN4_Pin)))   // if the Col 4 is low
	{
		while (!(HAL_GPIO_ReadPin (COLUMN4_GPIO_Port, COLUMN4_Pin)));   // wait till the button is pressed
		character_defined = 'B';
	}
	 // Scan the third row
	HAL_GPIO_WritePin (ROW1_GPIO_Port, ROW1_Pin, GPIO_PIN_SET);  //Pull the R1 low
	HAL_GPIO_WritePin (ROW2_GPIO_Port, ROW2_Pin, GPIO_PIN_SET);  // Pull the R2 High
	HAL_GPIO_WritePin (ROW3_GPIO_Port, ROW3_Pin, GPIO_PIN_RESET);  // Pull the R3 High
	HAL_GPIO_WritePin (ROW4_GPIO_Port, ROW4_Pin, GPIO_PIN_SET);  // Pull the R4 High
	if (!(HAL_GPIO_ReadPin (COLUMN1_GPIO_Port, COLUMN1_Pin)))   // if the Col 1 is low
	{
		while (!(HAL_GPIO_ReadPin (COLUMN1_GPIO_Port, COLUMN1_Pin)));   // wait till the button is pressed
		character_defined = '7';
	}

	if (!(HAL_GPIO_ReadPin (COLUMN2_GPIO_Port, COLUMN2_Pin)))   // if the Col 2 is low
	{
		while (!(HAL_GPIO_ReadPin (COLUMN2_GPIO_Port, COLUMN2_Pin)));   // wait till the button is pressed
		character_defined = '8';
	}

	if (!(HAL_GPIO_ReadPin (COLUMN3_GPIO_Port, COLUMN3_Pin)))   // if the Col 3 is low
	{
		while (!(HAL_GPIO_ReadPin (COLUMN3_GPIO_Port, COLUMN3_Pin)));   // wait till the button is pressed
		character_defined = '9';
	}

	if (!(HAL_GPIO_ReadPin (COLUMN4_GPIO_Port, COLUMN4_Pin)))   // if the Col 4 is low
	{
		while (!(HAL_GPIO_ReadPin (COLUMN4_GPIO_Port, COLUMN4_Pin)));   // wait till the button is pressed
		character_defined = 'C';
	}
	 // Scan the fourth row
	HAL_GPIO_WritePin (ROW1_GPIO_Port, ROW1_Pin, GPIO_PIN_SET);  //Pull the R1 low
	HAL_GPIO_WritePin (ROW2_GPIO_Port, ROW2_Pin, GPIO_PIN_SET);  // Pull the R2 High
	HAL_GPIO_WritePin (ROW3_GPIO_Port, ROW3_Pin, GPIO_PIN_SET);  // Pull the R3 High
	HAL_GPIO_WritePin (ROW4_GPIO_Port, ROW4_Pin, GPIO_PIN_RESET);  // Pull the R4 High
	if (!(HAL_GPIO_ReadPin (COLUMN1_GPIO_Port, COLUMN1_Pin)))   // if the Col 1 is low
	{
		while (!(HAL_GPIO_ReadPin (COLUMN1_GPIO_Port, COLUMN1_Pin)));   // wait till the button is pressed
		character_defined = '*';
	}

	if (!(HAL_GPIO_ReadPin (COLUMN2_GPIO_Port, COLUMN2_Pin)))   // if the Col 2 is low
	{
		while (!(HAL_GPIO_ReadPin (COLUMN2_GPIO_Port, COLUMN2_Pin)));   // wait till the button is pressed
		character_defined = '0';
	}

	if (!(HAL_GPIO_ReadPin (COLUMN3_GPIO_Port, COLUMN3_Pin)))   // if the Col 3 is low
	{
		while (!(HAL_GPIO_ReadPin (COLUMN3_GPIO_Port, COLUMN3_Pin)));   // wait till the button is pressed
		character_defined = '#';
	}

	if (!(HAL_GPIO_ReadPin (COLUMN4_GPIO_Port, COLUMN4_Pin)))   // if the Col 4 is low
	{
		while (!(HAL_GPIO_ReadPin (COLUMN4_GPIO_Port, COLUMN4_Pin)));   // wait till the button is pressed
		character_defined = 'D';
	}
	return character_defined;

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
 	char keypad_character =0;
 	char component_code[8] ="";
	/*uint8_t rtext[_MAX_SS];  File read buffer */

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
  MX_USART2_UART_Init();
  MX_SPI2_Init();
  MX_FATFS_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  // Lcd_PortType ports[] = { D4_GPIO_Port, D5_GPIO_Port, D6_GPIO_Port, D7_GPIO_Port };
    Lcd_PortType ports[] = { GPIOC, GPIOB, GPIOA, GPIOA };
    // Lcd_PinType pins[] = {D4_Pin, D5_Pin, D6_Pin, D7_Pin};
    Lcd_PinType pins[] = {GPIO_PIN_7, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_6};
    Lcd_HandleTypeDef lcd;
    // Lcd_create(ports, pins, RS_GPIO_Port, RS_Pin, EN_GPIO_Port, EN_Pin, LCD_4_BIT_MODE);
    lcd = Lcd_create(ports, pins, GPIOB, GPIO_PIN_5, GPIOB, GPIO_PIN_4, LCD_4_BIT_MODE);

    keypad_character = read_from_keypad();
    component_code[strlen(component_code)+1] = keypad_character;
    Lcd_string(&lcd, keypad_character);
    HAL_Delay(500);

//the end of code to read from a .csv file

    FRESULT result = f_mount(&USERFatFS, (TCHAR const*)USERPath, 0);

        if (result == FR_OK) {
          transmit_uart("Mounted successfully\n");
        } else {
          transmit_uart("Mounting error\n");
        }

        result = f_open(&USERFile, "data.csv", FA_READ);

            if (result == FR_OK)
            {
                // Read and process each line of the CSV file
                TCHAR buffer[128]; // Adjust the buffer size as needed

                  while (1)
                  {
                    // ... (previous code)

                    char *token;
                    while (f_gets(buffer, sizeof(buffer), &USERFile)) // Read lines from the CSV file
                    {
                      token = strtok(buffer, ";"); // Assuming ";" is the delimiter
                      if (token != NULL)
                      {
                       // Remove escape from the token
                    	  token = removeNonAlphabetChars(token);
                      // Remove double quotes from the token (if present)
                        if (token[0] == '"')
                    	  {
                    	   memmove(token, token + 1, strlen(token));
                    	   token[strlen(token) - 1] = '\0';
                    	  }
                        if (strcmp(token, component_code) == 0) // Compare with "kamel"
                        {
                          token = strtok(NULL, ";"); // Get the value from the second column
                          token[strlen(token) - 1] = '\0';
                          Lcd_cursor(&lcd, 0,1);
                          Lcd_string(&lcd, token);
                          transmit_uart(token);
                          transmit_uart(":string founded successfully \n ");
                        }
                        else
                        {
                        	transmit_uart(token);
                        	transmit_uart(":string not founded \n ");
                        }
                      }
                    }
                  }
                    f_close(&USERFile);            }
            else
            {
                transmit_uart("Error opening CSV file. Error code: ");
                transmit_uart("\n");
            }



      //the end of code to read from a csv file







  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

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
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|LD2_Pin|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, COLUMN4_Pin|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, COLUMN3_Pin|COLUMN2_Pin|COLUMN1_Pin|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 LD2_Pin PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|LD2_Pin|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : COLUMN4_Pin PC7 */
  GPIO_InitStruct.Pin = COLUMN4_Pin|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : COLUMN3_Pin COLUMN2_Pin COLUMN1_Pin PB4
                           PB5 PB6 */
  GPIO_InitStruct.Pin = COLUMN3_Pin|COLUMN2_Pin|COLUMN1_Pin|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : ROW4_Pin ROW3_Pin ROW2_Pin */
  GPIO_InitStruct.Pin = ROW4_Pin|ROW3_Pin|ROW2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : ROW1_Pin */
  GPIO_InitStruct.Pin = ROW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ROW1_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
