#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include <libsnes.h>

void far NmiHandler(void);
void far IrqHandler(void);

#endif //INTERRUPTS_H
