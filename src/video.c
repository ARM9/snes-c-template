#include "video.h"
#include "assets.h"

u16 frame_count = 0;

u8 brightness = 0;

// Trivial video initialization routine
void SetupVideo(void){
	//asm{pea #$807E;plb;}
	//asm{plb;}
	REG_VMAIN = 0x80; // crt0 already sets fblank
	REG_VMADD = BG1_TILESRC;
	dmaTransfer(0x1801, &ocean_tiles, &ocean_tiles_size - &ocean_tiles);
	
	REG_VMADD = BG1_MAPSRC;
	dmaTransfer(0x1801, &ocean_map, &ocean_map_size - &ocean_map);
	
	REG_CGADD = 0;
	dmaTransfer(0x2200, &ocean_pal, 32);
	
	//vram addresses
	REG_BG1SC	= (BG1_MAPSRC >> 8) & 0xFC;
	REG_BG12NBA	= (0x00) | (BG1_TILESRC >> 12);
	
	REG_BGMODE	= 0x01;
	REG_TM		= 0x01;
	REG_TMW		= 0x01;
	
	REG_CGWSEL	= 0;
	REG_CGADSUB	= 0x80|1;
	
	// Done, turn on nmi, autojoy, brightness and enable interrupts
	REG_NMITIMEN = 0x81;
	brightness = 0x0F;
	asm{cli;}
}

void Pulse(){
	static u8 color_dir = 0;
	static cmath color = {0xA0,0};

	REG_COLDATA = color.colors | color.intensity;
	
	if(frame_count&2){
		if(color_dir == 1){
			if(color.intensity < 0x0f){
				++color.intensity;
			}else{
				color_dir = 0;
			}
		}else{
			if(color.intensity > 0){
				--color.intensity;
			}else{
				color_dir = 1;
			}
		}
	}
}

