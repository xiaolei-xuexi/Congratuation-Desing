#ifndef __MENU_H
#define __MENU_H	 
#include "sys.h"                  // Device header


typedef struct
{
    u8 current;//��ǰ״̬������
	  u8 l;
	  u8 f;
    u8 next; //����һ��
    u8 enter; //ȷ��
    void (*current_operation)(void); //��ǰ״̬Ӧ��ִ�еĲ���
} Menu_table;

void  Menu_yaogan_set(void);
void show0(void);
void show1(void);
void show2(void);
void show3(void);
void show4(void);
void show5(void);
void show6(void);
void show7(void);
void show8(void);

#endif	
