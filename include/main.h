#ifndef MAIN_H
#define MAIN_H

#include <stddef.h>
#include <stdlib.h>
#include <libsnes.h>

#include "assets.h"
#include "interrupts.h"
#include "joypad.h"
#include "video.h"

void *far_heap_start = (void*)0x7F0000, *far_heap_end = (void*)0x7FFFFF;


#endif //MAIN_H
