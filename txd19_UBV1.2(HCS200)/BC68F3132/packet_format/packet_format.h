#ifndef _PACKET_FORMAT_H_
#define _PACKET_FORMAT_H_

#include "ui.h"
#include "BC68F3132.h"


//For UI
#define				FRAME_COUNT				PKT_FRAME_LOOP				//1~255
#define				GUARD_COUNT				PKT_GUARD_TIME				//12~255
#define				BACW					PKT_BACW					//0/1/2	
#define				MaxLongPress		    ((PKT_MAX_FRAME-1)*PKT_FRAME_LOOP)  

#if (PKT_MAX_FRAME >= 255)
#define 			LongPress_transmit			1
#else
#define 			LongPress_transmit			0
#endif

#endif