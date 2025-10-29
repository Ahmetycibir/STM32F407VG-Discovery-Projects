
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

GPIO_InitTypeDef GPIO_InitStruct;
ADC_InitTypeDef ADC_InitStruct;
ADC_CommonInitTypeDef ADC_CommonInitStruct;
uint8_t adc_value = 0;

void GPIO_Config()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    // LED pinleri: GPIOD 12-15
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    // ADC pin: PA1
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void ADC_Config()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;
    ADC_CommonInit(&ADC_CommonInitStruct);

    ADC_InitStruct.ADC_Resolution = ADC_Resolution_8b;
    ADC_Init(ADC1, &ADC_InitStruct);

    ADC_Cmd(ADC1, ENABLE);
}

uint8_t Read_ADC()
{
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_56Cycles);
    ADC_SoftwareStartConv(ADC1);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    return ADC_GetConversionValue(ADC1);
}

int main(void)
{
    GPIO_Config();
    ADC_Config();

    while (1)
    {
        adc_value = Read_ADC();

        // �nce t�m LEDleri kapat
        GPIO_ResetBits(GPIOD, GPIO_Pin_All);

        if (adc_value < 50)
        {
            GPIO_SetBits(GPIOD, GPIO_Pin_12);             // Sadece D12
        }
        else if (adc_value >= 50 && adc_value <= 100)
        {
            GPIO_SetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_14); // D12 ve D14
        }
        else if (adc_value > 100 && adc_value <= 150)
        {
            GPIO_SetBits(GPIOD, GPIO_Pin_13 | GPIO_Pin_15); // D13 ve D15
        }
        else if (adc_value > 150 && adc_value <= 200)
        {
            GPIO_SetBits(GPIOD, GPIO_Pin_All);              // T�m pinler
        }
        else if (adc_value > 200 && adc_value <= 250)
        {
            GPIO_SetBits(GPIOD, GPIO_Pin_15);              // Sadece D15
        }
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
