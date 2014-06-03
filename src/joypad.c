#include "joypad.h"

u16 pad1 = 0, pad1old;

int ReadJoy1(void){
	while(REG_HVBJOY & 1){
	}
	return REG_JOY1L | (REG_JOY1H << 8);
}
