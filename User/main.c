#include "stm32f10x.h"                 
#include "Delay.h"
#include "OLED.h"
#include "DHT11.h"
#include "Buzzer.h"
#include "AD.h"
#include "LED.h"
#include "Menu.h"
#include "Jdp.h"
#include "onenet.h"
#include "esp8266.h"
#include "usart.h"
#include <string.h>
extern u8 Temperature, Humidity; 
extern u16 M_DHT11_Temp;  //�¶���ֵ
extern u16 M_DHT11_Hum;		//ʪ����ֵ
extern u16 M_mq135_value;	//����������ֵ
extern u16 M_fire_value;	//������ֵ

int Onenet_flag_1 =0;
int Onenet_flag_2 =0;
int Onenet_flag_3 =0;

int LED_state = 0;
int ONE_state = 0 ;
void Hardware_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�жϿ�������������
	Jdp_init();
	OLED_Init();
	Buzzer_Init();
	AD_Init();
	LED_Init();
	Usart1_Init(115200);							//����1����ӡ��Ϣ��
	Usart2_Init(115200);							//����2������ESP8266��
	//UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
}
int main(void)
{

	unsigned short timeCount = 0;	//���ͼ������
	
	unsigned char *dataPtr = NULL;
	
	Hardware_Init();				//��ʼ����ΧӲ��
	
	ESP8266_Init();					//��ʼ��ESP8266
	while(OneNet_DevLink())			//����OneNET
	Delay_ms(100);
	
	Buzzer_ON();			//������ʾ����ɹ�
	Delay_ms(250);
	Buzzer_OFF();
	while(1)
	{	
		Menu_yaogan_set();

		if(Humidity>M_DHT11_Hum)//ʪ��
		{
			if(Onenet_flag_3 == 0){
				Jdp_jr_on();
			}
		}
		else
		{
			if(Onenet_flag_3 == 0){
				Jdp_jr_off();
			}
		}
		if(AD_Value[2]>M_mq135_value)//��
		{
			LED_state = 1;
			
			if(Onenet_flag_1 == 0){
				LED1_ON();
			}
			
			if(Onenet_flag_2 == 0){
				Jdp_pwm_on();
			}
		}
		else
		{
			LED_state = 0;
			
			if(Onenet_flag_1 == 0){
				LED1_OFF();
			}
			
			if(Onenet_flag_2 == 0){
				Jdp_pwm_off();
			}
		}
			
		if(Temperature>=M_DHT11_Temp|| AD_Value[3]<M_fire_value)//��
		{
			
			Buzzer_ON();

		}
		else
		{
			Buzzer_OFF();

		}
		//������ʾ�¶�
		DHT11_Read_Data(&Temperature,&Humidity);	//��ȡ��ʪ��ֵ
//		UsartPrintf(USART_DEBUG, "�¶ȣ�%d  ʪ�ȣ�%d\r\n",Temperature,Humidity);
				
		
		if(++timeCount >= 4)									//���ͼ��
		{
			//UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");
			
			OneNet_SendData();									//��������
			
			timeCount = 0;
			ESP8266_Clear();
		}
		
		dataPtr = ESP8266_GetIPD(3);
		if(dataPtr != NULL){
			OneNet_RevPro(dataPtr);
		}
		Delay_ms(10);//10
    }

}



