#include "stm32f4xx.h"

void delay(uint32_t t)
{
    while(t--);
}

void Dac_Config(void)
{
    /* GPIOA ve DAC saatlerini aç */
    RCC->AHB1ENR |= (1U << 0);
    RCC->APB1ENR |= (1U << 29);

    /* PA4 analog moda */
    GPIOA->MODER |= (3U << (4 * 2));

    /* DAC kanal 1'i aktif et */
    DAC->CR = 0;
    DAC->CR |= (1U << 0);  // EN1 = 1 (DAC enable)
    DAC->CR &= ~(1U << 1); // Buffer ON (default)
}

int main(void)
{
    Dac_Config();

    while (1)
    {
        for (uint16_t i = 0; i < 4096; i++)
        {
            DAC->DHR12R1 = i;
            delay(16800);
        }

        for (uint16_t i = 4095; i > 0; i--)
        {
            DAC->DHR12R1 = i;
            delay(16800);
        }
    }
}


/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
