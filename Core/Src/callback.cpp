//
// Created by Siwei Wang on 2026/10/2.
//
#include "../Inc/main.h"
#include "../Inc/dma.h"
#include "../Inc/usart.h"
#include "../Inc/gpio.h"
#include "../Inc/remote.h"
#include "../Inc/connect.hpp"
extern "C"
{

extern Remote remote;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart == &huart3
        && (HAL_UARTEx_GetRxEventType(huart) == HAL_UART_RXEVENT_IDLE || HAL_UARTEx_GetRxEventType(huart) == HAL_UART_RXEVENT_TC) )
    {
        remote.rxMsgCallback();
        remote.handle();
    }
}


    void remote_shadow_init(){
        remote.init();
    }
}
