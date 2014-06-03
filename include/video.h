#ifndef VIDEO_H
#define VIDEO_H
#include <libsnes.h>

#include "assets.h"

// vram constants
#define BG1_TILESRC	0x0000
#define BG1_MAPSRC	0x2000

//gotta test structs
typedef struct {
	u8 colors;
	u8 intensity;
}cmath;

extern u16 frame_count;

extern u8 brightness; //INIDISP mirror

void SetupVideo(void);
void Pulse();

#endif //VIDEO_H
