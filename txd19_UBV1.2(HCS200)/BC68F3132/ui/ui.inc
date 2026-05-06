INCLUDE BC68F3132.inc
extern _MRFDATA :byte 
;=========================================================================================================================
;RF
#define				RF_FREQ						39000000				;=RFfreq/10
#define				RF_XTAL_CL					32						;~16pF, internal capacitor load for the crystal
#define				RF_FSKOOK					0						;0:OOK  1:FSK
#define				RF_PWR						3						;0~3	:0/5/10/13dbm (level0)
#define				RF_DM_SYMBOL_TIME		 	400						;Make Sure RF Timer Interruput is 100/200/400us
;=========================================================================================================================
#define				RF_BACW						0						;0: BACW all;     1: BACW 1/2;   2: BACW 1/4;    
#define				RF_GUARD_TIME				39						;12~255		 		     PKT_GUARD_TIME*SYMBOL_TIME 
;=========================================================================================================================
;10/12-bit USER_DISC
#define				HOP_USER_DISC				0000H
;=========================================================================================================================
;table address
#define				HOP_USER_DISC_TABLE_ADDR		(LASTPAGE+0FCh)		;User Discrimination table address
;=========================================================================================================================
;Set 64-bit manufacture code                    
					
;Set 10/12-bit user discrimination code
HOP_USER_DISC_TABLE		.SECTION AT	HOP_USER_DISC_TABLE_ADDR  'code'
HOP_USER_DISC_TABLE:
					DC		HOP_USER_DISC & 0FFH 
					DC		(HOP_USER_DISC >> 8) & 0FH ;MSB
					
;=========================================================================================================================	 
