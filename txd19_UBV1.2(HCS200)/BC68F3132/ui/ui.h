#ifndef _UI_H_
#define _UI_H_

#include "ui.h"

//===============================================================================================================
// hopping code
#define HOP_PACKET_TYPE 1  // 1:Standard    	         0:HT_Enhanced
#define HOP_HOPPING_TYPE 0 // 1:Normal_Learn    	     0:Simple_Learn
#define HOP_NLF 0x3A5C742E // NLF		 32bit
#define HOP_LOOP_COUNT 528 // 1~65535
#define HOP_SYNC_COUNTER 0 // 16bit
//===============================================================================================================

//===============================================================================================================
// packet fprmat
#define PKT_FRAME_LOOP 1  // 1~255
#define PKT_MAX_FRAME 80  // 1~255         PKT_MAX_FRAME*PKT_FRAME_LOOP
#define PKT_GUARD_TIME 39 // 12~255		 		     PKT_GUARD_TIME*SYMBOL_TIME
#define PKT_BACW 0        // 0: BACW all;   		     1: BACW 1/2;   2: BACW 1/4;

//===============================================================================================================

//===============================================================================================================
// DISC and OVR
#define HOP_OVR_TYPE 0    // 0: None(00) 	1: Once(01) 	2: Twice(11)
#define HOP_DISC_TYPE 0   // 0: use Serial Number 		1: by user input
#define HOP_DISC_LENGTH 1 // 0: 10bit		1: 12bit
//===============================================================================================================

//===============================================================================================================
// app
#define APP_MCU_PACKAGE 1 // only1

// if APP_LED_EN enable , please turn off the APP_IO9_EN
#define APP_LED_EN 1 // 1: LED enable	             0:disable

// if APP_INTERNAL_LVD_EN enable , please turn off the APP_IO8_EN
#define APP_EXTERNAL_LVD_EN 0 // 1: External_LVD_EN enable	 0:disable
#define APP_INTERNAL_LVD_EN 1 // 1: Internal_LVD_EN enable	 0:disable
#define APP_INTERNAL_LVD 2    // if Internal_LVD_EN enable, 0~3 : 2.0V~2.7V
// app.key
// if APP_KEY_MODE =1(Direct),please turn off the APP_IO9_EN
#define APP_KEY_MODE 1 // 1:Direct    	             0:Matrix

#define APP_IO1_EN 0 //
#define APP_IO2_EN 1
#define APP_IO3_EN 1 //
#define APP_IO4_EN 1 //
#define APP_IO5_EN 0
#define APP_IO6_EN 0 // if debug , please turn off the APP_IO5_EN & APP_IO6_EN
#define APP_IO7_EN 1 //
#define APP_IO8_EN 0 // 1:enable IOx			     0:disable IOx
#define APP_IO9_EN 0 // turn off the external LVD/LED when using IO8&IO9

#define APP_KEY1 8
#define APP_KEY2 4
#define APP_KEY3 6
#define APP_KEY4 2
#define APP_KEY5 9
#define APP_KEY6 10
#define APP_KEY7 11
#define APP_KEY8 12
#define APP_KEY9 13
#define APP_KEY10 10
#define APP_KEY11 11
#define APP_KEY12 12
#define APP_KEY13 13
#define APP_KEY14 14
#define APP_KEY15 15
#define APP_KEY16 0
#define APP_KEY17 1
#define APP_KEY18 2
#define APP_KEY19 3
#define APP_KEY20 4 // Key.Value
//===============================================================================================================

#endif
