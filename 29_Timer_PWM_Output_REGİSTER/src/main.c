
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"


void RCC_Config()
{
	RCC->CR|=0x00010000;//HESON and HSEONRDY enable
		while(!(RCC->CR&0x00020000));//HSEON Ready Flag Wait
		RCC->CR|=0x00080000;//CSS Enable
		RCC->CR|0x00100000;//PLL ON
		RCC->PLLCFGR|=0x00400000;//PLL e HSE seçtik
		RCC->PLLCFGR|=0x00000004;//PLL M=4
		RCC->PLLCFGR|=0x00005A00;//PLL N=168
		RCC->PLLCFGR|=0x00000000;//PLL p=2
		RCC->CFGR|=0x00000000;//AHB Prescaler=1
		RCC->CFGR|=0x00080000;//AHB Prescaler=2
		RCC->CFGR|=0x00001400;//AHB Prescaler=4
		RCC->CIR|=0x00080000;//HSERDY Flag clear
		RCC->CIR|=0x00800000;//CSS Flag clear
}
void TIM_Config()
{
RCC->APB1ENR|=1<<2;//Tim 2 clock enable
TIM4->CR1|=0<<4;//Timer counter mode upcounter
TIM4->CR1|=0<<4;//Timer center-aligned mode edge-aligned mode
TIM4->CR1|=0<<8;//Timer clock div 1
TIM4->CCMR1|=0<<0|6<<4|0<<8|6<<12;
/*
 * CAPTURE/COMPARE 1&2 OUTPUT SELECTED
 * CAPTURE/COMPARE 1&2 PWM1 SELECTED
 */
TIM4->CCMR2|=0<<0|6<<4|0<<8|6<<12;
/*
 * CAPTURE/COMPARE 3&4 OUTPUT SELECTED
 * CAPTURE/COMPARE 3&4 PWM1 SELECTED
 */
TIM4->CCER|=1<<0|1<<4|1<<8|1<<12;
/*
 * CAPTURE/COMPARE 1&2&3&4 OUTPUT enable
 *
 */
TIM4->PSC=8399;
TIM4->ARR=9999;
TIM4->CCR1=2499;
TIM4->CCR2=4999;
TIM4->CCR3=7999;
TIM4->CCR4=9999;
TIM4->CR1|=1<<0;

}
void GPIO_Config()
{
	RCC->AHB1ENR|=1<<3;//GPIOD Clock enable
	GPIOD->MODER|=2<<24|2<<26|2<<28|2<<30;//PortD -> Pin12|13|14|15 AF
	GPIOD->AFR[1]|=2<<16|2<<20|2<<24|2<<28;//PortD -> Pin12|13|14|15 AF
}

int main(void)
{
	RCC_Config();
	GPIO_Config();
	TIM_Config();

  while (1)
  {

  }
}



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
