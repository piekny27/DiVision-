#include "main.h"
#include "GPIO_methods.c"

/* Private functions ---------------------------------------------------------*/
static GPIO_InitTypeDef GPIO_InitStruct;
/**
 * @brief  Main program
 * @param  None
 * @retval None
 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)		//metoda przerwañ (pamiêtaj o funkcji w pliku stm32f3xx_it.c)
				{
				 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3); 			// zmieñ stan diody
				}

int main(void) {


		/* This function configures the Flash prefetch, configures time base source, NVIC and Low level hardware */
			HAL_Init();
			SystemInit();

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
			GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;	//konfiguracja wejœcia jako przerwanie (inicjacja zarówno przy 																		zboczu narastaj¹cym jak i opadaj¹cym
			GPIO_InitStruct.Pull = GPIO_PULLUP;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
			HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

			/*uruchomienie przerwañ zewnêtrznych */
			HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);









}

#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(char *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

