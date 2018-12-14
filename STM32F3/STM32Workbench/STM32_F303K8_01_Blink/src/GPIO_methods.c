/**
 ******************************************************************************
 * @file    GPIO_methods.c
 * @author  Mateusz
 * @version V1.0
 * @date    02.12.18
 * @brief   Plik z bibliotek¹ GPIO
 ******************************************************************************
 */

/*
 * Przydatne kody:


 STEROWANIE WYJŒCIEM NP. DIOD¥

 GPIO_InitTypeDef gpio;										 // obiekt gpio bêd¹cy konfiguracj¹ portów GPIO
 gpio.Pin = GPIO_PIN_5; 									 // konfigurujemy pin 5
 gpio.Mode = GPIO_MODE_OUTPUT_PP; 							 // jako wyjœcie
 gpio.Pull = GPIO_NOPULL; 								     // rezystory podci¹gaj¹ce s¹ wy³¹czone
 gpio.Speed = GPIO_SPEED_FREQ_LOW; 						     // wystarcz¹ nieskie czêstotliwoœci prze³¹czania
 HAL_GPIO_Init(GPIOA, &gpio); 							     // inicjalizacja modu³u GPIOA



 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void blinkInit(void);
static void blinkToggle(int delay);
static void blinkWritePin(int delaySet, int delayReset);

static void buttonInit(void);
static void SystemClock_Config(void);
static void przyciskDioda(void);
/* Private functions ---------------------------------------------------------*/

/* NASZ KOD ----------------------------------------------------------------- */

/**
 * @brief  Kod do inicjacji funkcji blink
 * @param  None
 * @retval None
 */
void blinkInit(void) {

	/* This function configures the Flash prefetch, configures time base source, NVIC and Low level hardware */
	HAL_Init();

	/* Configure the system clock to 64 MHz */
	SystemClock_Config();

	/* Enable GPIO Clock (to be able to program the configuration registers) */
	__HAL_RCC_GPIOB_CLK_ENABLE()
	;

	/* GPIO init structure definition*/
	static GPIO_InitTypeDef GPIO_InitStruct;

	/* Configure IO in output push-pull mode to drive external LEDs */
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
 * @brief  Funkcja blink z prze³¹czniem toggle
 * @param  delay - czas miêdzy prze³¹czaniem w ms
 * @retval None
 */
void blinkToggle(int delay) {

	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
	HAL_Delay(delay);
}

/**
 * @brief  Funkcja blink z u¿yciem przypisania wartoœci pinu LOW/HIGH
 * @param  delaySet - czas wysokiego pinu (ms)
 * 		delayReset - czas stanu niskiego pinu (ms)
 * @retval None
 */
void blinkWritePin(int delaySet, int delayReset) {

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
	HAL_Delay(delaySet);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_Delay(delayReset);
}

/**
 * @brief  Kod do inicjacji przycisku pod pin D3 [PB_0]
 * @param  None
 * @retval None
 */
void buttonInit(void) {

	/* This function configures the Flash prefetch, configures time base source, NVIC and Low level hardware */
	HAL_Init();

	/* Configure the system clock to 64 MHz */
	SystemClock_Config();

	/* Enable GPIO Clock (to be able to program the configuration registers) */
	__HAL_RCC_GPIOB_CLK_ENABLE()
	;

	/* GPIO init structure definition*/
	static GPIO_InitTypeDef GPIO_InitStruct;

	/* Configure IO in input pull-up mode to read button */
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
 * @brief  Po wciœniêcu przycisku D2 [PA_12] zapala siê dioda D13 [PB_3]
 * @param  None
 * @retval None
 */
void przyciskDioda(void){

	static GPIO_InitTypeDef GPIO_InitStruct;

	/* This function configures the Flash prefetch, configures time base source, NVIC and Low level hardware */
		HAL_Init();

		/* Configure the system clock to 64 MHz */
		SystemClock_Config();

		/* Enable GPIO Clock (to be able to program the configuration registers) */
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();

		/* Configure IO in output push-pull mode to drive external LEDs in port B */
		GPIO_InitStruct.Pin = GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* Configure IO in input pull-up mode to read button in port A*/
		GPIO_InitStruct.Pin = GPIO_PIN_12;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		while (1) {

			//blinkToggle(500);
			if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
			} else {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);

			}
		}
	}



/* NIE NASZ KOD ------------------------------------------------------------- */

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSI)
 *            SYSCLK(Hz)                     = 64000000
 *            HCLK(Hz)                       = 64000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 2
 *            APB2 Prescaler                 = 1
 *            PLLMUL                         = RCC_PLL_MUL16 (16)
 *            Flash Latency(WS)              = 2
 * @param  None
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* HSI Oscillator already ON after system reset, activate PLL with HSI as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		/* Initialization Error */
		while (1)
			;
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		/* Initialization Error */
		while (1)
			;
	}
}
