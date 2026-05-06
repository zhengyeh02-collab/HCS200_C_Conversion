#include "key.h"
#include "app_set.h"

//********************************************************
// F_IO1		==>		PA3
// F_IO2		==>		PA4
// F_IO3		==>		PA5
// F_IO4		==>		PA6
// F_IO5		==>		PA0
// F_IO6		==>		PA2
// F_IO7		==>		PA7
// F_IO8		==>		PA1
// F_IO9		==>		PB7
//********************************************************
uint8 KeyDebounceTime;
uint8 KeyRepeatTime;
uint8 KeyCurrentStatus;
/*uint8 KeyPreviousStatus ;*/
byte_t x;
byte_t Key_IO, Matrix_IO;
bit fkeyrelse;

extern byte_t FLAG1;
void delay();

void Key_To_IO()
{
	Key_IO.bits.b0 = IO5_EN;					  // PA0
	Key_IO.bits.b1 = IO8_EN & (!EXTERNAL_LVD_EN); // PA1
	Key_IO.bits.b2 = IO6_EN;					  // PA2
	Key_IO.bits.b3 = IO1_EN;					  // PA3
	Key_IO.bits.b4 = IO2_EN;					  // PA4
	Key_IO.bits.b5 = IO3_EN;					  // PA5
	Key_IO.bits.b6 = IO4_EN;					  // PA6
	Key_IO.bits.b7 = IO7_EN;					  // PA7
}

void Key_Init()
{
	//	KeyPreviousStatus=0;
	//	KeyCurrentStatus=0;
	fkeyrelse = TRUE;
#if (KEY_MODE == 1)
	//--------------------Direct-------------------------
	Key_To_IO();
	// pa
	_pa = Key_IO.value;
	_pac = Key_IO.value;
	_papu = Key_IO.value; // PA pull up input
	_pawu = Key_IO.value; // PA wake enable
	// pb
	_pb = 0x7e;
	_pbc = 0x7c;
	_pbpu = 0x7c;

	KeyDebounceTime = KEY_CONSISTENCY_DURATION2;

#else
	//--------------------Matrix-------------------------
	// uint8 TempIO;
	Key_To_IO();

	// PA output io	| PA input io  0x78
	// TempIO = ((!IO5_EN<<4)|(!IO6_EN<<2)|(!IO7_EN)|(!IO8_EN<<1)|0x78);
	//	_pac  =  0XFF;
	//	_pa	  =  0XFF;
	////PA wake
	//	_papu = 0XFF;						//PA pull up input
	//	_pawu = 0x78;						//PA3-6 wake enable	 (M1-M4)
	////PB7
	//	if ( ((!LED_EN) & 0X01 ) && IO9_EN )
	//	{
	//		_pbpu7 = 1;
	//		_pbc7  = 1;
	//		_pb7   = 1;
	//	}

	_papu = 0XFF;
	_papu=0xF7;
	_pa = 0;
	_pawu = 0X78;
	if (((!LED_EN) & 0X01) && IO9_EN)
	{
		_pbc7 = 0;
		_pb7 = 0;
		_pbpu7 = 1;
	}

	KeyDebounceTime = KEY_CONSISTENCY_DURATION2;
#endif
}

uint8 CheckKeyStart(void)
{
	uint8 m_buff;
	m_buff = 0;
#if (KEY_MODE == 1)
	//--------------------Direct-------------------------
	x.value = 0x00;
	if ((!M7) && IO7_EN) 
	{
		x.value |= KEY3;
		m_buff = 1;
	}
	if ((!M2) && IO2_EN) 
	{
		x.value |= KEY2;
		m_buff = 1;
	}
	if ((!M3) && IO3_EN)
	{
		x.value |= KEY1;
		m_buff = 1;
	}
	if ((!M4) && IO4_EN)
	{
		x.value |= KEY4;
		m_buff = 1;
	}
	if ((!M5) && IO5_EN)
	{
		x.value |= KEY5;
		m_buff = 1;
	}
	if ((!M6) && IO6_EN)
	{
		x.value |= KEY6;
		m_buff = 1;
	}
	if ((!M3) && IO1_EN)
	{
		x.value |= KEY7;
		m_buff = 1;
	}
	if ((!M8) && IO8_EN)
	{
		x.value |= KEY8;
		m_buff = 1;
	}

	

#else
	//--------------------Matrix-------------------------
	x.value = 0x00;

#if (IO5_EN)
	{
		M5Output0;
		delay();
		if (!M7)
		{
			x.value |= KEY1;
			m_buff = 1;
		}
		if (!M2)
		{
			x.value |= KEY2;
			m_buff = 1;
		}
		if (!M3)
		{
			x.value |= KEY3;
			m_buff = 1;
		}
		if (!M4)
		{
			x.value |= KEY4;
			m_buff = 1;
		}
		M5PullInput;
	}
#endif
#if (IO6_EN)
	{
		M6Output0;
		delay();
		if (!M7)
		{
			x.value |= KEY5;
			m_buff = 1;
		}
		if (!M2)
		{
			x.value |= KEY6;
			m_buff = 1;
		}
		if (!M3)
		{
			x.value |= KEY7;
			m_buff = 1;
		}
		if (!M4)
		{
			x.value |= KEY8;
			m_buff = 1;
		}
		M6PullInput;
	}
#endif
#if (IO1_EN)  //修改處
	{
		M7Output0;
		delay();
		if (!M7)
		{
			x.value |= KEY9;
			m_buff = 1;
		}
		if (!M2)
		{
			x.value |= KEY10;
			m_buff = 1;
		}
		if (!M3)
		{
			x.value |= KEY11;
			m_buff = 1;
		}
		if (!M4)
		{
			x.value |= KEY12;
			m_buff = 1;
		}
		M7PullInput;
	}
#endif

#if (IO8_EN)

	M8Output0;
	delay();
	if (!M7)
	{
		x.value |= KEY13;
		m_buff = 1;
	}
	if (!M2)
	{
		x.value |= KEY14;
		m_buff = 1;
	}
	if (!M3)
	{
		x.value |= KEY15;
		m_buff = 1;
	}
	if (!M4)
	{
		x.value |= KEY16;
		m_buff = 1;
	}
	M8PullInput;

#endif
#if (IO9_EN)
	{
		M9Output0;
		delay();
		if (!M7)
		{
			x.value |= KEY17;
			m_buff = 1;
		}
		if (!M2)
		{
			x.value |= KEY18;
			m_buff = 1;
		}
		if (!M3)
		{
			x.value |= KEY19;
			m_buff = 1;
		}
		if (!M4)
		{
			x.value |= KEY20;
			m_buff = 1;
		}
		M9PullInput;
	}
#endif
#endif
	return m_buff;
}

uint8 CheckKeyStatus(void)
{
	uint8 m_buff1;

	m_buff1 = CheckKeyStart();
	if (m_buff1)
	{

		if ((x.value ^ KeyCurrentStatus) || fkeyrelse)
		{
			KeyCurrentStatus = x.value;
			if (FLAG1.bits.b5)
				KeyDebounceTime = KEY_CONSISTENCY_DURATION;
			else
				KeyDebounceTime = KEY_CONSISTENCY_DURATION2;
			fkeyrelse = FALSE;
			return 0;
		}

		else
		{
			if (KeyDebounceTime)
			{
				KeyDebounceTime--;
			}
			else
				return 1;
		}
	}
	else
		fkeyrelse = TRUE;

	return 0;
}




void delay()
{
	uint8 i;
	for (i = 0; i < 10; i++)
		_nop();
}