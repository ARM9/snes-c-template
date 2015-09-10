#ifndef __LIBSNES_H
#define __LIBSNES_H

#include "int.h"
#include "snes_regs.h"

extern void far WaitForVSync(void);
#define WaitForInterrupt asm wai
// test
#define WaitForHBlank asm{db $E2, $20, $A9, $40, $2C, $12, $42, $F0, $FB;}//, $C2, $20

#define BIT(x) (1<<x)

#define RGB15(r, g, b) (b << 10) | (g << 5) | (r)

//todo: move to pad header
#define PAD_A       0x0080
#define PAD_X       0x0040
#define PAD_L       0x0020
#define PAD_R       0x0010
#define PAD_B       0x8000
#define PAD_Y       0x4000
#define PAD_SELECT  0x2000
#define PAD_START   0x1000
#define PAD_UP      0x0800
#define PAD_DOWN    0x0400
#define PAD_LEFT    0x0200
#define PAD_RIGHT   0x0100

//DMA transfer macros
#define DMA_TYPE(chn,x) (*(unsigned int*)(0x4300 | ((chn) << 4))) = (x)
#define DMA_ADDR(chn,x) (*(void**)(0x4302 | ((chn) << 4))) = (x)
#define DMA_SIZE(chn,x) (*(unsigned int*)(0x4305 | ((chn) << 4))) = (x)

#define dmaTransfer(type, addr, size) DMA_TYPE((0), (type)); \
        DMA_ADDR((0), (addr)); \
        DMA_SIZE((0), (size)); \
        REG_MDMAEN = BIT(0);

#endif //__LIBSNES_H

