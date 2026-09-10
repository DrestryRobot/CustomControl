#include "stm32f1xx_hal.h"
#include "IIC.h"

/* ¶¨Òå¶ ÁÐ´SCLºÍSDAµÄºê£¬ÒÑÔö¼Ó´úÂëµÄ¿ÉÒÆÖ²ÐÔºÍ¿ÉÔÄ¶ÁÐÔ */
#define IIC_SCL_1(IIC) HAL_GPIO_WritePin((IIC)->GPIO_Port, (IIC)->SCL_Pin, GPIO_PIN_SET)
#define IIC_SCL_0(IIC) HAL_GPIO_WritePin((IIC)->GPIO_Port, (IIC)->SCL_Pin, GPIO_PIN_RESET)
#define IIC_SDA_1(IIC) HAL_GPIO_WritePin((IIC)->GPIO_Port, (IIC)->SDA_Pin, GPIO_PIN_SET)
#define IIC_SDA_0(IIC) HAL_GPIO_WritePin((IIC)->GPIO_Port, (IIC)->SDA_Pin, GPIO_PIN_RESET)
#define IIC_SDA_READ(IIC) HAL_GPIO_ReadPin((IIC)->GPIO_Port, (IIC)->SDA_Pin)

/* ¶¨ÒåIIC×ÜÏßÁ¬½ÓµÄGPIO¶Ë¿Ú, ÓÃ»§Ö»ÐèÒªÐÞ¸ÄÏÂÃæ4ÐÐ´úÂë¼´¿ÉÈÎÒâ¸Ä±äSCLºÍSDAµÄÒý½Å */
IIC_TypeDef IIC1 = {GPIOB, GPIO_PIN_4, GPIO_PIN_5};
IIC_TypeDef IIC2 = {GPIOB, GPIO_PIN_6, GPIO_PIN_7};
IIC_TypeDef IIC3 = {GPIOB, GPIO_PIN_8, GPIO_PIN_9};

/*
*********************************************************************************************************
*	º¯ Êý Ãû: IIC_Delay
*	¹¦ÄÜËµÃ÷: IIC×ÜÏßÎ»ÑÓ³Ù£¬×î¿ì400KHz
*	ÐÎ    ²Î£ºÎÞ
*	·µ »Ø Öµ: ÎÞ
*********************************************************************************************************
*/
static void IIC_Delay(void)
{
    uint8_t i;

    /*¡¡
     	ÏÂÃæµÄÊ±¼äÊÇÍ¨¹ý°²¸»À³AX-ProÂß¼­·ÖÎöÒÇ²âÊÔµÃµ½µÄ¡£
    	CPUÖ÷Æµ72MHzÊ±£¬ÔÚÄÚ²¿FlashÔËÐÐ, MDK¹¤³Ì²»ÓÅ»¯
    	Ñ­»·´ÎÊýÎª10Ê±£¬SCLÆµÂÊ = 205KHz
    	Ñ­»·´ÎÊýÎª7Ê±£¬SCLÆµÂÊ = 347KHz£¬ SCL¸ßµçÆ½Ê±¼ä1.5us£¬SCLµÍµçÆ½Ê±¼ä2.87us
     	Ñ­»·´ÎÊýÎª5Ê±£¬SCLÆµÂÊ = 421KHz£¬ SCL¸ßµçÆ½Ê±¼ä1.25us£¬SCLµÍµçÆ½Ê±¼ä2.375us

    IAR¹¤³Ì±àÒëÐ§ÂÊ¸ß£¬²»ÄÜÉèÖÃÎª7
    */
    for (i = 0; i < 10; i++);
}

/*
*********************************************************************************************************
*	º¯ Êý Ãû: IIC_Start
*	¹¦ÄÜËµÃ÷: CPU·¢ÆðIIC×ÜÏßÆô¶¯ÐÅºÅ
*	ÐÎ    ²Î£ºÎÞ
*	·µ »Ø Öµ: ÎÞ
*********************************************************************************************************
*/
void IIC_Start(IIC_TypeDef* IIC)
{
    /* µ±SCL¸ßµçÆ½Ê±£¬SDA³öÏÖÒ»¸öÏÂÌøÑØ±íÊ¾IIC×ÜÏßÆô¶¯ÐÅºÅ */
    IIC_SDA_1(IIC);
    IIC_SCL_1(IIC);
    IIC_Delay();
    IIC_SDA_0(IIC);
    IIC_Delay();
    IIC_SCL_0(IIC);
    IIC_Delay();
}

/*
*********************************************************************************************************
*	º¯ Êý Ãû: IIC_Start
*	¹¦ÄÜËµÃ÷: CPU·¢ÆðIIC×ÜÏßÍ£Ö¹ÐÅºÅ
*	ÐÎ    ²Î£ºÎÞ
*	·µ »Ø Öµ: ÎÞ
*********************************************************************************************************
*/
void IIC_Stop(IIC_TypeDef* IIC)
{
    /* µ±SCL¸ßµçÆ½Ê±£¬SDA³öÏÖÒ»¸öÉÏÌøÑØ±íÊ¾IIC×ÜÏßÍ£Ö¹ÐÅºÅ */
    IIC_SDA_0(IIC);
    IIC_SCL_1(IIC);
    IIC_Delay();
    IIC_SDA_1(IIC);
}

/*
*********************************************************************************************************
*	º¯ Êý Ãû: IIC_SendByte
*	¹¦ÄÜËµÃ÷: CPUÏòIIC×ÜÏßÉè±¸·¢ËÍ8bitÊý¾Ý
*	ÐÎ    ²Î£º_ucByte £º µÈ´ý·¢ËÍµÄ×Ö½Ú
*	·µ »Ø Öµ: ÎÞ
*********************************************************************************************************
*/
void IIC_Send_Byte(IIC_TypeDef* IIC,uint8_t _ucByte)
{
    uint8_t i;

    /* ÏÈ·¢ËÍ×Ö½ÚµÄ¸ßÎ»bit7 */
    for (i = 0; i < 8; i++)
    {
        if (_ucByte & 0x80)
        {
            IIC_SDA_1(IIC);
        }
        else
        {
            IIC_SDA_0(IIC);
        }
        IIC_Delay();
        IIC_SCL_1(IIC);
        IIC_Delay();
        IIC_SCL_0(IIC);
        if (i == 7)
        {
            IIC_SDA_1(IIC); // ÊÍ·Å×ÜÏß
        }
        _ucByte <<= 1;	/* ×óÒÆÒ»¸öbit */
        IIC_Delay();
    }
}

/*
*********************************************************************************************************
*	º¯ Êý Ãû: IIC_ReadByte
*	¹¦ÄÜËµÃ÷: CPU´ÓIIC×ÜÏßÉè±¸¶ÁÈ¡8bitÊý¾Ý
*	ÐÎ    ²Î£ºÎÞ
*	·µ »Ø Öµ: ¶Áµ½µÄÊý¾Ý
*********************************************************************************************************
*/
uint8_t IIC_Read_Byte(IIC_TypeDef* IIC,uint8_t ack)
{
    uint8_t i;
    uint8_t value;

    /* ¶Áµ½µÚ1¸öbitÎªÊý¾ÝµÄbit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        IIC_SCL_1(IIC);
        IIC_Delay();
        if (IIC_SDA_READ(IIC))
        {
            value++;
        }
        IIC_SCL_0(IIC);
        IIC_Delay();
    }
    if(ack==0)
        IIC_NAck(IIC);
    else
        IIC_Ack(IIC);
    return value;
}

/*
*********************************************************************************************************
*	º¯ Êý Ãû: IIC_WaitAck
*	¹¦ÄÜËµÃ÷: CPU²úÉúÒ»¸öÊ±ÖÓ£¬²¢¶ÁÈ¡Æ÷¼þµÄACKÓ¦´ðÐÅºÅ
*	ÐÎ    ²Î£ºÎÞ
*	·µ »Ø Öµ: ·µ»Ø0±íÊ¾ÕýÈ·Ó¦´ð£¬1±íÊ¾ÎÞÆ÷¼þÏìÓ¦
*********************************************************************************************************
*/
uint8_t IIC_Wait_Ack(IIC_TypeDef* IIC)
{
    uint8_t re;

    IIC_SDA_1(IIC);	/* CPUÊÍ·ÅSDA×ÜÏß */
    IIC_Delay();
    IIC_SCL_1(IIC);	/* CPUÇý¶¯SCL = 1, ´ËÊ±Æ÷¼þ»á·µ»ØACKÓ¦´ð */
    IIC_Delay();
    if (IIC_SDA_READ(IIC))	/* CPU¶ÁÈ¡SDA¿ÚÏß×´Ì¬ */
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    IIC_SCL_0(IIC);
    IIC_Delay();
    return re;
}

/*
*********************************************************************************************************
*	º¯ Êý Ãû: IIC_Ack
*	¹¦ÄÜËµÃ÷: CPU²úÉúÒ»¸öACKÐÅºÅ
*	ÐÎ    ²Î£ºÎÞ
*	·µ »Ø Öµ: ÎÞ
*********************************************************************************************************
*/
void IIC_Ack(IIC_TypeDef* IIC)
{
    IIC_SDA_0(IIC);	/* CPUÇý¶¯SDA = 0 */
    IIC_Delay();
    IIC_SCL_1(IIC);	/* CPU²úÉú1¸öÊ±ÖÓ */
    IIC_Delay();
    IIC_SCL_0(IIC);
    IIC_Delay();
    IIC_SDA_1(IIC);	/* CPUÊÍ·ÅSDA×ÜÏß */
}

/*
*********************************************************************************************************
*	º¯ Êý Ãû: IIC_NAck
*	¹¦ÄÜËµÃ÷: CPU²úÉú1¸öNACKÐÅºÅ
*	ÐÎ    ²Î£ºÎÞ
*	·µ »Ø Öµ: ÎÞ
*********************************************************************************************************
*/
void IIC_NAck(IIC_TypeDef* IIC)
{
    IIC_SDA_1(IIC);	/* CPUÇý¶¯SDA = 1 */
    IIC_Delay();
    IIC_SCL_1(IIC);	/* CPU²úÉú1¸öÊ±ÖÓ */
    IIC_Delay();
    IIC_SCL_0(IIC);
    IIC_Delay();
}

/*
*********************************************************************************************************
*	º¯ Êý Ãû: IIC_GPIO_Config
*	¹¦ÄÜËµÃ÷: ÅäÖÃIIC×ÜÏßµÄGPIO£¬²ÉÓÃÄ£ÄâIOµÄ·½Ê½ÊµÏÖ
*	ÐÎ    ²Î£ºÎÞ
*	·µ »Ø Öµ: ÎÞ
*********************************************************************************************************
*/
void IIC_GPIO_Init(IIC_TypeDef* IIC)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		//IIC->RCC_Enable();  // ´ò¿ªGPIOÊ±ÖÓ
		__HAL_RCC_GPIOB_CLK_ENABLE();
		GPIO_InitStructure.Pin = IIC->SCL_Pin | IIC->SDA_Pin;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;  // ¿ªÂ©Êä³ö
		HAL_GPIO_Init(IIC->GPIO_Port, &GPIO_InitStructure);
		
		/* ¸øÒ»¸öÍ£Ö¹ÐÅºÅ, ¸´Î»IIC×ÜÏßÉÏµÄËùÓÐÉè±¸µ½´ý»úÄ£Ê½ */
		IIC_Stop(IIC);

}

/*
*********************************************************************************************************
*	º¯ Êý Ãû: IIC_CheckDevice
*	¹¦ÄÜËµÃ÷: ¼ì²âIIC×ÜÏßÉè±¸£¬CPUÏò·¢ËÍÉè±¸µØÖ·£¬È»ºó¶ÁÈ¡Éè±¸Ó¦´ðÀ´ÅÐ¶Ï¸ÃÉè±¸ÊÇ·ñ´æÔÚ
*	ÐÎ    ²Î£º_Address£ºÉè±¸µÄIIC×ÜÏßµØÖ·
*	·µ »Ø Öµ: ·µ»ØÖµ 0 ±íÊ¾ÕýÈ·£¬ ·µ»Ø1±íÊ¾Î´Ì½²âµ½
*********************************************************************************************************
*/
uint8_t IIC_CheckDevice(IIC_TypeDef* IIC,uint8_t _Address)
{
    uint8_t ucAck;

    IIC_GPIO_Init(IIC);		/* ÅäÖÃGPIO */

    IIC_Start(IIC);		/* ·¢ËÍÆô¶¯ÐÅºÅ */

    /* ·¢ËÍÉè±¸µØÖ·+¶ÁÐ´¿ØÖÆbit£¨0 = w£¬ 1 = r) bit7 ÏÈ´« */
    IIC_Send_Byte(IIC,_Address|IIC_WR);
    ucAck = IIC_Wait_Ack(IIC);	/* ¼ì²âÉè±¸µÄACKÓ¦´ð */

    IIC_Stop(IIC);			/* ·¢ËÍÍ£Ö¹ÐÅºÅ */

    return ucAck;
}

