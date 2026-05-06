INCLUDE "BC68F3132_Set.inc"


;==========================================================================
RAMBank 0  DATA
DATA       .SECTION     'DATA'
mTemp1		DB	2 DUP(?)

PUBLIC   _SERIAL_NUMBER
PUBLIC   _MFG
PUBLIC   _SYMBOL_TIME
PUBLIC	 _DISC
PUBLIC  _MBUF

_SERIAL_NUMBER  DB      4 dup(?)
_MFG            DB      8 dup(?)
_SYMBOL_TIME    DB      2 dup(?) 
_DISC			DB		2 dup(?)
_MBUF		    DB	   10 dup(?)

BC68F3132    .SECTION  PAGE	'code'
;==========================================================================

;==========================================================================
;==========================================================================
; input    : mBuf1f[0]=addr. ,mBuf1f[1]=length, mBuf1f[9:2]=data
; template : MP0, mTemp1[0], mTemp1[1]
;==========================================================================
public	_Write_I2C_Multi_Byte
_Write_I2C_Multi_Byte	proc
			CALL	I2C_Start
			
			MOV		A,42h
			CALL	I2C_SendByte
			CALL	I2C_GetAck
			SZ		C
			JMP		wimb_Fail
			
			MOV		A,_MBUF[0]
			CALL	I2C_SendByte
			CALL	I2C_GetAck
			SZ		C
			JMP		wimb_Fail

			MOV		A,_MBUF[1]
			MOV		_MBUF[1],A
			MOV		A,OFFSET _MBUF[2]
			MOV		MP0,A
	wimb_Lp:	
			MOV		A,IAR0
			CALL	I2C_SendByte
			CALL	I2C_GetAck
			SZ		C
			JMP		wimb_Fail
			INC		MP0					
			SDZ		_MBUF[1]
			JMP		wimb_Lp
	wimb_Fail:
			CALL	I2C_Stop
			RET
_Write_I2C_Multi_Byte	endp			



;==========================================================================
; input    : mBuf1f[0]=addr. ,mBuf1f[1]=length
; output   : mBuf1[9:2]
; template : MP0, mTemp1[0], mTemp1[1]
;==========================================================================
public	_Read_I2C_Multi_Byte
_Read_I2C_Multi_Byte	proc
			CALL	I2C_Start
			
			MOV		A,42h
			CALL	I2C_SendByte
			CALL	I2C_GetAck
			SZ		C
			JMP		rimb_Fail
			
			MOV		A,_MBUF[0]
			CALL	I2C_SendByte
			CALL	I2C_GetAck
			SZ		C
			JMP		rimb_Fail
			
			CALL	I2C_Start
			
			MOV		A,43h
			CALL	I2C_SendByte
			CALL	I2C_GetAck
			SZ		C
			JMP		rimb_Fail
			
			MOV		A,OFFSET _MBUF[2]
			MOV		MP0,A									
			MOV		A,_MBUF[1]
			MOV		_MBUF[1],A
	rimb_Lp:	
			CALL	I2C_GetByte
			MOV		IAR0,A
			INC		MP0
			SET		C
			SDZA	_MBUF[1]			
			CLR		C
			CALL	I2C_SendAck
			SDZ		_MBUF[1]	
			JMP		rimb_Lp
			
	rimb_Fail:			
			CALL	I2C_Stop
			RET
_Read_I2C_Multi_Byte	endp			


;==========================================================================
;I2C_Ini
;==========================================================================
public	_I2C_Ini
_I2C_Ini		proc
			CLR		SCK_DIR
			CLR		SDA_DIR
			SET		SCK_PORT
			CLR		SDA_PORT
			ret
_I2C_Ini		endp



;==========================================================================
;I2C_XT_Wk
;==========================================================================
public	_I2C_XT_Wk
_I2C_XT_Wk	proc
			SDA_OUT
			SDA_HI		
			CALL	Delay_10us
			SDA_LO
			CALL	Delay_1ms
			CALL	Delay_1ms
			CALL	Delay_1ms
			RET
_I2C_XT_Wk	endp
		
		
		
;==========================================================================
;I2C_SCK_Toggle
;==========================================================================
public	_I2C_SCK_Toggle
_I2C_SCK_Toggle	proc
			SDA_HI
		
			SCK_HI
			CALL	Delay_10us
			SCK_LO
			CALL	Delay_10us
			CALL	Delay_10us
			CALL	Delay_10us
			SCK_HI
			CALL	Delay_10us
			RET
_I2C_SCK_Toggle	endp
	
	
		
;==========================================================================
;I2C_SDA_Toggle
;==========================================================================
public	_I2C_SDA_Toggle
_I2C_SDA_Toggle	proc
			SDA_LO
			CALL	Delay_10us
			SDA_HI
			CALL	Delay_10us
			SDA_LO	
			CALL	Delay_1ms
			CALL	Delay_1ms
			CALL	Delay_1ms	
			RET
_I2C_SDA_Toggle	endp



;==========================================================================
;I2C_Start
;==========================================================================
public	I2C_Start
I2C_Start	proc
			I2C_STR
			RET
I2C_Start	endp
	
	
				
;==========================================================================
;I2C_Stop
;==========================================================================
public	I2C_Stop
I2C_Stop	proc
			I2C_STP
			RET
I2C_Stop	endp
	
	
				
;==========================================================================
; Input : ACC=Data
; Template : mTemp1[0]
;==========================================================================
public	I2C_SendByte
I2C_SendByte	proc
			SDA_OUT
			MOV		mTemp1,A
			MOV		A,08h
	isb_Lp:		
			SCK_LO
			RLC		mTemp1
			SZ		C
			JMP		$1
			
	$0:
			SDA_LO
			JMP		$9			
			
	$1:		
			SDA_HI
	
	$9:	
			JMP		$+1
			SCK_HI
			DELAY
			SDZ		ACC
			JMP		isb_Lp
			SCK_LO
			SDA_IN			
			RET
I2C_SendByte	endp



;==========================================================================
; Template : mTemp1[0]
; Output : ACC
;==========================================================================
I2C_GetByte	proc
			SDA_IN
			MOV		A,08h
	igb_Lp:		
			SCK_LO
			DELAY
			
			SCK_HI
			CLR		C
			SZ_SDA
			SET		C			
			RLC		mTemp1
			
			SDZ		ACC
			JMP		igb_Lp
			
			SCK_LO
			MOV		A,mTemp1			
			RET
I2C_GetByte	endp



;==========================================================================
; Input : C
;==========================================================================
I2C_SendAck	proc
			SDA_OUT
			SZ		C
			JMP		$1
	$0:
			SDA_LO
			JMP		$9			
	$1:				
			SDA_HI	
	$9:
			SCK_HI
			DELAY
			SCK_LO
			RET
I2C_SendAck	endp



;==========================================================================
; Output : C
;==========================================================================
I2C_GetAck	proc
			SDA_IN
			SCK_HI
			DELAY
			CLR		C
			SZ_SDA
			SET		C		
			SCK_LO
			RET
I2C_GetAck	endp



;==========================================================================
;Delay_10us
;==========================================================================
public	Delay_10us
Delay_10us	proc
			MOV		A,4
			NOP
			SDZ		ACC
			JMP		$-2
			RET
Delay_10us	endp



;==========================================================================
;Delay_100us
;==========================================================================
public	Delay_100us
Delay_100us	proc
			MOV		A,49
			NOP
			SDZ		ACC
			JMP		$-2
			RET
Delay_100us	endp



;==========================================================================
;Delay_1ms
;==========================================================================
public	Delay_1ms
Delay_1ms	proc
			MOV		A,167
			SDZ		ACC
			JMP		$-1
			
			MOV		A,166
			SDZ		ACC
			JMP		$-1
			
			MOV		A,166
			SDZ		ACC
			JMP		$-1

			MOV		A,166
			SDZ		ACC
			JMP		$-1
			
			RET
Delay_1ms	endp
		
	
		
;==========================================================================
;CTM0_INIT
;==========================================================================		
            public    _CTM0_INIT
_CTM0_INIT:
            MOV     A,00000000B			 
            MOV     CTM0C0,A
            MOV     A,11000001B         ;timer mode/Comparator A match
            MOV     CTM0C1,A
 
            MOV     A,LOW (DM_SYMBOL_RATE*2)	;update time counter
            MOV     CTM0AL,A
            MOV     A,HIGH (DM_SYMBOL_RATE*2)
            MOV     CTM0AH,A                		            
            
            CLR     MF0F   
            SET     MF0E              	;Multi-function 0 Interrupt control
            SET		CTM0AE	
                              
            RET
      
;==========================================================================
;CTM1_INIT
;==========================================================================		
            public    _CTM1_INIT
_CTM1_INIT:
            MOV     A,00000000B			
            MOV     CTM1C0,A
            MOV     A,11000001B         ;timer mode/Comparator A match
            MOV     CTM1C1,A
 
            MOV     A,LOW (DM_SYMBOL_RATE)	;update time counter
            MOV     CTM1AL,A
            MOV     A,HIGH (DM_SYMBOL_RATE)
            MOV     CTM1AH,A                		            
            
            CLR     MF1F   
            SET     MF1E              	;Multi-function 0 Interrupt control
            SET		CTM1AE	
                             
            RET
      
                        
;==========================================================================
;set 10bit or 12bit  discrimination
;==========================================================================	
			public	_DISC_INIT
_DISC_INIT:	
			MOV		A,low offset _DISC
			MOV		MP0,A
			
			MOV		A,2
			MOV		mTemp1[0],A
			
			MOV		A,LOW DISC_CODE
			MOV		TBLP,A
			MOV		A,HIGH DISC_CODE
			MOV		TBHP,A
	DISC_loop:           
			TABRD   IAR0
			INC		TBLP			
			INC		MP0          
			SDZ		mTemp1
			JMP		DISC_loop               
            RET
			