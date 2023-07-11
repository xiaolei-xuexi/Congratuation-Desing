#ifndef __AD_H
#define __AD_H

#define LEFT       400//小于400 左
#define RIGHT      3500//大于2800 右
#define FORWARD    400//小于400 上
#define BANKE      3500//大于3400 下

extern uint16_t AD_Value[4];

void AD_Init(void);

#endif
