#ifndef _KEY_H_
#define _KEY_H_

#include "key.h"
#include "ui.h"
#include "typedef.h"
#include "BC68F3132.h"


enum
{
	KEY1_D=01,
	KEY2_D,
	KEY3_D,
	KEY4_D,
	KEY5_D,
	KEY6_D,
	KEY7_D,
	KEY8_D,
	KEY9_D,
	KEY10_D,
	KEY11_D,
	KEY12_D,
	KEY13_D,
	KEY14_D,
	KEY15_D,
	KEY16_D,
	KEY17_D,
	KEY18_D,
	KEY19_D,
	KEY20_D
};

//********************************************************
//For UI
#define				IO1_EN							APP_IO1_EN	
#define				IO2_EN							APP_IO2_EN		
#define				IO3_EN							APP_IO3_EN
#define				IO4_EN							APP_IO4_EN
#define				IO5_EN							APP_IO5_EN
#define				IO6_EN							APP_IO6_EN
#define				IO7_EN							APP_IO7_EN					
#define				IO8_EN							APP_IO8_EN					//1:enable IOx			0:disable IOx
#define				IO9_EN							APP_IO9_EN					//turn off the external LVD/LED when using IO8&IO9

#define				KEY_MODE						APP_KEY_MODE				

#define				KEY1							APP_KEY1
#define				KEY2							APP_KEY2
#define				KEY3							APP_KEY3
#define				KEY4							APP_KEY4
#define				KEY5							APP_KEY5
#define				KEY6							APP_KEY6
#define				KEY7							APP_KEY7
#define				KEY8							APP_KEY8
#define				KEY9							APP_KEY9
#define				KEY10							APP_KEY10
#define				KEY11							APP_KEY11
#define				KEY12							APP_KEY12
#define				KEY13							APP_KEY13
#define				KEY14							APP_KEY14
#define				KEY15							APP_KEY15
#define				KEY16							APP_KEY16
#define				KEY17							APP_KEY17
#define				KEY18							APP_KEY18
#define				KEY19							APP_KEY19
#define				KEY20							APP_KEY20					//Key.Value

//********************************************************

#define  			M1								_pa3
#define  			M2								_pa4
#define  			M3								_pa5
#define  			M4								_pa6
#define  			M5								_pa0
#define  			M6								_pa2
#define  			M7								_pa7
#define  			M8								_pa1

#define  			M5Output0						{_pac0=0;_pa0=0;}
#define  			M6Output0						{_pac2=0;_pa2=0;}
#define  			M7Output0						{_pac7=0;_pa7=0;}
#define  			M8Output0						{_pac1=0;_pa1=0;}
#define  			M9Output0						{_pbc7=0;_pb7=0;}

#define				M5PullInput					_pac0 = 1;
#define				M6PullInput					_pac2 = 1;
#define				M7PullInput					_pac7 = 1;
#define				M8PullInput					_pac1 = 1;
#define				M9PullInput					_pbc7 = 1;

#define  			KEY_CONSISTENCY_DURATION   		15    //4
#define  			KEY_CONSISTENCY_DURATION2   		40 

#define             ENABLE_KEY    1
#define             DisableKey    0

void Key_Init();
void Key_To_IO();
uint8 CheckKeyStart(void);
uint8 CheckKeyStatus(void);

#endif // _KEY_H_