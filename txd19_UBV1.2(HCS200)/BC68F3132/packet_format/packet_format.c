#include "packet_format.h"
#include "typedef.h"

extern uint8 MFG[8],MRFDATA,SERIAL_NUMBER[4];
#if(PACKET_TYPE== 0)

	bit f_Encryp_CRC_init;
	void CRC_8(uint8 mData);
	uint8 mCRCData;
	extern uint8 CRC_8BIT;
	extern uint8 cSync[2];
	extern uint8 MRFDATA;
	
	uint8 Serial_CRC()
	{
		mCRCData=0;
		CRC_8(SERIAL_NUMBER[0]);
		CRC_8(SERIAL_NUMBER[1]);
		CRC_8(SERIAL_NUMBER[2]);
		CRC_8(SERIAL_NUMBER[3]);	
		return mCRCData;
	}
	
	uint8 Encryp_CRC()
	{
		mCRCData=0;
		CRC_8(cSync[0]);
		CRC_8(cSync[1]);
		CRC_8(CRC_8BIT);
		CRC_8(MRFDATA);
		CRC_8(SERIAL_NUMBER[0]);
		CRC_8(SERIAL_NUMBER[1]);	
		CRC_8(SERIAL_NUMBER[2]);
		CRC_8(SERIAL_NUMBER[3]);
		mCRCData &=0x0F;	
		return mCRCData;
	}
	
	
	
	void CRC_8(uint8 mData)
	{
		GCC_MOVA(mData);
		GCC_XORM(mCRCData);
		ASM_CLR(_acc);
		BIT0SZ(mCRCData);  
		asm volatile("xor  A,94");
		
		BIT1SZ(mCRCData);  
		asm volatile("xor  A,188");
		
		BIT2SZ(mCRCData);
		asm volatile("xor  A,061H");
		
		BIT3SZ(mCRCData);  
		asm volatile("xor  A,0C2H");
		
		BIT4SZ(mCRCData);  
		asm volatile("xor  A,09DH");
		
		BIT5SZ(mCRCData);  
		asm volatile("xor  A,023H");
		
		BIT6SZ(mCRCData);  
		asm volatile("xor  A,046H");
		
		BIT7SZ(mCRCData);  
		asm volatile("xor  A,08CH");
		GCC_MOVM(mCRCData);
	}

#endif