#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "AD.h"
#include "DHT11.h"
#include "Delay.h"
//extern DHT11_Data_TypeDef DHT11_Data;

void CountSensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //rcc开启GPIO和AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;//GPIO结构体
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);//PB11的第11个中断线路
	
	EXTI_InitTypeDef EXTI_InitStructure;//外部中断
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;//指定配置的中断线
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组，优先级
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//响应优先级
	NVIC_Init(&NVIC_InitStructure);
}


//void EXTI15_10_IRQHandler(void)
//{
	
//	if (EXTI_GetITStatus(EXTI_Line11) == SET)
//	{
//			DHT11_Data.temp_int=40;
//			DHT11_Data.temp_deci=0;
//			DHT11_Data.humi_int=50;
//			DHT11_Data.humi_deci=0;

//		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
//		{
//			Delay_ms(20);
//			OLED_ShowCHinese(1,1,0);//温
//			OLED_ShowCHinese(1,3,1);//度
//			OLED_ShowCHinese(2,1,2);//湿
//			OLED_ShowCHinese(2,3,1);//度
//			OLED_ShowCHinese(3,1,4);//空
//			OLED_ShowCHinese(3,3,5);//气
//			OLED_ShowCHinese(4,1,6);//火
//			OLED_ShowCHinese(4,3,7);//焰
//			OLED_ShowChar(2,8,'.');
//			OLED_ShowChar(1,8,'.');
//			OLED_ShowNum(1,6,DHT11_Data.temp_int,2);
//			OLED_ShowNum(1,9,DHT11_Data.temp_deci,2);
//			OLED_ShowNum(2,6,DHT11_Data.humi_int,2);
//			OLED_ShowNum(2,9,DHT11_Data.humi_deci,2);
//			OLED_ShowNum(3, 6, AD_Value[2], 4);
//			OLED_ShowNum(4, 6, AD_Value[3], 4);
//		
//		}
//		EXTI_ClearITPendingBit(EXTI_Line11);//清楚中断标志位
//	}
//}
