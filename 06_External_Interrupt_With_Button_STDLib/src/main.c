#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

GPIO_InitTypeDef GPIO_InitStruct;
EXTI_InitTypeDef EXTI_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;

void delay(uint32_t time)
{
	while(time--);
}

void GPIO_Config(void)
{
	// Clock ayarları
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	// LED pinleri: PD12, PD13, PD14, PD15 -> Output
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	// USER button: PA0 -> Input
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN; // Buton basıldığında HIGH olur
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void EXTI_Config(void)
{
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, GPIO_PinSource0);

	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; // Butona basılınca tetiklenecek
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}


void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		// LED sıralı animasyon: 12 → 14 → 13 → 15
		GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		delay(10000000);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);

		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		delay(10000000);
		GPIO_ResetBits(GPIOD, GPIO_Pin_14);

		GPIO_SetBits(GPIOD, GPIO_Pin_13);
		delay(10000000);
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);

		GPIO_SetBits(GPIOD, GPIO_Pin_15);
		delay(10000000);
		GPIO_ResetBits(GPIOD, GPIO_Pin_15);

		// Başlangıç haline dön (tüm LED’ler açık)
		GPIO_SetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

		// Kesme bayrağını temizle
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

int main(void)
{
	GPIO_Config();
	EXTI_Config();

	// Başlangıçta 4 LED açık
	GPIO_SetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

	while (1)
	{
		// Ana döngü boş — kesme bekleniyor
	}
}


void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size) { return; }
uint16_t EVAL_AUDIO_GetSampleCallBack(void) { return -1; }
