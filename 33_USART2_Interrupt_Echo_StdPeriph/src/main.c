#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

GPIO_InitTypeDef GPIO_InitStruct;
USART_InitTypeDef USART_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;

char str[50];
char rx_buff[50];
int i = 0;

void delay(uint32_t time)
{
    while (time--);
}

void GPIO_Config(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); // PA2 -> Tx
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); // PA3 -> Rx

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void USART_Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;

    USART_Init(USART2, &USART_InitStruct);
    USART_Cmd(USART2, ENABLE);

    // RX interrupt enable
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void NVIC_Config(void)
{
    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);
}

void USART_Puts(USART_TypeDef* USARTx, volatile char *s)
{
    while (*s)
    {
        while (!(USARTx->SR & 0x00000040)); // Wait for transmission complete
        USART_SendData(USARTx, *s);
        s++;
    }
}

void USART2_IRQHandler(void)
{
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        char rx_char = USART_ReceiveData(USART2);

        if (rx_char != '\n' && rx_char != '\r' && i < 49)
        {
            rx_buff[i++] = rx_char;
        }
        else
        {
            rx_buff[i] = '\0'; // String terminator
            i = 0;
            USART_Puts(USART2, rx_buff); // Echo back to terminal

            // Buffer reset
            for (int a = 0; a < 50; a++)
                rx_buff[a] = '\0';
        }

        // Clear interrupt flag
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}

int main(void)
{
    GPIO_Config();
    USART_Config();
    NVIC_Config();

    USART_Puts(USART2, "USART2 Ready. Type something:\r\n");

    while (1)
    {
        // Main loop intentionally empty; all handled by interrupt
    }
}

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size)
{
    /* TODO, implement your code here */
    return;
}

uint16_t EVAL_AUDIO_GetSampleCallBack(void)
{
    /* TODO, implement your code here */
    return -1;
}
