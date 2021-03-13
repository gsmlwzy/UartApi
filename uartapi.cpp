/*
 * @brief 通过串口发送数据
 * @author gsmlwyz
 * @attention
 *      发送字符型:
 *          SendStr() 如果发送字符串"Hello"这样的, 推荐(char *)"Hello", 这个由c++特性决定
 *      发送无符号整型(uint8_t, uint16_t, uint32_t)
 *          SendNumber(), 但是需要全部做一次(uint32_t)的强制类型转换, 否则会重载失败
 *      发送有符号整型(int8_t, int16_t, int32_t)
 *          SendNumber(), 也需要做一次(int32_t)的强制类型转换, 否则重载失败
 *      发送单精度浮点数(float)
 *          SendNumber(num), 默认保留小数点后2位, 采用去尾法保留的, 后续会改进, 可以通过修改dec改变保留位数
 *              只能发送float类型, 发送类似1000.252这样的数据时, 会重载失败, 两种解决方案, (强制类型转换, 声明为单精度1000.252f)
 *      目前只测试了(char, uint8_t, uint16_t, uint32_t, int8_t, int16_t, int32_t, float),
 *      其他类型均未测试, 不推荐传入其他类型
 * @data 2021/3/12.
 *
 * @attition
 *      实现 << 运算符重载, 可以更方便的传输数据, 但是现在发送函数不可见, 必须采用 <<, 依然需要进行强制类型转换(或者叫匿名对象生成)
 *      缺点很明显, 无法对浮点数的精度进行控制, 需要在实例化串口时就指定精度
 * @data 2021/3/13
 */

// 外部库
#include "uartapi.h"
// 标准库
#include "stdlib.h"
#include "math.h"
#include "memory.h"
UartTransApi serial(&huart1);

void Test(void)
{
    serial << int32_t(100) << uint32_t(200) << 3.145f << (char *)"\r\n";
}

UartTransApi::UartTransApi(UART_HandleTypeDef *huart_)
    : huart(huart_), buff{0}, dec(2)
{
}

UartTransApi::UartTransApi(UART_HandleTypeDef *huart_, int dec_)
    : huart(huart_), buff{0}, dec(dec_)
{
}

void UartTransApi::SendStr(char *str)
{
    unsigned int k = 0;
    do
    {
        HAL_UART_Transmit(huart, (uint8_t *)(str + k), 1, 1000);
        k++;
    } while (*(str + k) != '\0');
    buffEraser();
}

void UartTransApi::SendNumber(uint32_t num)
{
    utoa(num, buff, 10);
    this->SendStr(buff);
}

void UartTransApi::SendNumber(int32_t num)
{
    itoa(num, buff, 10);
    this->SendStr(buff);
}

void UartTransApi::SendNumber(float num)
{
    int32_t num_int = (int32_t)num;
    // 不进行四舍五入, 数据有一点的难看, 但是不影响精度
    uint32_t num_dec = (uint32_t)((num - num_int) * pow(10.0, dec)); // 这儿没有进行四舍五入, 做的是去尾法, 先不管了
    this->SendNumber(num_int);
    this->SendStr((char *)".");
    this->SendNumber(num_dec);
}

void UartTransApi::buffEraser()
{
    memset(buff, 0, sizeof(buff));
}

UartTransApi &operator<<(UartTransApi &self, int32_t num)
{
    self.SendNumber(num);
    return self;
}

UartTransApi &operator<<(UartTransApi &self, uint32_t num)
{
    self.SendNumber(num);
    return self;
}

UartTransApi &operator<<(UartTransApi &self, char *str)
{
    self.SendStr(str);
    return self;
}

UartTransApi &operator<<(UartTransApi &self, float num)
{
    self.SendNumber(num);
    return self;
}