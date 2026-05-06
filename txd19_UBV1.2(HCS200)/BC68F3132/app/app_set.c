#include "app_set.h"
#include "BC68F3132.h"

void LVD_Init(void)
{
//incomplete
#if(EXTERNAL_LVD_EN==1)
	_pas03=0;
	_pas02=1;			//LVD_IN
	
	_lvdc = 0X17;		//EXTERNAL LVD :VLVD2~VLVD0 MUST 111B 
			
	LVDC_Init();
#else
	_pas03=0;
	_pas02=0;			//IO8 
#endif		

#if(INTERNAL_LVD_EN)
	LVDC_Init();	
	_lvden	=1;
#endif		
}

void LVDC_Init()
{
	/*undetermined*/
//	if(LVD_VOL==0) 
//	if(LVD_VOL==1) _lvdc |=	0x02;		//2.2V
//	if(LVD_VOL==2) _lvdc |=	0x03;		//2.4V
//	if(LVD_VOL==3) _lvdc |=	0x04;		//2.7V
#if(LVD_VOL== 0)
_lvdc |=	0x00;		//2.0V
#endif
#if(LVD_VOL== 1)
_lvdc |=	0x01;		//2.2V
#endif
#if(LVD_VOL== 2)
_lvdc |=	0x02;		//2.4V
#endif
#if(LVD_VOL== 3)
_lvdc |=	0x03;		//2.7V
#endif


}


void LED_Init(void)
{
#if(LED_EN)
	
		_pb7   = 0;
		_pbc7  = 0;	
		_pbpu7 = 0;
		
#else
	
		_pb7   = 1;
		_pbc7  = 1;	
		_pbpu7 = 1;
	
#endif


}
