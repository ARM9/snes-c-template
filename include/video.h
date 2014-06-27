#ifndef VIDEO_H
#define VIDEO_H
#include <libsnes.h>


// vram constants
#define BG1_TILESRC	0x0000
#define BG1_MAPSRC	0x2000

void SetupVideo(void);
void Pulse();

//gotta test structs
typedef struct {
	u8 colors;
	u8 intensity;
}cmath;

extern u16 frame_count;

extern u8 brightness; //INIDISP mirror

#endif //VIDEO_H
