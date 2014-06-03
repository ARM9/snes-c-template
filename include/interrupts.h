#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include <libsnes.h>

#include "joypad.h"
#include "video.h"

void far NMI_Handler(void);
void far IRQ_Handler(void);

#endif //INTERRUPTS_H
