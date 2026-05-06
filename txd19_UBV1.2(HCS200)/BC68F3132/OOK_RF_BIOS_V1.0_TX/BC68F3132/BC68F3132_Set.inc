INCLUDE ui.inc

;==========================================================================
;RF other settings
#define			DSleepTimer			14						;tDLY= 2ms℅(DLY_TOFF[3:0]+2;        0~14:4ms~32ms 15:Never enter the Deep Sleep Mode) 											
#define	  		FDEV				25						;8:8KHz 12:12KHz 25:25KHz 50:50KHz 80:80KHz 100:100KHz
#define         DM_SYMBOL_RATE  400    

#define			DISC_CODE			HOP_USER_DISC_TABLE

;IIC IO macro definition
#DEFINE	SCK_DIR 	PBC1
#DEFINE	SCK_PORT	PB1
#DEFINE	SDA_DIR 	PBC0
#DEFINE	SDA_PORT	PB0

;Software IIC
;==========================================================================
DELAY	MACRO
		JMP		$+1
		JMP		$+1
		JMP		$+1
		ENDM

SCK_LO	MACRO
		CLR		SCK_PORT
		ENDM						
		
SCK_HI	MACRO
		SET		SCK_PORT
		ENDM								

SDA_IN	MACRO
		SET		SDA_DIR
		JMP		$+1
		ENDM
		
SDA_OUT	MACRO
		CLR		SDA_DIR
		JMP		$+1
		ENDM
		
SDA_LO	MACRO
		CLR		SDA_PORT
		ENDM						
		
SDA_HI	MACRO
		SET		SDA_PORT
		ENDM								

SNZ_SDA	MACRO
		SNZ		SDA_PORT
		ENDM

SZ_SDA	MACRO
		SZ		SDA_PORT
		ENDM


I2C_STR	MACRO
		SDA_OUT
		SCK_HI
		SDA_HI
		DELAY
		SDA_LO
		ENDM
		
I2C_STP MACRO
		SDA_OUT
		SDA_LO
		DELAY
		SCK_HI
		DELAY
		SDA_HI
		DELAY
		ENDM
;==========================================================================

