//
// Created by i^2 on 2021/3/12.
//

#ifndef UARTAPI_UARTAPI_H
#define UARTAPI_UARTAPI_H

#include "stdint.h"
#include "usart.h"
class UartTransApi
{
public:
    explicit UartTransApi(UART_HandleTypeDef *huart_);
    void SendStr(char *str);
    void SendNumber(uint32_t num);
    void SendNumber(int32_t num);
    void SendNumber(float num, int dec = 2);
private:
    UART_HandleTypeDef *huart;
    char buff[32];
    void buffEraser(void);
};

void Test(void);
#endif //UARTAPI_UARTAPI_H
