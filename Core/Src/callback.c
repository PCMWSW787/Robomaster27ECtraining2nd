//
// Created by Siwei Wang on 2026/10/2.
//
#include <string.h>

#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "../../Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_gpio.h"

extern uint8_t rx_msg[10];
extern const uint8_t rx_tx_size;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart == &huart1 &&
        (HAL_UARTEx_GetRxEventType(huart) == HAL_UART_RXEVENT_TC ||
            HAL_UARTEx_GetRxEventType(huart) == HAL_UART_RXEVENT_IDLE))
    {
        // if (rx_msg[0] == 'R')
        //     HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET);
        // else if (rx_msg[0] == 'M')
        //     HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
        HAL_UART_Transmit_DMA(&huart1, rx_msg, Size);
    }
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1)
    {
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_msg, rx_tx_size);
    }
}
