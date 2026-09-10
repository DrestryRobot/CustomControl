#ifndef _IIC_H
#define _IIC_H

#include <inttypes.h>
#include "stm32f1xx_hal.h"
#include <stm32f103xb.h>
#include <stm32f1xx_hal_def.h>


#define IIC_WR	0		/* 写控制bit */
#define IIC_RD	1		/* 读控制bit */

typedef struct {
    GPIO_TypeDef* GPIO_Port;  // GPIO端口
    uint16_t SCL_Pin;  // SCL引脚
    uint16_t SDA_Pin;  // SDA引脚
    //void (*RCC_Enable)(void);  // RCC使能函数
} IIC_TypeDef;



extern IIC_TypeDef IIC1;
extern IIC_TypeDef IIC2;
extern IIC_TypeDef IIC3;



void IIC_Start(IIC_TypeDef* IIC);
void IIC_Stop(IIC_TypeDef* IIC);
void IIC_Send_Byte(IIC_TypeDef* IIC,uint8_t _ucByte);
uint8_t IIC_Read_Byte(IIC_TypeDef* IIC,uint8_t ack);
uint8_t IIC_Wait_Ack(IIC_TypeDef* IIC);
void IIC_Ack(IIC_TypeDef* IIC);
void IIC_NAck(IIC_TypeDef* IIC);
uint8_t IIC_CheckDevice(IIC_TypeDef* IIC,uint8_t _Address);
void IIC_GPIO_Init(IIC_TypeDef* IIC);


#endif

