/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	onenet.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-05-08
	*
	*	版本： 		V1.1
	*
	*	说明： 		与onenet平台的数据交互接口层
	*
	*	修改记录：	V1.0：协议封装、返回判断都在同一个文件，并且不同协议接口不同。
	*				V1.1：提供统一接口供应用层使用，根据不同协议文件来封装协议相关的内容。
	************************************************************
	************************************************************
	************************************************************
**/

//单片机头文件
#include "stm32f10x.h"

//网络设备
#include "esp8266.h"

//协议文件
#include "onenet.h"
#include "mqttkit.h"

//硬件驱动
#include "usart.h"
#include "delay.h"
#include "AD.h"


//C库
#include <string.h>
#include <stdio.h>

#include "DHT11.h"
#include "LED.h"

#include "Jdp.h"

#define PROID		"501462"  //产品ID
#define AUTH_INFO	"123" //鉴权信息
#define DEVID		"1072230738"//设备ID


extern unsigned char esp8266_buf[128];

extern u8 Temperature, Humidity; 
extern int LED_state ;

extern int Onenet_flag_1 ;
extern int Onenet_flag_2 ;
extern int Onenet_flag_3 ;

//==========================================================
//	函数名称：	OneNet_DevLink
//
//	函数功能：	与onenet创建连接
//
//	入口参数：	无
//
//	返回参数：	1-成功	0-失败
//
//	说明：		与onenet平台建立连接
//==========================================================
_Bool OneNet_DevLink(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//协议包

	unsigned char *dataPtr;
	
	_Bool status = 1;
	
	UsartPrintf(USART_DEBUG, "OneNet_DevLink\r\n"
							"PROID: %s,	AUIF: %s,	DEVID:%s\r\n"
                        , PROID, AUTH_INFO, DEVID);
	
	if(MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//上传平台
		
		dataPtr = ESP8266_GetIPD(250);									//等待平台响应
		if(dataPtr != NULL)
		{
			if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)
			{
				switch(MQTT_UnPacketConnectAck(dataPtr))
				{
					case 0:UsartPrintf(USART_DEBUG, "Tips:	连接成功\r\n");status = 0;break;
					
					case 1:UsartPrintf(USART_DEBUG, "WARN:	连接失败：协议错误\r\n");break;
					case 2:UsartPrintf(USART_DEBUG, "WARN:	连接失败：非法的clientid\r\n");break;
					case 3:UsartPrintf(USART_DEBUG, "WARN:	连接失败：服务器失败\r\n");break;
					case 4:UsartPrintf(USART_DEBUG, "WARN:	连接失败：用户名或密码错误\r\n");break;
					case 5:UsartPrintf(USART_DEBUG, "WARN:	连接失败：非法链接(比如token非法)\r\n");break;
					
					default:UsartPrintf(USART_DEBUG, "ERR:	连接失败：未知错误\r\n");break;
				}
			}
		}
		
		MQTT_DeleteBuffer(&mqttPacket);								//删包
	}
	else
		UsartPrintf(USART_DEBUG, "WARN:	MQTT_PacketConnect Failed\r\n");
	
	return status;
	
}

unsigned char OneNet_FillBuf(char *buf)
{
	
	char text[32];
	
	DHT11_Read_Data(&Temperature,&Humidity);	//读取温湿度值
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
//	函数名称：	OneNet_SendData
//
//	函数功能：	上传数据到平台
//
//	入口参数：	type：发送数据的格式
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNet_SendData(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};												//协议包
	
	char buf[128];
	
	short body_len = 0, i = 0;
	
//	UsartPrintf(USART_DEBUG, "Tips:	OneNet_SendData-MQTT\r\n");
	
	memset(buf, 0, sizeof(buf));
	
	body_len = OneNet_FillBuf(buf);																	//获取当前需要发送的数据流的总长度
	
//	UsartPrintf(USART_DEBUG, "mqttPacket._data：%s \r\n",mqttPacket._data);
	
	if(body_len)
	{
		if(MQTT_PacketSaveData(DEVID, body_len, NULL, 5, &mqttPacket) == 0)							//封包
		{
			for(; i < body_len; i++)
				mqttPacket._data[mqttPacket._len++] = buf[i];
			
			ESP8266_SendData(mqttPacket._data, mqttPacket._len);									//上传数据到平台
//			UsartPrintf(USART_DEBUG, "Send %d Bytes\r\n", mqttPacket._len);
			
			MQTT_DeleteBuffer(&mqttPacket);															//删包
		}
		else
			UsartPrintf(USART_DEBUG, "WARN:	EDP_NewBuffer Failed\r\n");
	}
	
}

//==========================================================
//	函数名称：	OneNet_RevPro
//
//	函数功能：	平台返回数据检测
//
//	入口参数：	dataPtr：平台返回的数据
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNet_RevPro(unsigned char *cmd)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};								//协议包
	
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
		case MQTT_PKT_CMD:															//命令下发
			
			result = MQTT_UnPacketCmd(cmd, &cmdid_topic, &req_payload, &req_len);	//解出topic和消息体
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

			  UsartPrintf(USART_DEBUG, "温度：%d  湿度：%d\r\n",Temperature,Humidity);
				
				
				
				if(MQTT_PacketCmdResp(cmdid_topic, req_payload, &mqttPacket) == 0)	//命令回复组包
				{
//					UsartPrintf(USART_DEBUG, "Tips:	Send CmdResp\r\n");
//					
//					ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//回复命令
					MQTT_DeleteBuffer(&mqttPacket);									//删包
				}
			}
		
		break;
			
		case MQTT_PKT_PUBACK:														//发送Publish消息，平台回复的Ack
		
			if(MQTT_UnPacketPublishAck(cmd) == 0)
//				UsartPrintf(USART_DEBUG, "Tips:	MQTT Publish Send OK\r\n");
			
		break;
		
		default:
			result = -1;
		break;
	}
	
	ESP8266_Clear();									//清空缓存
	
	if(result == -1)
		return;
	
	dataPtr = strchr(req_payload, '}');					//搜索'}'

	if(dataPtr != NULL && result != -1)					//如果找到了
	{
		dataPtr++;
		
		while(*dataPtr >= '0' && *dataPtr <= '9')		//判断是否是下发的命令控制数据
		{
			numBuf[num++] = *dataPtr++;
		}
		numBuf[num] = 0;
		
		num = atoi((const char *)numBuf);				//转为数值形式
	}

	if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
	{
		MQTT_FreeBuffer(cmdid_topic);
		MQTT_FreeBuffer(req_payload);
	}

}
