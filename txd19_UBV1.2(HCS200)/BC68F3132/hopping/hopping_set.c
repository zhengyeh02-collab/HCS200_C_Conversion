#include "BC68F3132.h"
#include "hopping_set.h"
#include "typedef.h"
#include "main.h"

extern uint8 K_R[4];
extern uint8 cSync[2], CRC_8BIT;
// uint32 tSecretKey[2];
uint8 tSecretKey[8];

#if (DISC_LENGTH == 0)
bit sync_overflow;
uint8 ovr;
#endif
// typedef struct {
//	__bits_type Button: 4 ;
//	__bits_type OVR	  : 2 ;
//	__bits_type DISC  : 10;
//	uint8 SYNC  : 16;
// } Encrypted_Portion;
//
// Hopping_data Encrypted_Portion;

//----------------------------------------------------------
// hopping encryption: normal_learn / simple learn
//----------------------------------------------------------
void Hopping_Encryption()
{
#if (PACKET_TYPE == 0)
	LoopCnt_Init(); // loop count
#if (HOPPING_TYPE)
	// Normal_Learn
	HpKey_Init(CK); // crypt key
#else
	// Simple_Learn
	HpKey_Init(MF); // manufacture
#endif
	if (++cSync[0] == 0)
		cSync[1]++;
#else
	/*undetermined*/

	if (++cSync[0] == 0)
		cSync[1]++;

#if (DISC_LENGTH == 0)
	if (sync_overflow && ovr)
	{
		sync_overflow = 0;
		if (ovr & 0x01)
		{
			ovr &= 0x02;
			EEPROM_WR(3, ovr);
		}
		else
		{
			ovr = 0x00;
			EEPROM_WR(3, ovr);
		}
	}
	if (cSync[1] == 255 && cSync[0] == 255)
		sync_overflow = 1;
#endif

	LoopCnt_Init(); // loop count
#if (HOPPING_TYPE)
	// Normal_Learn
	HpKey_Init(CK); // crypt key
#else
	// Simple_Learn
	HpKey_Init(MF); // manufacture
#endif
#endif
	NLFSR_Init(CK); // 4bit key status + 2bit + 10bit disc +16bit sync counter
	Encrypt();		// START Encrypt
	NLFSR_Read();	// read K_R = 32bit hopping code
}

//----------------------------------------------------------
// loopcnt initialization
//----------------------------------------------------------
void LoopCnt_Init()
{
	_loopcnt0 = (uint8)(LOOP_COUNT & 0x00ff);
	_loopcnt1 = (uint8)(LOOP_COUNT >> 8); // LOOP_COUNT
}

//----------------------------------------------------------
// nlf initialization
//----------------------------------------------------------
void NLF_Init()
{
	_nlf3 = (uint8)(NLF >> 24);
	_nlf2 = (uint8)(NLF >> 16);
	_nlf1 = (uint8)(NLF >> 8);
	_nlf0 = (uint8)(NLF);
}

void produceKey()
{
#if (HOPPING_TYPE)
	LoopCnt_Init();	 // loop count
	NLF_Init();		 // NLF
	HpKey_Init(MF);	 // manufacturer
	NLFSR_Init(SN6); // serial number+0x60
	Decrypt();
	NLFSR_Read();
	//	tSecretKey[1]=K_R;		//crypt key low 32bit
	tSecretKey[7] = K_R[3];
	tSecretKey[6] = K_R[2];
	tSecretKey[5] = K_R[1];
	tSecretKey[4] = K_R[0];

	LoopCnt_Init();	 // loop count
	NLF_Init();		 // NLF
	HpKey_Init(MF);	 // manufacturer
	NLFSR_Init(SN2); // serial number+0x20
	Decrypt();
	NLFSR_Read();
	/*	tSecretKey[0]=K_R;		//crypt key high 32bit*/
	tSecretKey[3] = K_R[3];
	tSecretKey[2] = K_R[2];
	tSecretKey[1] = K_R[1];
	tSecretKey[0] = K_R[0];
	HpKey_Init(CK);
#else
	LoopCnt_Init(); // loop count
	NLF_Init();		// NLF
	HpKey_Init(MF); // manufacture
#endif
}

//----------------------------------------------------------
// hpkey initialization
// input : tmp
// tmp=3 : hpkey = crypt key
// other : hpkey = manufacture
//----------------------------------------------------------
void HpKey_Init(uint8 tmp)
{
	if (tmp)
	{
		_hpkey0 = MFG[0];
		_hpkey1 = MFG[1];
		_hpkey2 = MFG[2];
		_hpkey3 = MFG[3];
		_hpkey4 = MFG[4];
		_hpkey5 = MFG[5];
		_hpkey6 = MFG[6];
		_hpkey7 = MFG[7];
	}
	else
	{
		_hpkey0 = (tSecretKey[0]);
		_hpkey1 = (tSecretKey[1]);
		_hpkey2 = (tSecretKey[2]);
		_hpkey3 = (tSecretKey[3]);
		_hpkey4 = (tSecretKey[4]);
		_hpkey5 = (tSecretKey[5]);
		_hpkey6 = (tSecretKey[6]);
		_hpkey7 = (tSecretKey[7]);
	}
}

//----------------------------------------------------------
// nlfsr initialization
// input : tmp
// tmp =1: nlfsr = serial number+0x20
// tmp =2: nlfsr = serial number+0x60
// tmp =0: nlfsr = data
//----------------------------------------------------------
void NLFSR_Init(uint8 tmp)
{
	switch (tmp)
	{
	case 0:
	{
#if (PACKET_TYPE == 0)
		_nlfsr0 = (uint8)cSync[0];
		_nlfsr1 = (uint8)(cSync[1]);
		_nlfsr2 = CRC_8BIT;
		_nlfsr3 = MRFDATA;

#else
		_nlfsr0 = (uint8)cSync[0];
		_nlfsr1 = (uint8)(cSync[1]); // sync counter
		_nlfsr2 = DISC[0];
#if (DISC_LENGTH == 0)
		_nlfsr3 = (DISC[1] & 0x03) + (ovr << 2) + (MRFDATA << 4); // DISC(10bit) + ovr(2bit) + key(4bit)
#else
		_nlfsr3 = (DISC[1] & 0x0f) + (MRFDATA << 4); // DISC(12bit)  + key(4bit)
#endif

#endif
	}
	break;

	case 2:
	{
		_nlfsr3 = (SERIAL_NUMBER[3] | 0x20);
		_nlfsr0 = SERIAL_NUMBER[0];
		_nlfsr1 = SERIAL_NUMBER[1];
		_nlfsr2 = SERIAL_NUMBER[2];
	}
	break;

	case 3:
	{
		_nlfsr3 = (SERIAL_NUMBER[3] | 0x60);
		_nlfsr0 = SERIAL_NUMBER[0];
		_nlfsr1 = SERIAL_NUMBER[1];
		_nlfsr2 = SERIAL_NUMBER[2];
	}
	break;

	default:;
		break;
	}
}

//----------------------------------------------------------
// read K_R
//----------------------------------------------------------
void NLFSR_Read()
{
	K_R[3] = 0;
	K_R[2] = 0;
	K_R[1] = 0;
	K_R[0] = 0;

	K_R[3] = _nlfsr3;
	K_R[2] = _nlfsr2;
	K_R[1] = _nlfsr1;
	K_R[0] = _nlfsr0;
	HP_END; // end keeloq
}

//-----------------------------------------------------------------
void Decrypt()
{
	_encdec = 1;
	_hpeen = 1;
	do
	{
		_clrwdt();
	} while (_hpeen);
}

//-----------------------------------------------------------------
void Encrypt()
{
	_encdec = 0;
	_hpeen = 1;
	do
	{
		_clrwdt();
	} while (_hpeen);
}
