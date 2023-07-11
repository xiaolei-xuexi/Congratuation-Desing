/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	onenet.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-05-08
	*
	*	�汾�� 		V1.1
	*
	*	˵���� 		��onenetƽ̨�����ݽ����ӿڲ�
	*
	*	�޸ļ�¼��	V1.0��Э���װ�������ж϶���ͬһ���ļ������Ҳ�ͬЭ��ӿڲ�ͬ��
	*				V1.1���ṩͳһ�ӿڹ�Ӧ�ò�ʹ�ã����ݲ�ͬЭ���ļ�����װЭ����ص����ݡ�
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//�����豸
#include "esp8266.h"

//Э���ļ�
#include "onenet.h"
#include "mqttkit.h"

//Ӳ������
#include "usart.h"
#include "delay.h"
#include "AD.h"


//C��
#include <string.h>
#include <stdio.h>

#include "DHT11.h"
#include "LED.h"

#include "Jdp.h"

#define PROID		"501462"  //��ƷID
#define AUTH_INFO	"123" //��Ȩ��Ϣ
#define DEVID		"1072230738"//�豸ID


extern unsigned char esp8266_buf[128];

extern u8 Temperature, Humidity; 
extern int LED_state ;

extern int Onenet_flag_1 ;
extern int Onenet_flag_2 ;
extern int Onenet_flag_3 ;

//==========================================================
//	�������ƣ�	OneNet_DevLink
//
//	�������ܣ�	��onenet��������
//
//	��ڲ�����	��
//
//	���ز�����	1-�ɹ�	0-ʧ��
//
//	˵����		��onenetƽ̨��������
//==========================================================
_Bool OneNet_DevLink(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//Э���

	unsigned char *dataPtr;
	
	_Bool status = 1;
	
	UsartPrintf(USART_DEBUG, "OneNet_DevLink\r\n"
							"PROID: %s,	AUIF: %s,	DEVID:%s\r\n"
                        , PROID, AUTH_INFO, DEVID);
	
	if(MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//�ϴ�ƽ̨
		
		dataPtr = ESP8266_GetIPD(250);									//�ȴ�ƽ̨��Ӧ
		if(dataPtr != NULL)
		{
			if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)
			{
				switch(MQTT_UnPacketConnectAck(dataPtr))
				{
					case 0:UsartPrintf(USART_DEBUG, "Tips:	���ӳɹ�\r\n");status = 0;break;
					
					case 1:UsartPrintf(USART_DEBUG, "WARN:	����ʧ�ܣ�Э�����\r\n");break;
					case 2:UsartPrintf(USART_DEBUG, "WARN:	����ʧ�ܣ��Ƿ���clientid\r\n");break;
					case 3:UsartPrintf(USART_DEBUG, "WARN:	����ʧ�ܣ�������ʧ��\r\n");break;
					case 4:UsartPrintf(USART_DEBUG, "WARN:	����ʧ�ܣ��û������������\r\n");break;
					case 5:UsartPrintf(USART_DEBUG, "WARN:	����ʧ�ܣ��Ƿ�����(����token�Ƿ�)\r\n");break;
					
					default:UsartPrintf(USART_DEBUG, "ERR:	����ʧ�ܣ�δ֪����\r\n");break;
				}
			}
		}
		
		MQTT_DeleteBuffer(&mqttPacket);								//ɾ��
	}
	else
		UsartPrintf(USART_DEBUG, "WARN:	MQTT_PacketConnect Failed\r\n");
	
	return status;
	
}

unsigned char OneNet_FillBuf(char *buf)
{
	
	char text[32];
	
	DHT11_Read_Data(&Temperature,&Humidity);	//��ȡ��ʪ��ֵ
	memset(text, 0, sizeof(text));
	strcpy(buf, ",;");
	
	memset(text, 0, sizeof(text));
	sprintf(text, "Tempreture,%d;", Temperature);
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "Humidity,%d;", Humidity);
	strcat(buf, text);
	
		
	memset(text, 0, sizeof(text));
	sprintf(text, "LED,%d;", LED_state);
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "MQ-135,%d;", AD_Value[2]);
	strcat(buf, text);
	
		memset(text, 0, sizeof(text));
	sprintf(text, "FIRE,%d;", AD_Value[3]);
	strcat(buf, text);
	
	
			memset(text, 0, sizeof(text));
	sprintf(text, "Y,%d;", AD_Value[0]);
	strcat(buf, text);
	
	
			memset(text, 0, sizeof(text));
	sprintf(text, "X,%d;", AD_Value[1]);
	strcat(buf, text);
	

	
	
	return strlen(buf);

}

//==========================================================
//	�������ƣ�	OneNet_SendData
//
//	�������ܣ�	�ϴ����ݵ�ƽ̨
//
//	��ڲ�����	type���������ݵĸ�ʽ
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void OneNet_SendData(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};												//Э���
	
	char buf[128];
	
	short body_len = 0, i = 0;
	
//	UsartPrintf(USART_DEBUG, "Tips:	OneNet_SendData-MQTT\r\n");
	
	memset(buf, 0, sizeof(buf));
	
	body_len = OneNet_FillBuf(buf);																	//��ȡ��ǰ��Ҫ���͵����������ܳ���
	
//	UsartPrintf(USART_DEBUG, "mqttPacket._data��%s \r\n",mqttPacket._data);
	
	if(body_len)
	{
		if(MQTT_PacketSaveData(DEVID, body_len, NULL, 5, &mqttPacket) == 0)							//���
		{
			for(; i < body_len; i++)
				mqttPacket._data[mqttPacket._len++] = buf[i];
			
			ESP8266_SendData(mqttPacket._data, mqttPacket._len);									//�ϴ����ݵ�ƽ̨
//			UsartPrintf(USART_DEBUG, "Send %d Bytes\r\n", mqttPacket._len);
			
			MQTT_DeleteBuffer(&mqttPacket);															//ɾ��
		}
		else
			UsartPrintf(USART_DEBUG, "WARN:	EDP_NewBuffer Failed\r\n");
	}
	
}

//==========================================================
//	�������ƣ�	OneNet_RevPro
//
//	�������ܣ�	ƽ̨�������ݼ��
//
//	��ڲ�����	dataPtr��ƽ̨���ص�����
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void OneNet_RevPro(unsigned char *cmd)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};								//Э���
	
	char *req_payload = NULL;
	char *cmdid_topic = NULL;
	
	unsigned short req_len = 0;
	
	unsigned char type = 0;
	
	short result = 0;

	char *dataPtr = NULL;
	char numBuf[10];
	int num = 0;
	
	type = MQTT_UnPacketRecv(cmd);
	switch(type)
	{
		case MQTT_PKT_CMD:															//�����·�
			
			result = MQTT_UnPacketCmd(cmd, &cmdid_topic, &req_payload, &req_len);	//���topic����Ϣ��
			if(result == 0)
			{
				UsartPrintf(USART_DEBUG, "cmdid: %s, req: %s, req_len: %d\r\n", cmdid_topic, req_payload, req_len);
		    UsartPrintf(USART_DEBUG, "cmdid: %s, req: %s, req_len: %d\r\n", cmdid_topic, req_payload, req_len);
				
				if(strstr(req_payload,"switch1:1") != NULL )
				{
					Onenet_flag_1 = 1;
					UsartPrintf(USART_DEBUG, "LED1_ON! \r\n");
					LED1_ON();
				}
				else if(strstr(req_payload,"switch1:0") != NULL)
				{
					
					UsartPrintf(USART_DEBUG, "LED1_OFF! \r\n");
					LED1_OFF();
					Onenet_flag_1 = 0;
				}
				
				//
				if(strstr(req_payload,"switch2:1") != NULL )
				{
					Onenet_flag_2 = 1;
					UsartPrintf(USART_DEBUG, "Jdp_pwm_on! \r\n");
					Jdp_pwm_on();
				}
				else if(strstr(req_payload,"switch2:0") != NULL)
				{
					
					UsartPrintf(USART_DEBUG, "Jdp_pwm_off! \r\n");
					Jdp_pwm_off();
					Onenet_flag_2 = 0;
				}
				
				//
				if(strstr(req_payload,"switch3:1") != NULL )
				{
					Onenet_flag_3 = 1;
					UsartPrintf(USART_DEBUG, "Jdp_jr_on! \r\n");
					Jdp_jr_on();
				}
				else if(strstr(req_payload,"switch3:0") != NULL)
				{
					
					UsartPrintf(USART_DEBUG, "Jdp_jr_off! \r\n");
					Jdp_jr_off();
					Onenet_flag_3 = 0;
				}

			  UsartPrintf(USART_DEBUG, "�¶ȣ�%d  ʪ�ȣ�%d\r\n",Temperature,Humidity);
				
				
				
				if(MQTT_PacketCmdResp(cmdid_topic, req_payload, &mqttPacket) == 0)	//����ظ����
				{
//					UsartPrintf(USART_DEBUG, "Tips:	Send CmdResp\r\n");
//					
//					ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//�ظ�����
					MQTT_DeleteBuffer(&mqttPacket);									//ɾ��
				}
			}
		
		break;
			
		case MQTT_PKT_PUBACK:														//����Publish��Ϣ��ƽ̨�ظ���Ack
		
			if(MQTT_UnPacketPublishAck(cmd) == 0)
//				UsartPrintf(USART_DEBUG, "Tips:	MQTT Publish Send OK\r\n");
			
		break;
		
		default:
			result = -1;
		break;
	}
	
	ESP8266_Clear();									//��ջ���
	
	if(result == -1)
		return;
	
	dataPtr = strchr(req_payload, '}');					//����'}'

	if(dataPtr != NULL && result != -1)					//����ҵ���
	{
		dataPtr++;
		
		while(*dataPtr >= '0' && *dataPtr <= '9')		//�ж��Ƿ����·��������������
		{
			numBuf[num++] = *dataPtr++;
		}
		numBuf[num] = 0;
		
		num = atoi((const char *)numBuf);				//תΪ��ֵ��ʽ
	}

	if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
	{
		MQTT_FreeBuffer(cmdid_topic);
		MQTT_FreeBuffer(req_payload);
	}

}
