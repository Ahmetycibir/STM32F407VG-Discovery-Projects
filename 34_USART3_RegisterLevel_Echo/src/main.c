
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

char Rx_Buff[100];
int i=0;



void RCC_Config()
{
	RCC->CR|=0x00010000;//HESON  enable
	while(!(RCC->CR&0x00020000));//HSEON Ready Flag Wait
	RCC->CR|=0x00080000;//CSS Enable
	RCC->CR|=0x01000000;//PLL ON
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
void GPIO_Config()
{
RCC->AHB1ENR|=0x00000002;//GPIOD Clock Enable
GPIOB->MODER|=(2<<20|2<<22);//PB10 AND PB11 AF
GPIOB->AFR[1]|=(7<<8)|(7<<12);//PB10 And PB11 AF7 (USART3)

}
void USART_Config()
{
	RCC->APB1ENR|=1<<18; // USART3 CLOCK ENABLE
	USART3->BRR=0x1112;//BaudRate 9600
	USART3->CR1|=1<<2; //Rx enable
	USART3->CR1|=1<<3; //Tx enable
	USART3->CR1|=1<<5; //RXNE interrupt enable
	USART3->CR1|=0<<10; //No parity
	USART3->CR1|=0<<12; //Word lenght 8 bit
	USART3->CR2|=0<<12; //Stop bit 1


	USART3->CR1|=1<<13; //USART ENABLE

}
void NVIC_Config()
{
NVIC->ISER[1] |=1<<7;

}
void USART3_IRQHandler()
{
volatile int Str;
Str=USART3->SR;
Rx_Buff[i]=USART3->DR;
i++;

}
void Send_Char(char message)
{
while(!(USART3->SR&0x00000080)); //TXE Buffer dolu ise bekle
USART3->DR=message;
}

void Send_Message(char *Str)
{
	while(*Str)
	{
		Send_Char(*Str);
		Str++;

	}

}

int main(void)
{
	 RCC_Config();
	 GPIO_Config();
	 USART_Config();
	 NVIC_Config();

  while (1)
  {
	Send_Message("Hello World \n");
	for(int i =0;i<100000;i++);
  }
}



void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}


uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
