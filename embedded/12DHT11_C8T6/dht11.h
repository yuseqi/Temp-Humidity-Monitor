#ifndef __DHT11_H
#define __DHT11_H
#include "main.h"

#define DHT11_Pin GPIO_PIN_12
#define DHT11_GPIO_Port GPIOB

//IO方向设置-->更换引脚时候需要替换
//8-0,9-4,10-8,11-12，12-16，13-20,14-24,15-28
#define DHT11_IO_IN()  {GPIOB->CRH&=0XFFF0FFFF;GPIOB->CRH|=8<<16;}
#define DHT11_IO_OUT() {GPIOB->CRH&=0XFFF0FFFF;GPIOB->CRH|=3<<16;}
   

uint8_t DHT11_Init(void);//初始化DHT11
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi);//读取温湿度
uint8_t DHT11_Read_Byte(void);//读出一个字节
uint8_t DHT11_Read_Bit(void);//读出一个位
uint8_t DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11  
#endif
