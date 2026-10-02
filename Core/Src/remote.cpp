/**
******************************************************************************
 * @file    remote.cpp/h
 * @brief   Remote control. 遥控器
 ******************************************************************************
 * Copyright (c) 2026 Team JiaoLong-SJTU
 * All rights reserved.
 ******************************************************************************
 */

#include "../Inc/remote.h"
#include "usart.h"
#include "string.h"

constexpr uint16_t REMOTE_CONNECT_TIMEOUT = 500u; 
// Constructor 构造函数
Remote::Remote(UART_HandleTypeDef *huart): huart_(huart), connect_(REMOTE_CONNECT_TIMEOUT){
    switch_.l = RCSwitchState_e::DOWN;
    switch_.r = RCSwitchState_e::DOWN;
}

// Start UART(SBUS) receive. 打开UART接收
void Remote::init() {
    // Your code here
    HAL_UARTEx_ReceiveToIdle_DMA(huart_, rx_buf, RC_RX_BUF_SIZE);
}

// Reset RC data. 重置遥控器数据
void Remote::reset() {
    // Your code here
    channel_.l_col = 0;
    channel_.r_col = 0;
    channel_.l_row = 0;
    channel_.r_row = 0;
    switch_.l = RCSwitchState_e::DOWN;
    switch_.r = RCSwitchState_e::DOWN;
    memset(rx_buf, 0, RC_RX_BUF_SIZE);
    memset(rx_data_, 0, RC_FRAME_LEN);
}

// Check for uart correspondence. 检查串口是否匹配
bool Remote::rxMsgCheck(UART_HandleTypeDef *huart) const {
    // Your code here
    if (huart == huart_) return true;
    else return false;
}

// Update connect status, restart UART(SBUS) receive.
// 更新连接状态，重新打开UART(SBUS)接收
void Remote::rxMsgCallback(){
    // Your code here 
    connect_.refresh();
    memcpy(rx_data_, rx_buf, 18u);
    this->init();
}

// Unpack data. 数据解包
void Remote::handle() {
    // Your code here
    if (!connect_.check()) return;
    channel_.r_col = ((static_cast<uint16_t>(rx_data_[0])) | (static_cast<uint16_t>(rx_data_[1])) << 8) & 0x07FF;
    channel_.r_row = ((static_cast<uint16_t>(rx_data_[1]) >> 3) | (static_cast<uint16_t>(rx_data_[2]) << 5)) & 0x07FF;
    channel_.l_col = ((static_cast<uint16_t>(rx_data_[2]) >> 6) | (static_cast<uint16_t>(rx_data_[3])) << 2 | (static_cast<uint16_t>(rx_data_[4]) << 10)) & 0x07FF;
    channel_.l_row = ((static_cast<uint16_t>(rx_data_[4]) >> 1) | (static_cast<uint16_t>(rx_data_[5]) << 7)) & 0x07FF;
    switch(((rx_data_[5] >> 4) & 0x000C) >> 2)
    {
        case 1: switch_.l = RCSwitchState_e::UP; break;
        case 2: switch_.l = RCSwitchState_e::DOWN; break;
        case 3: switch_.l = RCSwitchState_e::MID; break;
        default: break;
    }
    switch((rx_data_[5] >> 4) & 0x0003)
    {
        case 1: switch_.r = RCSwitchState_e::UP; break;
        case 2: switch_.r = RCSwitchState_e::DOWN; break;
        case 3: switch_.r = RCSwitchState_e::MID; break;
        default: break;
    }
}

Remote remote(&huart3);
