
#include "stm32f10x.h"    
void Jdp_init(void)//对继电器初始化，即对PB7口初始化
{
  GPIO_InitTypeDef     GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}          
 
void Jdp_pwm_on(void)//PB7置1
{
  GPIO_SetBits(GPIOB,GPIO_Pin_7);
}
 
void Jdp_pwm_off(void)//PB7置0
{ 
  GPIO_ResetBits(GPIOB,GPIO_Pin_7);
}

void Jdp_jr_on(void)//PB6置1
{
  GPIO_SetBits(GPIOB,GPIO_Pin_6);
}
 
void Jdp_jr_off(void)//PB6置0
{ 
  GPIO_ResetBits(GPIOB,GPIO_Pin_6);
}

