INCLUDE TX_Encoder.inc

;******************************************************************
;******************************************************************
PUBLIC	 RF_ISR
PUBLIC	 _DATA_PRO
PUBLIC   _MRFDATA
PUBLIC	 _T_FRAME_COUNT					;increase
public   _DATA1_PRO

extern	 _T_GUARD_COUNT		 :byte		;increase
extern	 _FLAG1          	 :byte
extern   _FLAG2				 :byte
extern   _K_R            	 :byte
extern 	 _MFG     			 :byte
extern   _SERIAL_NUMBER		 :byte    
extern   _CRC_4BIT			 :byte 

;******************************************************************
;******************************************************************
RAMBank 0  DATA
DATA	    .SECTION  	'data'

mTxBufSN_0      DB      ?       
mTxBufSN_1		DB		?		
mTxBufSN_2     	DB      ?
mTxBufSN_3		DB		?		;RF serial Number 28bit  

mTxBuf1     	DB      ?
mTxBuf2     	DB      ?
mTxBuf3     	DB      ?
mTxBuf4     	DB      ?
mTxBuf5     	DB      ?

mTxBits     	DB      ?		;data bit number
mTxCnt      	DB      ?		;µ¥¦PTE

mTemp			DB		?

Crypto_counter	DB		?

mTxStep			DB		?

fZeroOne		dbit

mSTS1			DB		?		;Stack res
mACC1			DB		?
;==============================================
;=============================================

_MRFDATA	    DB		? 


_T_FRAME_COUNT	    DB		? 		;increase

;==============================================

;==============================================
;==============================================
if(Leading_code==3)
TX_PRE_HIGH 	EQU     1  
TX_PRE_LOW  	EQU     31  
endif

if(Leading_code==4)    
TX_PRE_HIGH 	EQU     1  
TX_PRE_LOW  	EQU     1 
endif
;==============================================
;==============================================
if(Start_Format==3)
TX_STR_P_LOW 	EQU     1  
TX_STR_LOW  	EQU     8  
endif
;==============================================
;==============================================
if(Bit_Format==2)
TX_1_HIGH   	EQU     1  
TX_1_LOW    	EQU     2  

TX_0_HIGH   	EQU     2  
TX_0_LOW    	EQU     1  
endif
;==============================================
;==============================================
IF(END_Format==4)
TX_END_HIGH		EQU     2
TX_END_LOW		EQU     2
ENDIF


;--------------------------
INIT_STEP		EQU     0
PRE_LOW			EQU     1
PRE_HIGH		EQU     2
STR_STEP		EQU     3
STR_END			EQU     4
ENCHIGH_STEP	EQU		5
ENCLOW_STEP		EQU		6 
FIXHIGH_STEP	EQU     7
FIXLOW_STEP    	EQU     8
;--------------------------

;******************************************************************
;******************************************************************
TX_LOW      MACRO
            CLR     TX_OUTPUT 
            ;CLR     TX_OUTPUT_DATA 
;            CLR		pb7
ENDM
            
TX_HIGH     MACRO
		    SET     TX_OUTPUT 
            ;SET     TX_OUTPUT_DATA
;			set		PB7
ENDM


CODE3	    .SECTION  PAGE	'code'
;******************************************************************  
;------------------------------------------------------------------      
RF_ISR:
			

			SNZ		RF_TMf
			RETI						
			CLR		RF_TMf

;------------------------------------	
;increase : 2021/04
;brief    : frame loop mechanism of handling error  
;------------------------------------	
			SZ		_T_FRAME_COUNT
			
			jmp		$+2
			jmp		TxEnd
;------------------------------------			
			
	
			MOV		A,ACC
			MOV     mACC1,A   
            MOV     A,STATUS
            MOV     mSTS1,A

			CLR     WDT
          
            SNZ     _FLAG1.2    ;;fTxBusy
	        JMP     TMR_Ext
	        
	        snz		_FLAG1.3    ;;fDataOk
	        jmp		TMR_Ext
	        
	        MOV     A,mTxStep	;
	        ADDM    A,PCL
	        JMP     TxInit      ;0
	        JMP     TxPreLow    ;1        
	        JMP     TxPreHigh   ;2       
	        JMP     TxStart     ;3
	        JMP     TxStarLow   ;4
	        JMP		TxEncryHigh	;5  	
	        JMP		TxEncryLow	;6  	
	        JMP     TxFixedHi   ;7   
	        JMP     TxFixedLow  ;8   
	        JMP     TxEndHIGH   ;9
	        JMP		TxEndLOW	;10
  	        JMP		TxEnd		;11
            
    TMR_Ext:
	TMR_Ext1:
			mov		a,mSTS1
			mov		STATUS,A
			mov		a,mACC1
			mov		ACC,A
			reti      
TxInit:               
            MOV     A,Encrypted_C+1     ;Encrypted Code
            MOV     mTxBits,A   
                        
            TX_HIGH

            MOV     A,TX_PRE_HIGH
            MOV     mTxCnt,A
if(Leading_code==4)            
            MOV     A,12
            MOV     Crypto_counter,A
	
endif             
           
            
            INC     mTxStep
            
            JMP     TMR_Ext 
                     
;******************************************************************
;Leading 
;------------------------------------------------------------------            
if(Leading_code==3)                 
TxPreLow:
            SDZ     mTxCnt
            JMP     TMR_Ext 
            TX_LOW    
            MOV     A,TX_PRE_LOW   
            MOV     mTxCnt,A
            INC     mTxStep
            INC     mTxStep
            JMP     TMR_Ext  
            
TxPreHigh:  
			JMP     TMR_Ext          
endif

if(Leading_code==4)                 
TxPreLow:
            SDZ     mTxCnt
            JMP     TMR_Ext 
            TX_LOW
            
            SDZ     Crypto_counter
            JMP     $+5
    
            MOV     A,TX_PRE_LOW   
            MOV     mTxCnt,A
            INC     mTxStep
            JMP     $+3
            MOV     A,TX_PRE_HIGH
            MOV     mTxCnt,A
             
            INC     mTxStep
            JMP     TMR_Ext              

TxPreHigh:
            SDZ     mTxCnt
            JMP     TMR_Ext 
            
            TX_HIGH
            
            MOV     A,TX_PRE_HIGH
            MOV     mTxCnt,A    
            DEC     mTxStep
            JMP     TMR_Ext   
endif

;******************************************************************
;START    
if(Start_Format==1)
			
			
TxStart:

TxStarLow:			
			jmp		TxEncryHigh	
else  
  
TxStart:
            SDZ     mTxCnt
            JMP     TMR_Ext 

            MOV     A,TX_STR_P_LOW   
            MOV     mTxCnt,A
            
            INC     mTxStep
            JMP     TMR_Ext  
    
TxStarLow:
            SDZ     mTxCnt
            JMP     TMR_Ext        
            
            TX_LOW
            
            MOV     A,TX_STR_LOW  
            MOV     mTxCnt,A
            
            INC     mTxStep
            JMP     TMR_Ext  
            
endif             
;******************************************************************
;Encrypted Code              
TxEncryHigh:         	
     

            SDZ     mTxCnt
            JMP     TMR_Ext        

            SDZ     mTxBits
            JMP     $+2
			JMP		TxCodeHiPre

            TX_HIGH            

            CLR     C    
            RRC     mTxBuf5                              
            RRC     mTxBuf4      
            RRC     mTxBuf3
            RRC     mTxBuf2

            SNZ     C
            JMP     PrcTx0Hi
            JMP		PrxTx1Hi

TxCodeHiPre:
            inc     mTxStep
            inc		mTxStep
            
            MOV     A,Fixed_C+STATUS_Bit      ;Fixed Code Data
            MOV     mTxBits,A 
            
            TX_HIGH
            
            CLR		C
            RRC     mTxBuf1            
            RRC     mTxBufSN_0
            RRC     mTxBufSN_1
            RRC     mTxBufSN_2
            RRC     mTxBufSN_3      


            SNZ     C
            JMP     PrcTx0Hi
            jmp		PrxTx1Hi			
			            			             
TxFixedHi:      
			SDZ     mTxCnt
            JMP     TMR_Ext        

            SDZ     mTxBits
            JMP     TxCodeHi1  
            INC		mTxStep
            INC		mTxStep
	
	
	IF(END_Format==4)            
            MOV		A,TX_END_HIGH
            MOV		mTxCnt,A
            TX_HIGH
	ENDIF   
	IF(END_Format==1)            	
			jmp	TxEnd
	ENDIF      
            JMP		TMR_Ext
	TxCodeHi1:
			TX_HIGH
			CLR		C
			
          RRC		mTxBuf1			
            RRC     mTxBufSN_0			
            RRC     mTxBufSN_1
            RRC     mTxBufSN_2			
            RRC		mTxBufSN_3

            
            SNZ     C
            JMP     PrcTx0Hi
            jmp		PrxTx1Hi         			          
                  
TxEncryLow:     	
      
TxFixedLow:    
            SDZ     mTxCnt
            JMP     TMR_Ext        
            
            TX_LOW
            
            SNZ     fZeroOne
            JMP     Prc0Low
            
Prc1Low:    
            MOV     A,TX_1_LOW
            JMP     PrcLowC

Prc0Low:            
            MOV     A,TX_0_LOW

PrcLowC:
            MOV     mTxCnt,A
            
            MOV		A,mTxStep
            XOR		A,FIXLOW_STEP
            SZ		Z
			JMP		$+2
			JMP		PrcLowC1	
            MOV     A,FIXHIGH_STEP
            MOV     mTxStep,A
            JMP     TMR_Ext 
	PrcLowC1:
            MOV     A,ENCHIGH_STEP
            MOV     mTxStep,A
            JMP     TMR_Ext        


PrxTx1Hi:
            SET     fZeroOne
            
            MOV     A,TX_1_HIGH
            JMP     PrcHiC

PrcTx0Hi:                   
            CLR     fZeroOne

            MOV     A,TX_0_HIGH
PrcHiC:
            MOV     mTxCnt,A
            
            MOV		A,mTxStep
            XOR		A,FIXHIGH_STEP
            SZ		Z
			JMP		$+2
			JMP		PrcHiC1	
            MOV     A,FIXLOW_STEP
            MOV     mTxStep,A
            JMP     TMR_Ext 
	PrcHiC1:
            MOV     A,ENCLOW_STEP
            MOV     mTxStep,A
            JMP     TMR_Ext                                                    
  
TxEndHIGH:
	IF(END_Format==1)
			CLR		mTxCnt
			JMP		TxEnd
	TxEndLOW:
			JMP		TxEnd
					
	ENDIF

;------------------------------------	
;increase : 2021/04
;brief    : guard_time 
;------------------------------------	
IF(END_Format==2)					
			INC     mTxStep
            MOV     A,_T_GUARD_COUNT 
            MOV     mTxCnt,A
            TX_LOW
            JMP     TMR_Ext    
TxEndLOW:     
        	SDZ		mTxCnt
            JMP     TMR_Ext  
			JMP		TxEnd 
            
	 ENDIF
;-----------------------------------
IF(END_Format==4)		
            SDZ     mTxCnt
            JMP     TMR_Ext        
            
            TX_LOW
            MOV		A,TX_END_LOW
            MOV		mTxCnt,A
            INC		mTxStep
            JMP		TMR_Ext
           
TxEndLOW:
			SDZ		mTxCnt
			JMP		TMR_Ext            
		IF	0			
           	TX_HIGH
           	INC		mTxStep
           	JMP		TMR_Ext	
		ELSE
			JMP		TxEnd
	    ENDIF       	
	ENDIF
	
	      		      
TxEnd:
        TX_LOW   

        CLR     mTxStep
        
;-------------------------------------          
;increase : 2021/04
;brief    : frame loop/ BACW 
;------------------------------------
        SZ		_T_FRAME_COUNT
        DEC 	_T_FRAME_COUNT  ;resend
;        CLR     _FLAG1.6
        CLR     _FLAG1.7  	 	;fTimerBACW         
        
        clr		ct0on		 	;
        set		ct1on		    ;
;-------------------------------------    
          
        CLR     _FLAG1.2   		;fTxBusy    
        CLR		_FLAG1.3   		;fDataOk 
        JMP     TMR_Ext                 
;******************************************************************
;****************************************************************** 
;HT_Enhanced_DATA_PRO	  .SECTION  PARA	'code'        
_DATA1_PRO:
            CLR     mTxStep
			MOV		A,_K_R[3]
			MOV		mTxBuf5,A
			MOV		A,_K_R[2]
			MOV		mTxBuf4,A					
			MOV		A,_K_R[1]
			MOV		mTxBuf3,A	
			MOV		A,_K_R[0]
			MOV		mTxBuf2,A				;_K_R[]?32bit?[?K??u

			MOV		A,_SERIAL_NUMBER[3]
			MOV		mTxBufSN_0,A		

			MOV		A,_SERIAL_NUMBER[2]
			MOV		mTxBufSN_1,A	

			MOV		A,_SERIAL_NUMBER[1]
			MOV		mTxBufSN_2,A				

			MOV		A,_SERIAL_NUMBER[0]
			MOV		mTxBufSN_3,A	

;            CLR     mTxBufSN_0				;
            SWAP    _CRC_4BIT       
			MOV		A,_CRC_4BIT
			CLR     mTxBufSN_0.4
			CLR     mTxBufSN_0.5
			CLR     mTxBufSN_0.6
			CLR     mTxBufSN_0.7
			ADDM    A,mTxBufSN_0			;
			SWAP    _CRC_4BIT

			clr		mTxBuf1					; low battary
			SZ		_FLAG2.0
			set		mTxBuf1.0
			SZ		_FLAG2.1				; repeat  frame
			set		mTxBuf1.1
			
			SET		_FLAG1.3    ;;fDataOk										
			RET	 
                  
;******************************************************************
;****************************************************************** 
;Data_Pro	  .SECTION  PARA	'code'        
_DATA_PRO:
            CLR     mTxStep
			MOV		A,_K_R[3]
			MOV		mTxBuf5,A
			MOV		A,_K_R[2]
			MOV		mTxBuf4,A					
			MOV		A,_K_R[1]
			MOV		mTxBuf3,A	
			MOV		A,_K_R[0]
			MOV		mTxBuf2,A				;_K_R[]?32bit?[?K??u

			MOV		A,_SERIAL_NUMBER[3]
			MOV		mTxBufSN_0,A		

			MOV		A,_SERIAL_NUMBER[2]
			MOV		mTxBufSN_1,A	

			MOV		A,_SERIAL_NUMBER[1]
			MOV		mTxBufSN_2,A				

			MOV		A,_SERIAL_NUMBER[0]
			MOV		mTxBufSN_3,A	

;            CLR     mTxBufSN_0				;
            SWAP    _MRFDATA        
			MOV		A,_MRFDATA
			CLR     mTxBufSN_0.4
			CLR     mTxBufSN_0.5		
			CLR     mTxBufSN_0.6
			CLR     mTxBufSN_0.7
			ADDM    A,mTxBufSN_0			;
			SWAP    _MRFDATA

			clr		mTxBuf1
			SZ		_FLAG2.0			; low battary
			set		mTxBuf1.0
			SZ		_FLAG2.1			; repeat  frame
			set		mTxBuf1.1

			
Data_Pro_EXT:
			SET		_FLAG1.3    ;;fDataOk										
			RET		
			
					


