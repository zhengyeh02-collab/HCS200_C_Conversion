//*************************************************************************************
// filename		: main.c
// description	: BC68F3132 hopping code
// created by	: ***
// created date	: ***
// modify by	: zdjiang
// modify date	: 2023/03/30
// version		: 1.2.0
//*************************************************************************************
// CTM0         : for TX encoding
// CTM1         : for BACW
// TB0          : time base 0, occurs every 2ms, for key de-bounce
//*************************************************************************************
//	|   IO    |  Driect |	Matrix	|  Other	|
//  ---------------------------------------------
// BC68F3132
//	|	PA0   |   KEY5 	| output IO	|  OCDSDA	|
//	|	PA1   |   KEY8 	| output IO	|  LVD		|
//	|	PA2   |   KEY6 	| output IO	|  OCDSCK	|
//	|	PA3	  |	  KEY1	| input IO	|			|
//	|	PA4   |   KEY2	| input IO	|			|
//	|	PA5   |   KEY3	| input IO	|			|
//	|	PA6   |   KEY4 	| input IO	|			|
//	|	PA7   |   KEY7 	| output IO	|			|
//	|	PB7	  |   		| output IO	|  LED		|
// BC45F0023
//	|	PA0   |   	 	| output IO	|  OCDSDA	|
//	|	PA1   |   	 	| output IO	|  LVD		|
//	|	PA2   |   	 	| output IO	|  OCDSCK	|
//	|	PA3	  |	  KEY1	| input IO	|			|
//	|	PA4   |   KEY2	| input IO	|			|
//	|	PA5   |   KEY3	| input IO	|			|
//	|	PA6   |   KEY4	| input IO	|			|
//	|	PA7   |   	 	| output IO	|TX_OUTPUT_DATA|
//	|	PB0	  |   		| 			|  			|
//	|	PB1	  |   		| 			|  			|
//	|	PB7	  |   		| output IO	|  LED		|
//*************************************************************************************
#include "BC68F3132.h"
#include "main.h"
#include "key.h"
#include "app_set.h"
#include "hopping_set.h"
#include "packet_format.h"

// transmit
bit key_changemode_state = 0;
bit new_data_flag = 0;
uint8 pulse_width_low = 0, pulse_width_high = 0;
uint8 count_low = 0, count_high = 0,count_reset=0;
uint16 count_sleep=0;
bit preamble_detected = 0;
uint8 bit_index = 0;
uint8 bit_index_2=0;
uint8 preamble_count = 0;
uint8 i;
uint8  Askcount=0,Fskcount=0;
uint8 MBUFCOPY[4];

bit fcheckkey;
uint8 T_GUARD_COUNT;
uint16 cTimer500ms, tCount, tBACW, cBACW, cMaxLongPress;

uint8 K_R[4];
uint8 cSync[2];
byte_t FLAG1, FLAG2;


	

uint8 CRC_8BIT, CRC_4BIT;

#if (DISC_LENGTH == 0)
extern uint8 ovr;
#endif

extern bit fkeyrelse;

const unsigned char KEYDATA[21] = {1, KEY1, KEY2, KEY3, KEY4, KEY5, KEY6, KEY7, KEY8, KEY9, KEY10, KEY11, KEY12, KEY13, KEY14, KEY15, KEY16, KEY17, KEY18, KEY19, KEY20};
//*************************************************************************************
// main
void main()
{
	if ((_pdf == TRUE) && (_to == TRUE))
		goto WakeUpBegin;
	_wdtc = 0x57;
	_hircc = 0x05;
	_scc = 0x02;
	RAM_CLR(); // clear RAM
	Const_Init(); // SERIAL_NUMBER //MFG
    ReadRfdata();
	SYNC_Load(); // load sync counter
	produceKey();
#if (PACKET_TYPE == 0)
	CRC_8BIT = Serial_CRC();
#endif

PAWAKELoop:
	//-----Deepsleep wake up ==> DISABL E IO_ISO_EN and PWDN
	_wdtc = 0x57;
	_hircc = 0x05; // 0x09:12M  0x0d:8M
	_scc = 0x02;   // fH

	MCU_Init();		  // IO/KEY/LVD/LED initialization
	freqfinit();      //freqinput
	BC68F3132_Init(); // BC68F3132 initialization
	intc_init();
	Timer_Init(); // TBx/CTM0/CTM1 initialization
	
	ReadRfdata();

NEW_TX_START:
	fSleep = FALSE;
	fMaxOk = FALSE; // clear flag
	fkeyrelse = TRUE;
	tFrameCount = FRAME_COUNT;	  // for min packet
	tGuardCount = GUARD_COUNT;	  // for guard time
	cMaxLongPress = MaxLongPress; // for max packet
	cTimer500ms = 0;
#if (BACW == 1)
	tBACW = 540;
#elif (BACW == 2)
	tBACW = 1560;
#endif
	RFTimerOff;	  // RF interrupt disable (CTM0)
	BACWTimerOff; // BACW interrupt disable (CTM1)

#if LED_EN
	LED_OFF; // LED off
#endif
	_emi = TRUE; // interrupt enable
RunLoop:
	_clrwdt();
	ChangeKeyStatus(); // change statue
	if (key_changemode_state)
	{
	    fcheckkey=FALSE;
		fTimer2ms = FALSE;
		//LED_ON;
		_pbc0=1;
	    _pac3 = 1;
	    _drvcc0=1;

	   _pac1 = 0;
	   _pa1 = 1;
	   

		if (new_data_flag) //  Rx_data
		{
			_emi = 0;
			writedata();
			key_changemode_state = 0;
			ReadRfdata();	   // READ EEPROM
			Const_Init();
			new_data_flag = 0; 
			LED_OFF;
			_pbc0=1;
			_emi = 1;
		}
		
		if(count_sleep>=10000)
		{
			count_sleep=0;
			count_reset++;
			_pb7=~_pb7;              //LED
			if(count_reset>=24)
			{
				count_reset=0;
				key_changemode_state=0;
			}
		}
	}
	else
	{
		//------------------------------------------------------------------------------------
		// check Key	state every 2ms
		if (fTimer2ms)
		{
			fTimer2ms = FALSE;
			if (CheckKeyStatus())
				fcheckkey = TRUE;
			else
				fcheckkey = FALSE;
			if (_lvdo)
			{
				fVlow = 1;
			}
			else
				fVlow = 0;
			if (fTxStart == 0)
			{
				cTimer500ms++;
				if (cTimer500ms == 250)
				{
					cTimer500ms = 0;
					fSleep = TRUE;
				}
			}
		}
	}

//------------------------------------------------------------------------------------
// BACW
#if (BACW)
	if (fTimerBACW)
	{
		fTimerBACW = FALSE;
		if (++tCount == tBACW)
		{
			tCount = 0;
			fBacwOk = TRUE;
			BACWTimerOff;
		}
	}
#else
	fBacwOk = TRUE;
	BACWTimerOff;
#endif

	//------------------------------------------------------------------------------------
	// first press (if press the key & the key value changes & RF not in send state) (KeyPreviousStatus!=0) &&
	if (fcheckkey && !fTxBusy && !fTxStart)
	{
#if LED_EN
		LED_ON; // Tx LED on
#endif
		fcheckkey = FALSE;
		fTxStart = TRUE;
		fTxBusy = TRUE; // RF in send state
		cTimer500ms = 0;
		MRFDATA = KeyCurrentStatus; // 4bit key status
#if (PACKET_TYPE == 0)
		CRC_4BIT = Encryp_CRC();

#endif
		Hopping_Encryption(); // 32bit hopping code
		SYNC_Save();		  // save sync counter
#if (PACKET_TYPE == 0)
		DATA1_PRO();
#else
		DATA_PRO(); // fill in data to Tx buffer
#endif
		RFTimerOn; // RF start send
	}

	//------------------------------------------------------------------------------------
	// resend data
	if (tFrameCount != 0 && !fTxBusy && fBacwOk && fTxStart)
	{
		fBacwOk = FALSE;
		fRepeat = TRUE;
		fTxBusy = TRUE;
#if (PACKET_TYPE == 0)
		CRC_4BIT = Encryp_CRC();
		DATA1_PRO();
#else
		DATA_PRO(); // fill in data to Tx buffer
#endif
		RFTimerOn; // RF start resend
	}

	//------------------------------------------------------------------------------------
	// end of send min frame data
	if (tFrameCount == 0 && !fTxBusy)
	{

		if (MRFDATA != KeyCurrentStatus && fcheckkey)
		{
			fTxStart = FALSE;
			fRepeat = FALSE;
			fDataOk = FALSE;
			LED_OFF;
			RFTimerOff;
			BACWTimerOff;
			goto NEW_TX_START;
		}
		else if (fcheckkey && !fMaxOk && MaxLongPress != 0)
		{ // if long press
		
		//FSKOOK();     //FSKOOK_Change
		  
			tFrameCount = TRUE;
#if LongPress_transmit == 0
			if (--cMaxLongPress == 0)
				fMaxOk = TRUE;
#endif
		}
		else // exceed max time
		{
			fTxStart = FALSE;
			fRepeat = FALSE;
			fDataOk = FALSE;
			fTxBusy = FALSE;
			fcheckkey = FALSE;
			fSleep = TRUE; // set sleep flag
#if LED_EN
			LED_OFF; // Tx LED off
#endif
			RFTimerOff;	  // RF end
			BACWTimerOff; // BACW end
		}
	}

	//------------------------------------------------------------------------------------
	// ready to go to sleep mode
	if (fSleep)
	{
		_wdtc = 0xAF;
		_scc = 0x00;
		_hircc = 0x01;
		_lvden = 0; // DISABLE LVD
		Key_Init();
		_halt(); // sleep mode

		_clrwdt();
		goto PAWAKELoop; // wake
	}
	goto RunLoop;

WakeUpBegin:
	goto PAWAKELoop;
}

// main end
//*************************************************************************************

//----------------------------------------------------------
// clear RAM
//----------------------------------------------------------
void RAM_CLR(void)
{
	_bp = 0;
	_mp1 = 0x80;
	for (_tblp = 0x80; _tblp > 0; _tblp--)
	{
		_iar1 = 0;
		_mp1++;
	}
}

//----------------------------------------------------------
// IO initialization
//----------------------------------------------------------
void IO_Init()
{
	_pas0 = 0;
	_pas1 = 0;
	_pbs1 = 0;
}

//----------------------------------------------------------
// TB0 initialization
//----------------------------------------------------------
void TBx_Init()
{
	/*undetermined*/
	_pscr = 0x01;
	_tb0c = 0x03;
	_tb0e = 1;
	_tb0on = 1;

	_tb1c = 0x02; // my
	_tb1e = 1;	  // my
	_tb1on = 1;	  // my
	MBUF[0]=1;
}

//----------------------------------------------------------
// write eeprom
// input : eeprom addr , data
//----------------------------------------------------------
void EEPROM_WR(unsigned char EEPROM_ADDR, unsigned char DATA)
{
	_eea = EEPROM_ADDR;
	_eed = DATA;
	_bp = 0x01;
	_mp1 = 0x40; // eec address
	_iar1 |= 0x08;
	_iar1 |= 0x04;

	while (_iar1 & 0x04)
	{
		GCC_CLRWDT();
	}

	_iar1 = 0x00;
	_bp &= 0b11111110;
}

//----------------------------------------------------------
// read eeprom
// input : eeprom addr
// output: eeprom data
//----------------------------------------------------------

unsigned char EEPROM_RD(unsigned char EEPRON_ADDR)
{
	uint8 tData;

	_eea = EEPRON_ADDR;
	_mp1 = 0x40;
	_bp = 0x01;
	_emi = 0;
	_iar1 |= 0x02;
	_iar1 |= 0x01;

	while (_iar1 &= 0x01)
	{
		GCC_CLRWDT();
	}

	tData = _eed;
	_iar1 = 0x00;
	_bp &= 0b11111110;

	return tData;
}
//----------------------------------------------------------
// reset sync counter
//----------------------------------------------------------
void SYNC_Load()
{
	if (EEPROM_RD(2) != 0xbc)
	{
		EEPROM_WR(2, 0xbc); //
		cSync[0] = SYNC_COUNTER & 0xff;
		cSync[1] = SYNC_COUNTER >> 8;

		EEPROM_WR(0, (uint8)(cSync[0]));
		EEPROM_WR(1, (uint8)(cSync[1]));
#if (DISC_LENGTH == 0)
#if (OVR_TYPE == 0)
		ovr = 0x00;
		EEPROM_WR(3, ovr);
#endif
#if (OVR_TYPE == 1)
		ovr = 0x01;
		EEPROM_WR(3, ovr);
#endif
#if (OVR_TYPE == 2)
		ovr = 0x03;
		EEPROM_WR(3, ovr);
#endif
#endif
	}
	else
	{
		cSync[0] = EEPROM_RD(0);
		cSync[1] = EEPROM_RD(1);
#if (DISC_LENGTH == 0)
		ovr = EEPROM_RD(3);
#endif
	}
}

//----------------------------------------------------------
// save sync counter
//----------------------------------------------------------
void SYNC_Save()
{
	EEPROM_WR(0, (uint8)(cSync[0]));
	EEPROM_WR(1, (uint8)(cSync[1]));
}

//----------------------------------------------------------
// ???????A
//----------------------------------------------------------
void ChangeKeyStatus() //為檢波狀態
{
	if ((!M7) && (!M2) && 1)
	{
		
		if(count_sleep>=40000)    //LED_time
		{
			count_sleep=0;
			count_reset++;
			if(count_reset>=2)
			{
				count_reset=0;
				key_changemode_state=1;
			}
		}
	}
	
}




void ReadRfdata()   
{
	_emi=0;

	for (i = 0; i < 8; i++)
	{
		MFG[i] = EEPROM_RD(11 - i);
	}
	for (i = 0; i < 4; i++)
	{
		SERIAL_NUMBER[i] = EEPROM_RD(15 - i);
	}
	for (i = 0; i < 4; i++)
	{
		MBUFCOPY[i] = EEPROM_RD(30+i);
	}
	
	_emi=1;

}

void writedata()
{
	_emi=0;
	for (i = 0; i < 8; i++)
	{
		EEPROM_WR(i+4,MFG[i]);
	}
	for (i = 0; i < 4; i++)
	{
		EEPROM_WR(i+12,SERIAL_NUMBER[i]);
	}
	for (i = 0; i < 4; i++)
	{
		EEPROM_WR(30+i,MBUFCOPY[i]);
	}
	_emi=1;
}

void intc_init()
{
	
	_integ = 0x03;
	_inte = 1;
}

void freqfinit()
{
	MBUF[0]=0x00;
	MBUF[1]=0x08;
	MBUF[2]=0xA0;
	MBUF[3]=0xE1;
	MBUF[4]=0x34;
	if(MBUFCOPY[0]==0xA3)
	{
		MBUF[5]=0x4D;
	}
	else
	{
		MBUF[5]=0xCD;
	}
	MBUF[6]=MBUFCOPY[1];
	MBUF[7]=MBUFCOPY[2];
	MBUF[8]=MBUFCOPY[3];
	MBUF[9]=0x4B;					
}

void freqeeprom()
{
	for(i=0;i<8;i++)
	{
		EEPROM_WR(30+i,MBUF[2+i]);
	}
}




void decode_pulse(uint8 width_high , uint8 width_low) //解碼
{
  uint8 index;

  if (bit_index < 64)
  {
    index = bit_index;
    if (width_high < width_low)
    {
      MFG[index / 8] &= ~(1 << (7 - (index % 8)));
    }
    else
    {
      MFG[index / 8] |= (1 << (7 - (index % 8)));
    }
  }
  if (bit_index > 64 && bit_index < 96)
  {
    index = bit_index - 64;
    if (width_high < width_low)
    {
      SERIAL_NUMBER[index / 8] &= ~(1 << (7 - (index % 8)));
    }
    else
    {
      SERIAL_NUMBER[index / 8] |= (1 << (7 - (index % 8)));
    }

  }
  if (bit_index > 96)
  {
    index = bit_index - 96;
    if (width_high < width_low)
    {
      MBUFCOPY[index / 8] &= ~(1 << (7 - (index % 8)));
    }
    else
    {
      MBUFCOPY[index / 8] |= (1 << (7 - (index % 8)));
    }
  }

  bit_index++; // 增加位索引
  if (bit_index >= 128)
  {
    bit_index = 0;
    new_data_flag = 1; // 設置數據接收完成標誌
    preamble_detected = 0;
    preamble_count = 0;
  }
}

DEFINE_ISR(time, 0x10)
{
	_tb0f = 0;
	count_high++;
	count_low++;
	
	if (((!M7) && (!M2)) || (key_changemode_state == 1)) 
    {
        count_sleep++;
    }
    else
    {
        count_sleep = 0; // 鬆開按鍵且未進入狀態才清零
    }

    if (count_low > TIMEOUT_THRESHOLD)
    {
        bit_index = 0;
        preamble_detected = 0;
        count_low = 0;  
    }
}


DEFINE_ISR(intc, 0x04)
{
	_intf = 0;
	if (RX) //_pa3
	{
		
		count_high = 0;
		pulse_width_low = count_low;
		if (!preamble_detected)
		{
			if (pulse_width_high > PREAMBLE_MIN && pulse_width_high < PREAMBLE_MAX)
			{
				preamble_count++;

				if (preamble_count == 6)
				{
					preamble_detected = 1;
					
				}
			}
			else
			{
				preamble_count = 0;
			}
		}
		else
		{
			decode_pulse(pulse_width_high ,pulse_width_low);
		}
	}
	else
	{
		count_low = 0;
		pulse_width_high = count_high;
	}
}
