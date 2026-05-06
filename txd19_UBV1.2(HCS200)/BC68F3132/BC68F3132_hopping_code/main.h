#include "typedef.h"
#include "ui.h"

extern uint8 KeyCurrentStatus;
extern uint8 T_FRAME_COUNT;
extern uint8 MRFDATA;
extern uint8 MFG[8];
extern uint8 DISC[2];
extern uint8 SERIAL_NUMBER[4];
extern uint8 MBUF[10]; 
extern byte_t x, Key_IO;
extern uint8 count_low;
extern uint8 count_high;
extern bit new_data_flag;


void IO_Init();
void RAM_CLR(void);
void TBx_Init();
void freqfinit();
void freqeeprom();

//RXstate
void ChangeKeyStatus();   
void intc_init();        
void LED_CHANG();
void ReadRfdata();
void writedata();
void IOchange();


#define RX _pa3
#define RAM_BANK0 0x80
#define TIMEOUT_THRESHOLD 100
#define PREAMBLE_MAX 90
#define PREAMBLE_MIN 5

//-----------------------------------------------------------------------
// asm
extern void CTM0_INIT();
extern void CTM1_INIT();
extern void MFG_INIT();
extern void SN_INIT();
extern void DISC_INIT();
extern void I2C_INI();
extern void I2C_XT_WK();
extern void I2C_SCK_TOGGLE();
extern void I2C_SDA_TOGGLE();
extern void WRITE_I2C_MULTI_BYTE();
extern void READ_I2C_MULTI_BYTE();
void DATA_PRO();
void DATA1_PRO();

//-----------------------------------------------------------------------
// other .c
extern void Key_Init(void);
extern void Key_To_IO();
extern uint8 CheckKeyStatus(void);
extern void LVD_Init(void);
extern void LED_Init(void);
void Delay_20us(uint8 xs);
void EEPROM_WR(unsigned char EEPROM_ADDR, unsigned char DATA);
unsigned char EEPROM_RD(unsigned char EEPRON_ADDR);
void SYNC_Load();
void SYNC_Save();
void SYNC_CLR();

void produceKey();
uint8 Serial_CRC();
//-----------------------------------------------------------------------
// C<<==>>ASM
#define fTimer2ms FLAG1.bits.b0	 // 2ms flag
#define fSleep FLAG1.bits.b1	 // sleep flag
#define fTxBusy FLAG1.bits.b2	 // sending flag
#define fDataOk FLAG1.bits.b3	 // data processed flag
#define fMaxOk FLAG1.bits.b4	 // Send Ok flag
#define fTxStart FLAG1.bits.b5	 // TX send flag
#define fBacwOk FLAG1.bits.b6	 // bacw time ok flag
#define fTimerBACW FLAG1.bits.b7 // bacw timer flag

#define fVlow FLAG2.bits.b0
#define fRepeat FLAG2.bits.b1

#define tFrameCount T_FRAME_COUNT
#define tGuardCount T_GUARD_COUNT

//-----------------------------------------------------------------------
#define BC68F3132_Init()  \
	;                     \
	I2C_INI();            \
	I2C_XT_WK();          \
	I2C_SCK_TOGGLE();     \
	WRITE_I2C_MULTI_BYTE(); \
	I2C_SCK_TOGGLE();     \
	I2C_SDA_TOGGLE(); // RF initialization



#define MCU_Init()                     \
	;                                  \
	IO_Init();	/*IO  initialization*/ \
	Key_Init(); /*KEY initialization*/ \
	LVD_Init(); /*LVD initialization*/ \
	LED_Init(); /*LED initialization*/

#define Timer_Init()                                 \
	;                                                \
	CTM0_INIT(); /*CTM0 initialization for RF send*/ \
	CTM1_INIT(); /*CTM1 initialization for BACW*/    \
	TBx_Init();	 /*TB0 initialization for 2ms*/
#if (HOP_DISC_TYPE == 0)
#define Const_Init()                                  \
	;                                                 \
	DISC[0] = SERIAL_NUMBER[0];                       \
	DISC[1] = SERIAL_NUMBER[1]; /*const	variable : DISC*/
#else
#define Const_Init()                                  \
	;                                                 \
	DISC_INIT(); /*const	variable : DISC*/
#endif
//-----------------------------------------------------------------------

#define RFTimerOn _ct0on = TRUE
#define RFTimerOff _ct0on = FALSE

#define BACWTimerOn _ct1on = TRUE
#define BACWTimerOff _ct1on = FALSE



