#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void delay(uint32_t time)
{
	while(time--);
}

void GPIO_Config(void)
{
	RCC->AHB1ENR |= (1 << 0);  // GPIOA clock enable (Button)
	RCC->AHB1ENR |= (1 << 3);  // GPIOD clock enable (LEDs)

	// PD12, PD13, PD14, PD15 -> output
	GPIOD->MODER |= (1 << 24) | (1 << 26) | (1 << 28) | (1 << 30);
	GPIOD->OTYPER = 0x0000;     // Push-pull
	GPIOD->OSPEEDR |= (3 << 24) | (3 << 26) | (3 << 28) | (3 << 30);
	GPIOD->PUPDR = 0x0000;      // No pull-up/down

	// PA0 -> input (User button)
	GPIOA->MODER &= ~0x3;       // Input mode
	GPIOA->PUPDR = 0x00;        // No pull-up/pull-down
}

int main(void)
{
	GPIO_Config();

	while (1)
	{
		// Butona basýldý mý?
		if (GPIOA->IDR & 0x00000001)
		{
			// Buton býrakýlana kadar bekle (debounce)
			while (GPIOA->IDR & 0x00000001);
			delay(1000000);

			// --- LED’leri sýrayla yak ---
			GPIOD->ODR = (1 << 12);
			delay(10000000);

			GPIOD->ODR |= (1 << 13);
			delay(10000000);

			GPIOD->ODR |= (1 << 14);
			delay(10000000);

			GPIOD->ODR |= (1 << 15);
			delay(10000000);

			// --- LED’leri sýrayla söndür ---
			GPIOD->ODR &= ~(1 << 15);
			delay(10000000);

			GPIOD->ODR &= ~(1 << 14);
			delay(10000000);

			GPIOD->ODR &= ~(1 << 13);
			delay(10000000);

			GPIOD->ODR &= ~(1 << 12);
			delay(10000000);
		}
	}
}

// Ses callback'leri (gerekli deðil ama derleme için boþ býrakýldý)
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size) { return; }
uint16_t EVAL_AUDIO_GetSampleCallBack(void) { return -1; }
