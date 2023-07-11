#include "Menu.h"
#include "stm32f10x.h" 
#include "AD.h"
#include "Delay.h"
#include "Key.h"
#include "LED.h"
#include "DHT11.h"
void (*current_operation_index)(); //ִ�е�ǰ��������
uint8_t func_index = 0;
uint8_t flag = 0;
u8 Temperature, Humidity; 
u16 M_DHT11_Temp=45;  //�¶���ֵ
u16 M_DHT11_Hum=50;		//ʪ����ֵ
u16 M_mq135_value=1900;	//����������ֵ
u16 M_fire_value=1500;	//������ֵ

/*�������������飬��������Ҫ��������򰴼�ֵ����ѡ��*/
 Menu_table  table[10]=
{
    {0,0,0,0,1,(*show0)},//һ������         ��ǰ״̬  �� �� �� ��
    {1,0,4,2,5,(*show1)},//��������    ��һ��
    {2,0,1,3,6,(*show2)},//��������    �ڶ���
    {3,0,2,4,7,(*show3)},//��������   ������
    {4,0,3,1,8,(*show4)},
		{5,1,5,5,5,(*show5)},//���������һ��
		{6,1,6,6,6,(*show6)},
		{7,1,7,7,7,(*show7)},
		{8,1,8,8,8,(*show8)},
};


void  Menu_yaogan_set(void)
{
	if(AD_Value[1]<LEFT)
  {
		Delay_ms(200);
		
    func_index=table[func_index].l;//����next���º��������
    OLED_Clear();
  }
	if(AD_Value[0]<FORWARD)
  {
		Delay_ms(200);
		
    func_index=table[func_index].f;//����next���º��������
    OLED_Clear();
  }

  if(AD_Value[0]>BANKE)
  {   
		Delay_ms(200);
    func_index=table[func_index].next;//����next���º��������
    OLED_Clear();
  }
  if(AD_Value[1]>RIGHT)
  {
		Delay_ms(200);
    func_index=table[func_index].enter;
    OLED_Clear();
  }

  current_operation_index=table[func_index].current_operation;//ִ�е�ǰ����������Ӧ�Ĺ��ܺ�����
  (*current_operation_index)();//ִ�е�ǰ��������
		
}

void show0(void)//��һҳ
{

	OLED_ShowChar(1,16,' ');
	OLED_ShowCHinese(1,1,0);//��
  OLED_ShowCHinese(1,3,1);//��
	
	OLED_ShowCHinese(2,1,2);//ʪ
  OLED_ShowCHinese(2,3,1);//��
	
	OLED_ShowCHinese(3,1,4);//��
	OLED_ShowCHinese(4,1,6);//��
  OLED_ShowChar(3,9,'Y');//y
	
	OLED_ShowChar(4,9,'X');//x
	
	
	DHT11_Read_Data(&Temperature,&Humidity);	//��ȡ��ʪ��ֵ

	OLED_ShowNum(1,6,Temperature,2);	//��ʾ�¶�	
	OLED_ShowNum(2,6,Humidity,2);		//��ʾʪ��	 
  Delay_ms(100);
	OLED_ShowNum(3, 4, AD_Value[2], 4);//��
	Delay_ms(100);
	OLED_ShowNum(4, 4, AD_Value[3], 4);//��
	OLED_ShowNum(3, 11, AD_Value[0], 4);//x
	OLED_ShowNum(4, 11, AD_Value[1], 4);//y

}


void show1(void)//�ڶ�ҳ
{
	OLED_ShowCHinese(1,1,0);//��
  OLED_ShowCHinese(1,3,1);//��
	
	OLED_ShowCHinese(2,1,2);//ʪ
  OLED_ShowCHinese(2,3,1);//��
	
	OLED_ShowCHinese(3,1,4);//��
	OLED_ShowCHinese(4,1,6);//��
	OLED_ShowNum(1,6,M_DHT11_Temp,2);
	OLED_ShowNum(2,6,M_DHT11_Hum,2);
	OLED_ShowNum(3, 4, M_mq135_value, 4);
	OLED_ShowNum(4, 4, M_fire_value, 4);
	OLED_ShowString(1,12,"<");


}


void show2(void)
{
	OLED_ShowCHinese(1,1,0);//��
  OLED_ShowCHinese(1,3,1);//��
	OLED_ShowCHinese(2,1,2);//ʪ
  OLED_ShowCHinese(2,3,1);//��
	OLED_ShowCHinese(3,1,4);//��
	OLED_ShowCHinese(4,1,6);//��
	OLED_ShowNum(1,6,M_DHT11_Temp,2);
	OLED_ShowNum(2,6,M_DHT11_Hum,2);
	OLED_ShowNum(3, 4, M_mq135_value, 4);
	OLED_ShowNum(4, 4, M_fire_value, 4);
	OLED_ShowString(2,12,"<");  
}
void show3(void)
{
	OLED_ShowCHinese(1,1,0);//��
  OLED_ShowCHinese(1,3,1);//��
	OLED_ShowCHinese(2,1,2);//ʪ
  OLED_ShowCHinese(2,3,1);//��
	OLED_ShowCHinese(3,1,4);//��
	OLED_ShowCHinese(4,1,6);//��
	OLED_ShowNum(1,6,M_DHT11_Temp,2);
	OLED_ShowNum(2,6,M_DHT11_Hum,2);
	OLED_ShowNum(3, 4, M_mq135_value, 4);
	OLED_ShowNum(4, 4, M_fire_value, 4);
	OLED_ShowString(3,12,"<");  
}

void show4(void)
{
	OLED_ShowCHinese(1,1,0);//��
  OLED_ShowCHinese(1,3,1);//��
	OLED_ShowCHinese(2,1,2);//ʪ
  OLED_ShowCHinese(2,3,1);//��
	OLED_ShowCHinese(3,1,4);//��
	OLED_ShowCHinese(4,1,6);//��
	OLED_ShowNum(1,6,M_DHT11_Temp,2);
	OLED_ShowNum(2,6,M_DHT11_Hum,2);
	OLED_ShowNum(3, 4, M_mq135_value, 4);
	OLED_ShowNum(4, 4, M_fire_value, 4);
	OLED_ShowString(4,12,"<");  
}

void show5(void)//��һҳ
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
	OLED_ShowCHinese(2,2,0);//��
  OLED_ShowCHinese(2,4,1);//��
  OLED_ShowNum(2,7,M_DHT11_Temp,2);
}

void show6(void)//��һҳ
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
  OLED_ShowCHinese(2,2,2);//ʪ
  OLED_ShowCHinese(2,4,1);//��
	OLED_ShowNum(2,7,M_DHT11_Hum,2);
}

void show7(void)//��һҳ
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
	OLED_ShowCHinese(2,5,4);//��
	OLED_ShowNum(2,7, M_mq135_value, 4);
}

void show8(void)//��һҳ
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
	OLED_ShowCHinese(2,5,6);//��
	OLED_ShowNum(2,7, M_fire_value, 4);

}

