#include "RX.h"


bit new_data_flag=0;
uint8 bit_index=0;  
uint8 Rx_data[12];         
uint8 count_low=0;
uint8 count_high=0;
uint8 pulse_width_low=0;
uint8 pulse_width_high=0;
bit preamble_detected=0;
uint8 preamble_count=0;


void RX_change()
{

}

