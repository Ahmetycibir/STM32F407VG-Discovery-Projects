#include "stm32f4xx.h"

uint8_t adc_value = 0;

void GPIO_Config(void) {
    // Enable clock for GPIOD and GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOAEN;

    // GPIOD pins 12-15 output
    GPIOD->MODER &= ~(0xFF000000);          // Clear MODER12-15
    GPIOD->MODER |= 0x55000000;             // Set MODER12-15 to output (01)

    GPIOD->OTYPER &= ~(0xF000);             // Push-pull
    GPIOD->OSPEEDR |= 0xFF000000;           // High speed
    GPIOD->PUPDR &= ~(0xFF000000);          // No pull-up/pull-down

    // GPIOA pin 1 analog
    GPIOA->MODER |= (3 << (1*2));           // PA1 analog mode (11)
}

void ADC_Config(void) {
    // Enable ADC1 clock
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    // ADC prescaler (common)
    ADC->CCR |= ADC_CCR_ADCPRE_0; // Prescaler /4

    // ADC1 configuration
    ADC1->CR1 = 0;                    // 8-bit resolution is bits 24:25 (00 for 12-bit, 01 for 10-bit, 10 for 8-bit)
    ADC1->CR1 |= (2 << 24);           // 8-bit resolution

    ADC1->CR2 = ADC_CR2_ADON;         // Enable ADC
}

uint8_t Read_ADC(void) {
    // Select channel 1 (PA1)
    ADC1->SQR3 = 1;                    // 1. conversion in regular sequence
    ADC1->SQR1 = 0;                    // Only one conversion

    ADC1->CR2 |= ADC_CR2_SWSTART;      // Start conversion

    // Wait until conversion complete
    while (!(ADC1->SR & ADC_SR_EOC));

    return ADC1->DR;                   // 8-bit conversion result
}

void Update_LEDs(uint8_t value) {
    GPIOD->ODR &= ~(0xF << 12); // Clear D12-D15

    if (value < 50) {
        GPIOD->ODR |= (1 << 12);            // D12
    } else if (value <= 100) {
        GPIOD->ODR |= (1 << 12) | (1 << 14); // D12 + D14
    } else if (value <= 150) {
        GPIOD->ODR |= (1 << 13) | (1 << 15); // D13 + D15
    } else if (value <= 200) {
        GPIOD->ODR |= 0xF << 12;             // All D12-D15
    } else if (value <= 250) {
        GPIOD->ODR |= (1 << 15);             // D15
    }
}

int main(void) {
    GPIO_Config();
    ADC_Config();

    while (1) {
        adc_value = Read_ADC();
        Update_LEDs(adc_value);
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
