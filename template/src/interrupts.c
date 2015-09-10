#include "interrupts.h"
#include "joypad.h"
#include "video.h"

s16 x_velocity = 0;
static u16 x = 0; // static variables do not show up in .map

static u16 prev_frame = 0;

void far NmiHandler(void){
    if(prev_frame == frame_count)
        return;

    pad1old = pad1;
    pad1 = ReadJoy1();

    if(pad1 & PAD_RIGHT){
        x_velocity += (x_velocity < 48) ? 1 : 0;
    }else if(pad1 & PAD_LEFT){
        if(x_velocity > -48){
            x_velocity -= 1;
        }
        //x_velocity -= (x_velocity > 48) ? 1 : 0; //neither works, wut
        //x_velocity += (x_velocity > 48) ? -1 : 0; //be careful with ternary ops
    }else{
        x_velocity += (x_velocity > 0) ? -1 : (x_velocity < 0 ? 1 : 0);
    }

    x += x_velocity;

    REG_BG1HOFS = x>>4;
    REG_BG1HOFS = 0;

    Pulse();

    REG_INIDISP = brightness;
    prev_frame = frame_count;
}

void far IrqHandler(void){

}
