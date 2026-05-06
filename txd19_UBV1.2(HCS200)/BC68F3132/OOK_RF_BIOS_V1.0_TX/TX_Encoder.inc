INCLUDE BC68F3132.inc

;******************************************************************        
;****************************************************************** 
;	�f= 1ms/500us/200us(Synbol rate=1k/2k/5k)
;******************************************************************
;CHOOSE THE Leading Code mode
;******************************************************************
#define		Leading_code	    4	    ;3 	1�f high + 31�f low		;* RF BIOS V1.00
								        ;4  (1�f high+1�f low)*23	;* KEELOQ  V1.00
;******************************************************************
;CHOOSE Start Format
;******************************************************************
#define		Start_Format 	    3 	    ;1	disable					;* RF BIOS V1.00
							 	        ;3  (0�f high+10�f low)		;* KEELOQ  V1.00
;******************************************************************
;CHOOSE  Bit Format
;******************************************************************                    		
#define 	Bit_Format 		    2 	    ;2	only
								        ;	0-data: 2�f high+ 1�f low 		;* KEELOQ V1.00 
                      			        ;	1-data: 1�f high+ 2�f low
;******************************************************************
;CHOOSE  Button bit+VLow bit+Repeat bit(STATUS_Bit)
;******************************************************************				
#define 	STATUS_Bit			6	    ;6-bit
;******************************************************************
;CHOOSE  END_Format
;******************************************************************
#define 	END_Format 		    2	    ;1	disable					;* RF BIOS V1.00 
										;2	guard time low
								        ;4	(2�f High+2�f low)		;* RF BIOS V1.00 
								        
;******************************************************************
;Set  the data and the address width
;******************************************************************
#define		Encrypted_C			32	    ;32-bit Encrypted Code Data
#define		Fixed_C			    28	    ;28-bit Fixed Code Data
;******************************************************************
				 
;******************************************************************	
#define		RF_TMf			     CTM0AF	;set RF timer overflow flag
#define		TX_OUTPUT		     PB0 	;which I/O OUTOUT DATA (ONLY BC68F2130/2140 USE DTXD)
#define		TX_OUTPUT_DATA		 PA7

;;******************************************************************
RFTimerOn		macro
           	SET     CTON		
endm            

;;******************************************************************
RFTimerOff		macro
           	CLR     CTON		
endm            












