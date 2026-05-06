#ifndef _HOPPING_SET_H_
#define _HOPPING_SET_H_

#include "hopping_set.h"
#include "typedef.h"
#include "ui.h"

#define			PACKET_TYPE			HOP_PACKET_TYPE				
#define			HOPPING_TYPE  		HOP_HOPPING_TYPE
#define			LOOP_COUNT			HOP_LOOP_COUNT
#define			NLF					HOP_NLF
#define			CRYPT_KEY			HOP_CRYPT_KEY				
#define			SYNC_COUNTER		HOP_SYNC_COUNTER			
#define			OVR_TYPE			HOP_OVR_TYPE
#define			DISC_TYPE			HOP_DISC_TYPE
#define			DISC_LENGTH			HOP_DISC_LENGTH


//#define			HP_START			{_encdec =1;_hpeen = 1;}
//#define			Decrypt_START		{_encdec =0;_hpeen = 1;}
#define			HP_END				{_hpeinte =0;_hpeen = 0;}

#define			CK					0
#define			MF					1
#define			SN2					2
#define			SN6					3

void Hopping_Encryption();
void LoopCnt_Init();
void HpKey_Init(uint8 tmp);
void NLF_Init();
void NLFSR_Init(uint8 tmp);
void NLFSR_Read();
void Encrypt();
void Decrypt();

#endif // _HOPPING_SET_H_