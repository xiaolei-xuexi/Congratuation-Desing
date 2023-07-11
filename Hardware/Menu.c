#include "Menu.h"
#include "stm32f10x.h" 
#include "AD.h"
#include "Delay.h"
#include "Key.h"
#include "LED.h"
#include "DHT11.h"
void (*current_operation_index)(); //执行当前操作函数
uint8_t func_index = 0;
uint8_t flag = 0;
u8 Temperature, Humidity; 
u16 M_DHT11_Temp=45;  //温度阈值
u16 M_DHT11_Hum=50;		//湿度阈值
u16 M_mq135_value=1900;	//空气质量阈值
u16 M_fire_value=1500;	//火焰阈值

/*定义了索引数组，索引数组要配合主程序按键值进行选择*/
 Menu_table  table[10]=
{
    {0,0,0,0,1,(*show0)},//一级界面         当前状态  左 上 下 右
    {1,0,4,2,5,(*show1)},//二级界面    第一行
    {2,0,1,3,6,(*show2)},//二级界面    第二行
    {3,0,2,4,7,(*show3)},//二级界面   第三行
    {4,0,3,1,8,(*show4)},
		{5,1,5,5,5,(*show5)},//三级界面第一行
		{6,1,6,6,6,(*show6)},
		{7,1,7,7,7,(*show7)},
		{8,1,8,8,8,(*show8)},
};


void  Menu_yaogan_set(void)
{
	if(AD_Value[1]<LEFT)
  {
		Delay_ms(200);
		
    func_index=table[func_index].l;//按键next按下后的索引号
    OLED_Clear();
  }
	if(AD_Value[0]<FORWARD)
  {
		Delay_ms(200);
		
    func_index=table[func_index].f;//按键next按下后的索引号
    OLED_Clear();
  }

  if(AD_Value[0]>BANKE)
  {   
		Delay_ms(200);
    func_index=table[func_index].next;//按键next按下后的索引号
    OLED_Clear();
  }
  if(AD_Value[1]>RIGHT)
  {
		Delay_ms(200);
    func_index=table[func_index].enter;
    OLED_Clear();
  }

  current_operation_index=table[func_index].current_operation;//执行当前索引号所对应的功能函数。
  (*current_operation_index)();//执行当前操作函数
		
}

void show0(void)//第一页
{

	OLED_ShowChar(1,16,' ');
	OLED_ShowCHinese(1,1,0);//温
  OLED_ShowCHinese(1,3,1);//度
	
	OLED_ShowCHinese(2,1,2);//湿
  OLED_ShowCHinese(2,3,1);//度
	
	OLED_ShowCHinese(3,1,4);//空
	OLED_ShowCHinese(4,1,6);//火
  OLED_ShowChar(3,9,'Y');//y
	
	OLED_ShowChar(4,9,'X');//x
	
	
	DHT11_Read_Data(&Temperature,&Humidity);	//读取温湿度值

	OLED_ShowNum(1,6,Temperature,2);	//显示温度	
	OLED_ShowNum(2,6,Humidity,2);		//显示湿度	 
  Delay_ms(100);
	OLED_ShowNum(3, 4, AD_Value[2], 4);//空
	Delay_ms(100);
	OLED_ShowNum(4, 4, AD_Value[3], 4);//火
	OLED_ShowNum(3, 11, AD_Value[0], 4);//x
	OLED_ShowNum(4, 11, AD_Value[1], 4);//y

}


void show1(void)//第二页
{
	OLED_ShowCHinese(1,1,0);//温
  OLED_ShowCHinese(1,3,1);//度
	
	OLED_ShowCHinese(2,1,2);//湿
  OLED_ShowCHinese(2,3,1);//度
	
	OLED_ShowCHinese(3,1,4);//空
	OLED_ShowCHinese(4,1,6);//火
	OLED_ShowNum(1,6,M_DHT11_Temp,2);
	OLED_ShowNum(2,6,M_DHT11_Hum,2);
	OLED_ShowNum(3, 4, M_mq135_value, 4);
	OLED_ShowNum(4, 4, M_fire_value, 4);
	OLED_ShowString(1,12,"<");


}


void show2(void)
{
	OLED_ShowCHinese(1,1,0);//温
  OLED_ShowCHinese(1,3,1);//度
	OLED_ShowCHinese(2,1,2);//湿
  OLED_ShowCHinese(2,3,1);//度
	OLED_ShowCHinese(3,1,4);//空
	OLED_ShowCHinese(4,1,6);//火
	OLED_ShowNum(1,6,M_DHT11_Temp,2);
	OLED_ShowNum(2,6,M_DHT11_Hum,2);
	OLED_ShowNum(3, 4, M_mq135_value, 4);
	OLED_ShowNum(4, 4, M_fire_value, 4);
	OLED_ShowString(2,12,"<");  
}
void show3(void)
{
	OLED_ShowCHinese(1,1,0);//温
  OLED_ShowCHinese(1,3,1);//度
	OLED_ShowCHinese(2,1,2);//湿
  OLED_ShowCHinese(2,3,1);//度
	OLED_ShowCHinese(3,1,4);//空
	OLED_ShowCHinese(4,1,6);//火
	OLED_ShowNum(1,6,M_DHT11_Temp,2);
	OLED_ShowNum(2,6,M_DHT11_Hum,2);
	OLED_ShowNum(3, 4, M_mq135_value, 4);
	OLED_ShowNum(4, 4, M_fire_value, 4);
	OLED_ShowString(3,12,"<");  
}

void show4(void)
{
	OLED_ShowCHinese(1,1,0);//温
  OLED_ShowCHinese(1,3,1);//度
	OLED_ShowCHinese(2,1,2);//湿
  OLED_ShowCHinese(2,3,1);//度
	OLED_ShowCHinese(3,1,4);//空
	OLED_ShowCHinese(4,1,6);//火
	OLED_ShowNum(1,6,M_DHT11_Temp,2);
	OLED_ShowNum(2,6,M_DHT11_Hum,2);
	OLED_ShowNum(3, 4, M_mq135_value, 4);
	OLED_ShowNum(4, 4, M_fire_value, 4);
	OLED_ShowString(4,12,"<");  
}

void show5(void)//第一页
{
if(flag&&AD_Value[0] >=400&&AD_Value[0]<=3500 )
	{
		
		flag = 0;
	}
	else 
		
	 {
			if(AD_Value[0]<400)
		{
			
			M_DHT11_Temp+=1;
		  Delay_ms(100);
			flag = 1;
		}
		  else if(AD_Value[0]>3500)
		{
			
			M_DHT11_Temp-=1;
		  Delay_ms(100);
			flag = 1;
		}
		
}
	OLED_ShowCHinese(2,2,0);//温
  OLED_ShowCHinese(2,4,1);//度
  OLED_ShowNum(2,7,M_DHT11_Temp,2);
}

void show6(void)//第一页
{
	
	
if(flag&&AD_Value[0] >=400&&AD_Value[0]<=3500 )
	{
		
		flag = 0;
	}
	else 
		
	 {
			if(AD_Value[0]<400)
		{
			
			M_DHT11_Hum+=2;
		  Delay_ms(100);
			flag = 1;
		}
		  else if(AD_Value[0]>3500)
		{
			
			M_DHT11_Hum-=2;
		  Delay_ms(100);
			flag = 1;
		}
		
}
  OLED_ShowCHinese(2,2,2);//湿
  OLED_ShowCHinese(2,4,1);//度
	OLED_ShowNum(2,7,M_DHT11_Hum,2);
}

void show7(void)//第一页
{
	
	
	if(flag&&AD_Value[0] >=400&&AD_Value[0]<=3500 )
	{
		
		flag = 0;
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	}
	else 
		
	 {
			if(AD_Value[0]<400)
		{
			
			M_mq135_value+=50;
		  Delay_ms(100);
			flag = 1;
		}
		  else if(AD_Value[0]>3500)
		{
			
			M_mq135_value-=50;
		  Delay_ms(100);
			flag = 1;
		}
		
}
	OLED_ShowCHinese(2,5,4);//空
	OLED_ShowNum(2,7, M_mq135_value, 4);
}

void show8(void)//第一页
{
	
		if(flag&&AD_Value[0] >=400&&AD_Value[0]<=3500 )
	  {
			flag = 0;
			GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	  }
		else 
		{
			if(AD_Value[0]<400)
		{
			
			M_fire_value+=50;
		  Delay_ms(100);
			flag = 1;
		}
		  else if(AD_Value[0]>3500)
		{
			
			M_fire_value-=50;
		  Delay_ms(100);
			flag = 1;
		}

	}
	OLED_ShowCHinese(2,5,6);//火
	OLED_ShowNum(2,7, M_fire_value, 4);

}

