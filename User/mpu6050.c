#include "stm32f1xx_hal.h"
#include "mpu6050.h"

MPU6050_TypeDef MPU6050[3]={0};
int jug_pitch[3]={0};
int jug_roll[3]={0};
int jug_yaw[3]={0};

void get_total_angle_MPU6050(MPU6050_TypeDef *p,int i)
{
	if((p->pitch>0&&p->pitch<p->last_pitch)||(p->pitch<0&&p->pitch>p->last_pitch))
	{
		jug_pitch[i]++;
	}
	if((p->roll>0&&p->roll<p->last_roll)||(p->roll<0&&p->roll>p->last_roll))
	{
		jug_roll[i]++;
	}
	if((p->yaw>0&&p->yaw<p->last_yaw)||(p->yaw<0&&p->yaw>p->last_yaw))
	{
		jug_yaw[i]++;
	}
	if(jug_pitch[i]>=500)
	{
		p->total_pitch+=p->pitch-p->last_pitch;
	}
	if(jug_roll[i]>=500)
	{
		p->total_roll+=p->roll-p->last_roll;
	}
	if(jug_yaw[i]>=500)
	{
		p->total_yaw+=p->yaw-p->last_yaw;
	}
	p->last_pitch=p->pitch;
	p->last_roll=p->roll;
	p->last_yaw=p->yaw;

	
}


//初始化MPU6050
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Init(IIC_TypeDef* IIC)
{
    uint8_t res;
    MPU_IIC_Init(IIC);//初始化IIC总线
    MPU_Write_Byte(IIC,MPU_PWR_MGMT1_REG,0X80);	//复位MPU6050
    delay_ms(100);
    MPU_Write_Byte(IIC,MPU_PWR_MGMT1_REG,0X00);	//唤醒MPU6050
    MPU_Set_Gyro_Fsr(IIC,3);					//陀螺仪传感器,±2000dps
    MPU_Set_Accel_Fsr(IIC,0);					//加速度传感器,±2g
    MPU_Set_Rate(IIC,500);						//设置采样率50Hz
    MPU_Write_Byte(IIC,MPU_INT_EN_REG,0X00);	//关闭所有中断
    MPU_Write_Byte(IIC,MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
    MPU_Write_Byte(IIC,MPU_FIFO_EN_REG,0X00);	//关闭FIFO
    MPU_Write_Byte(IIC,MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
    res=MPU_Read_Byte(IIC,MPU_DEVICE_ID_REG);
    if(res==MPU_ADDR)//器件ID正确
    {
        MPU_Write_Byte(IIC,MPU_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
        MPU_Write_Byte(IIC,MPU_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
        MPU_Set_Rate(IIC,50);						//设置采样率为50Hz
    } else return 1;
    return 0;
}
//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败
uint8_t MPU_Set_Gyro_Fsr(IIC_TypeDef* IIC,uint8_t fsr)
{
    return MPU_Write_Byte(IIC,MPU_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围
}
//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败
uint8_t MPU_Set_Accel_Fsr(IIC_TypeDef* IIC,uint8_t fsr)
{
    return MPU_Write_Byte(IIC,MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围
}
//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败
uint8_t MPU_Set_LPF(IIC_TypeDef* IIC,uint16_t lpf)
{
    uint8_t data=0;
    if(lpf>=188)data=1;
    else if(lpf>=98)data=2;
    else if(lpf>=42)data=3;
    else if(lpf>=20)data=4;
    else if(lpf>=10)data=5;
    else data=6;
    return MPU_Write_Byte(IIC,MPU_CFG_REG,data);//设置数字低通滤波器
}
//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败
uint8_t MPU_Set_Rate(IIC_TypeDef* IIC,uint16_t rate)
{
    uint8_t data;
    if(rate>1000)rate=1000;
    if(rate<4)rate=4;
    data=1000/rate-1;
    data=MPU_Write_Byte(IIC,MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
    return MPU_Set_LPF(IIC,rate/2);	//自动设置LPF为采样率的一半
}

//得到温度值
//返回值:温度值(扩大了100倍)
short MPU_Get_Temperature(IIC_TypeDef* IIC)
{
    uint8_t buf[2];
    short raw;
    float temp;
    MPU_Read_Len(IIC,MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf);
    raw=((uint16_t)buf[0]<<8)|buf[1];
    temp=36.53+((double)raw)/340;
    return temp*100;;
}
//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Get_Gyroscope(IIC_TypeDef* IIC,short *gx,short *gy,short *gz)
{
    uint8_t buf[6],res;
    res=MPU_Read_Len(IIC,MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
    if(res==0)
    {
        *gx=((uint16_t)buf[0]<<8)|buf[1];
        *gy=((uint16_t)buf[2]<<8)|buf[3];
        *gz=((uint16_t)buf[4]<<8)|buf[5];
    }
    return res;;
}
//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Get_Accelerometer(IIC_TypeDef* IIC,short *ax,short *ay,short *az)
{
    uint8_t buf[6],res;
    res=MPU_Read_Len(IIC,MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
    if(res==0)
    {
        *ax=((uint16_t)buf[0]<<8)|buf[1];
        *ay=((uint16_t)buf[2]<<8)|buf[3];
        *az=((uint16_t)buf[4]<<8)|buf[5];
    }
    return res;;
}
//IIC连续写
//addr:器件地址
//reg:寄存器地址
//len:写入长度
//buf:数据区
//返回值:0,正常
//    其他,错误代码
uint8_t MPU_Write_Len(IIC_TypeDef* IIC,uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
    uint8_t i;
    MPU_IIC_Start(IIC);
    MPU_IIC_Send_Byte(IIC,(addr<<1)|0);//发送器件地址+写命令
    if(MPU_IIC_Wait_Ack(IIC))	//等待应答
    {
        MPU_IIC_Stop(IIC);
        return 1;
    }
    MPU_IIC_Send_Byte(IIC,reg);	//写寄存器地址
    MPU_IIC_Wait_Ack(IIC);		//等待应答
    for(i=0; i<len; i++)
    {
        MPU_IIC_Send_Byte(IIC,buf[i]);	//发送数据
        if(MPU_IIC_Wait_Ack(IIC))		//等待ACK
        {
            MPU_IIC_Stop(IIC);
            return 1;
        }
    }
    MPU_IIC_Stop(IIC);
    return 0;
}
//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
uint8_t MPU_Read_Len(IIC_TypeDef* IIC,uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
    MPU_IIC_Start(IIC);
    MPU_IIC_Send_Byte(IIC,(addr<<1)|0);//发送器件地址+写命令
    if(MPU_IIC_Wait_Ack(IIC))	//等待应答
    {
        MPU_IIC_Stop(IIC);
        return 1;
    }
    MPU_IIC_Send_Byte(IIC,reg);	//写寄存器地址
    MPU_IIC_Wait_Ack(IIC);		//等待应答
    MPU_IIC_Start(IIC);
    MPU_IIC_Send_Byte(IIC,(addr<<1)|1);//发送器件地址+读命令
    MPU_IIC_Wait_Ack(IIC);		//等待应答
    while(len)
    {
        if(len==1)*buf=MPU_IIC_Read_Byte(IIC,0);//读数据,发送nACK
        else *buf=MPU_IIC_Read_Byte(IIC,1);		//读数据,发送ACK
        len--;
        buf++;
    }
    MPU_IIC_Stop(IIC);	//产生一个停止条件
    return 0;
}
//IIC写一个字节
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
uint8_t MPU_Write_Byte(IIC_TypeDef* IIC,uint8_t reg,uint8_t data)
{
    MPU_IIC_Start(IIC);
    MPU_IIC_Send_Byte(IIC,(MPU_ADDR<<1)|0);//发送器件地址+写命令
    if(MPU_IIC_Wait_Ack(IIC))	//等待应答
    {
        MPU_IIC_Stop(IIC);
        return 1;
    }
    MPU_IIC_Send_Byte(IIC,reg);	//写寄存器地址
    MPU_IIC_Wait_Ack(IIC);		//等待应答
    MPU_IIC_Send_Byte(IIC,data);//发送数据
    if(MPU_IIC_Wait_Ack(IIC))	//等待ACK
    {
        MPU_IIC_Stop(IIC);
        return 1;
    }
    MPU_IIC_Stop(IIC);
    return 0;
}
//IIC读一个字节
//reg:寄存器地址
//返回值:读到的数据
uint8_t MPU_Read_Byte(IIC_TypeDef* IIC,uint8_t reg)
{
    uint8_t res;
    MPU_IIC_Start(IIC);
    MPU_IIC_Send_Byte(IIC,(MPU_ADDR<<1)|0);//发送器件地址+写命令
    MPU_IIC_Wait_Ack(IIC);		//等待应答
    MPU_IIC_Send_Byte(IIC,reg);	//写寄存器地址
    MPU_IIC_Wait_Ack(IIC);		//等待应答
    MPU_IIC_Start(IIC);
    MPU_IIC_Send_Byte(IIC,(MPU_ADDR<<1)|1);//发送器件地址+读命令
    MPU_IIC_Wait_Ack(IIC);		//等待应答
    res=MPU_IIC_Read_Byte(IIC,0);//读取数据,发送nACK
    MPU_IIC_Stop(IIC);			//产生一个停止条件
    return res;
}
