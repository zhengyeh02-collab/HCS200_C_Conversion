#ifndef _APP_SET_H_
#define _APP_SET_H_

#include "app_Set.h"
#include "ui.h"



#define				LED_EN					APP_LED_EN
#define				MCU_PACKAGE				APP_MCU_PACKAGE
#define				EXTERNAL_LVD_EN			APP_EXTERNAL_LVD_EN
#define				INTERNAL_LVD_EN			APP_INTERNAL_LVD_EN	
#define				LVD_VOL					APP_INTERNAL_LVD


#define				LED_ON					{if(LED_EN) {_pb7  = 1;}}
#define				LED_OFF					{if(LED_EN) {_pb7  = 0;}}


void LVD_Init(void);
void LVDC_Init(void);
void LED_Init(void);


#endif // _APP_SET_H_