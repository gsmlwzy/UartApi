//
// Created by i^2 on 2021/3/12.
//

#ifndef UARTAPI_UARTAPI_H
#define UARTAPI_UARTAPI_H

#include "stdint.h"
#include "usart.h"
class UartTransApi;

class UartTransApi
{
public:
    explicit UartTransApi(UART_HandleTypeDef *huart_);
    UartTransApi(UART_HandleTypeDef *huart_, int dec_);

    friend UartTransApi &operator<<(UartTransApi &self, int32_t num);
    friend UartTransApi &operator<<(UartTransApi &self, uint32_t num);
    friend UartTransApi &operator<<(UartTransApi &self, char *str);
    friend UartTransApi &operator<<(UartTransApi &self, float num);

private:
    UART_HandleTypeDef *huart;
    char buff[32];
    const int dec; // float类型保留的精度

    void SendStr(char *str);
    void SendNumber(uint32_t num);
    void SendNumber(int32_t num);
    void SendNumber(float num);

    void buffEraser(void);
};

void Test(void);
#endif //UARTAPI_UARTAPI_H
