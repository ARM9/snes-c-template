/*
*   SNES MMIO register definitions
*/

#ifndef __SNES_REGS_H
#define __SNES_REGS_H
#include "int.h"

#define u8_reg(addr) (*(vu8*)addr)
#define u16_reg(addr) (*(vu16*)addr)
#define s8_reg(addr) (*(vs8*)addr)
#define s16_reg(addr) (*(vs16*)addr)

#define REG_INIDISP     (*(vu8*)0x2100) // f--- xxxx, Force vblank and set screen brightness

//OAM registers
#define REG_OBJSEL      (*(vu8*)0x2101) // sssn nbbb, s = obj size, n = name select b = name base addr>>14
#define REG_OBSEL       (*(vu8*)0x2101)
#define REG_OAMADD      (*(vu16*)0x2102)// OAM address pointer to 16 bits see next line
#define REG_OAMADDL     (*(vu8*)0x2102) // aaaa aaaa, a = OAM address low byte
#define REG_OAMADDH     (*(vu8*)0x2103) // p--- ---b, OAM address high bit and priority
/*
 Object size and tile address 
    sssnnbbb
        sss = size
        000 =  8x8  and 16x16 sprites
        001 =  8x8  and 32x32 sprites
        010 =  8x8  and 64x64 sprites
        011 = 16x16 and 32x32 sprites
        100 = 16x16 and 64x64 sprites
        101 = 32x32 and 64x64 sprites
    nn = name select, bbb = name base select
 Each sprite entry in oam is 4 bytes (+2 bits in the high table), format is:
    1 xxxx xxxx
    2 yyyy yyyy
    3 tttt tttt Note that this could also be considered as 'rrrrcccc' specifying the row and column of the tile in the 16x16 character table.
    4 hvoo pppN N = msb of tile number, p = set palette 0-7, o = priority, h/v flip
 Each byte in the high table contains settings for 4 sprites, ie 2 bits per sprite.
    first bit = X "sign" bit for x position 
    second bit = S size bit, used to determine which size to use, each sss setting in $2101 has two different sizes see above. Set to 0 for small 1 for large
 Sprite tile table in vram:
 The first table is at the address specified by the Name Base bits of $2101, and the offset of the second is determined by the Name bits of $2101.
 The word address in VRAM of a sprite's first tile may be calculated as:
 ((Base<<13) + (cccccccc<<4) + (N ? ((Name+1)<<12) : 0)) & 0x7fff
 
 Changing sprite priority sprite to sprite (not sprite to BG, see byte 4 of sprite entry for this):
 Write 1 to b to index the high table
 p can be used for rotating priority order in OAM
 step 1: set p to 1
 step 2: write the highest priority OBJ number (0-127) to $2102
 during V-blank every frame
 step 3: repeat
*/
#define REG_OAMDATA     (*(vu8*)0x2104) // bbbb bbbb, OAM Data write register
// Write 1 byte at a time to OAM, changes won't take effect until a full word is written (so never write an uneven amount of bytes)

#define REG_BGMODE      (*(vu8*)0x2105) // DCBA emmm, e = mode 1 bg3 prio bit, mmm = BG mode
// A/B/C/D = tile size for BG1/BG2/BG3/BG4, if set 16x16 else 8x8
#define REG_MOASIC      (*(vu8*)0x2106) // xxxx DCBA, xxxx = size, DCBA = bg 4321

//BG tile and map vram address set registers
// a = map address>>10, x = horizontal flip, y = vertical flip
#define REG_BG1SC       (*(vu8*)0x2107) // aaaa aayx, 1k address boundaries
#define REG_BG2SC       (*(vu8*)0x2108) // aaaa aayx $0000, $0400, $0800, $0C00, $1000 etc.
#define REG_BG3SC       (*(vu8*)0x2109) // aaaa aayx
#define REG_BG4SC       (*(vu8*)0x210A) // aaaa aayx
#define REG_BG12NBA     (*(vu8*)0x210B) // bbbb aaaa, tile address>>12 (4k boundary), a = BG1, b = BG2
#define REG_BG34NBA     (*(vu8*)0x210C) // bbbb aaaa, a = BG3, b = BG4

//BG Scroll registers
// The registers for BG1 behave differently in mode 7 from mode 0-6, see below
#define REG_M7HOFS      (*(vu8*)0x210D) // \ ---x xxxx xxxx xxxx, signed 13 bit twos complement
#define REG_M7VOFS      (*(vu8*)0x210E) // | ---y yyyy yyyy yyyy, signed 13 bit twos complement
#define REG_BG1HOFS     (*(vu8*)0x210D) // | ---- --xx xxxx xxxx, unsigned, 10 bit x scroll
#define REG_BG1VOFS     (*(vu8*)0x210E) // | ---- --yy yyyy yyyy, unsigned, 10 bit y scroll
#define REG_BG2HOFS     (*(vu8*)0x210F) // | all bgs use same format as BG1 (non-mode7)
#define REG_BG2VOFS     (*(vu8*)0x2110) // | 
#define REG_BG3HOFS     (*(vu8*)0x2111) // |-write twice registers
#define REG_BG3VOFS     (*(vu8*)0x2112) // | 
#define REG_BG4HOFS     (*(vu8*)0x2113) // | 
#define REG_BG4VOFS     (*(vu8*)0x2114) // / 

//VRAM registers
#define REG_VMAIN       (*(vu8*)0x2115) // i--- mmaa
/* Using VMAIN:
 i = set to 0/1 for increment after writing $2118/2119
 mm = Address translation
 aa = Address increment step (0..3 = Increment Word-Address by 1,32,128,128)
    m m a a     Increment value for $2116 and $2117
    0 1 0 0     Increment by 8 (for 32 times) (4 color mode)
    1 0 0 0     Increment by 8 (for 64 times) (16 color mode)
    1 1 0 0     Increment by 8 (for 128 times) (256 color mode)
    0 0 0 0     Address Increments 1x1
    0 0 0 1     Address Increments 32x32
    0 0 1 0     Address Increments 128x128 (64x64?)
    0 0 1 1     Address Increments 128x128
 address increment > 0 (with/without translation) is useful for updating BG Map columns? (eg. after horizontal scrolling).
 $89 to write 4bpp column, $85 seems to work as well?
*/
#define REG_VMADD       (*(vu16*)0x2116)// VRAM address, pointer to 16 bits
#define REG_VMADDL      (*(vu8*)0x2116) // Low byte of VRAM read/write address
#define REG_VMADDH      (*(vu8*)0x2117) // High byte of VRAM read/write address
#define REG_VMDATA      (*(vu16*)0x2118)// VRAM data write, pointer to 16 bits
#define REG_VMDATAL     (*(vu8*)0x2118) // Write register for low VRAM bank
#define REG_VMDATAH     (*(vu8*)0x2119) // Write register for high VRAM bank
// VRAM writes are done as such: write $2118, write 2119, write 2118, write 2119 etc.

//Mode7 settings and matrix
#define REG_M7SEL       (*(vu8*)0x211A) // rc-- --yx
/* Using M7SEL:
 r = When clear, the playing field is 1024x1024 pixels (so the tilemap completely fills it). When set, the playing field is much larger, and the ‘empty space’ fill is controlled by bit 6
 c = Empty space fill, when bit r is set: 0 = Transparent. 1 = Fill with character 0.
 x/y = Horizontal/Veritcal mirroring. If the bit is set, flip the 256x256 pixel 'screen' in that direction.

 Mode7 affine matrix, all are write twice registers
 M7A and M7B can also be used for signed multiplication.
 Write a 16 bit value to M7A and an 8 bit value to M7B and the 24 bit product
 can be read from registers $2134 to $2136 with trivial delay.
 However this is not possible during scanning in mode 7.
*/
#define REG_M7A         (*(vu8*)0x211B) // \  aaaa aaaa aaaa aaaa
#define REG_M7B         (*(vu8*)0x211C) // |  bbbb bbbb bbbb bbbb
#define REG_M7C         (*(vu8*)0x211D) // |  you get the idea
#define REG_M7D         (*(vu8*)0x211E) // | 8.8 fixed two's complement
                                        // | write twice registers
#define REG_M7X         (*(vu8*)0x211F) // |  ---x xxxx xxxx xxxx 
#define REG_M7Y         (*(vu8*)0x2120) // /  ---y yyyy yyyy yyyy
// M7X/Y are 13 bit two's complement signed, origin X,Y for mode7 affine transformation

//CGRAM registers
#define REG_CGADD       (*(vu8*)0x2121) // cccc cccc, destination address in CGRAM for read/write
#define REG_CGDATA      (*(vu8*)0x2122) // color to be written, write twice register low byte then high
// Color format is 15 bits 5x5x5 BGR as such: -bbb bbgg gggr rrrr

/* Window mask settings
 The following 3 registers have the format:
    ABCD abcd
        c/a = Enable window 1/2 for BG1/BG3/OBJ
        C/A = Enable window 1/2 for BG2/BG4/Color
        d/b = Window 1/2 Inversion for BG1/BG3/OBJ
        D/B = Window 1/2 Inversion for BG2/BG4/Color
 Example: window 2 enabled for BG1 outside and window 1 enabled for BG2 inside
 lda #%00101100
 sta $2123
*/
#define REG_W12SEL      (*(vu8*)0x2123) // 
#define REG_W34SEL      (*(vu8*)0x2124) // 
#define REG_WOBJSEL     (*(vu8*)0x2125) // 

//Window positions, all 4 are 1 byte unsigned
#define REG_WH0         (*(vu8*)0x2126) // Window 1 Left Position
#define REG_WH1         (*(vu8*)0x2127) // Window 1 Right Position
#define REG_WH2         (*(vu8*)0x2128) // Window 2 Left Position
#define REG_WH3         (*(vu8*)0x2129) // Window 2 Right Position

//Window BG, OBJ and Color logic
//  44/33/22/11/oo/cc = Mask logic for BG1/BG2/BG3/BG4/OBJ/Color
#define REG_WBGLOG      (*(vu8*)0x212A) // 4433 2211
#define REG_WOBJLOG     (*(vu8*)0x212B) // ---- ccoo
#define REG_TM          (*(vu8*)0x212C) // ---o 4321 layer main screen enable
#define REG_TS          (*(vu8*)0x212D) // ---o 4321 layer sub screen enable
#define REG_TMW         (*(vu8*)0x212E) // ---o 4321 Window Mask Designation for the Main Screen
#define REG_TSW         (*(vu8*)0x212F) // ---o 4321 Window Mask Designation for the Subscreen

//Color math registers
#define REG_CGWSEL      (*(vu8*)0x2130) // Color Addition Select
#define REG_CGADSUB     (*(vu8*)0x2131) // Color math designation
#define REG_COLDATA     (*(vu8*)0x2132) // Color for color math

//Various ppu registers, investigate further on your own if you want to know more
#define REG_SETINI      (*(vu8*)0x2133) // Screen Mode/Video Select

#define REG_MPYLM       (*(vs16*)0x2134)// Mode7 A*B product, pointer to low 16 bits
#define REG_MPYMH       (*(vs16*)0x2135)// product, pointer to high 16 bits
#define REG_MPYL        (*(vu8*)0x2134) // low byte
#define REG_MPYM        (*(vu8*)0x2135) // mid byte
#define REG_MPYH        (*(vu8*)0x2136) // high byte
#define REG_SLHV        (*(vu8*)0x2137) // When read, the H/V counter (as read from $213C and $213D) will be latched to the current X and Y position if bit 7 of $4201 is set. The data actually read is open bus.
#define REG_OAMDATAREAD (*(vu8*)0x2138) // 
#define REG_VMDATAREAD  (*(vu16*)0x2139)// 
#define REG_VMDATALREAD (*(vu8*)0x2139) // 
#define REG_VMDATAHREAD (*(vu8*)0x213A) // 
#define REG_CGDATAREAD  (*(vu8*)0x213B) // 
/* $2137, $213C and $213D
  Operation goes something like this:
    Read register $2137 to latch counter
    Read register twice for position (9 bits, bits 1-7 of high byte are ppu2 open bus)
    Each register ($213C, $213D) keeps seperate track of whether to return the low or high byte. The high/low selector is reset to ‘low’ when $213f is read (the selector is NOT reset when the counter is latched). 
    H Counter values range from 0 to 339, with 22-277 being visible on the screen. V Counter values range from 0 to 261 in NTSC mode (262 is possible every other frame when interlace is active) and 0 to 311 in PAL mode (312 in interlace?), with 1-224 (or 1-239(?) if overscan is enabled) visible on the screen.*/
#define REG_OPHCT   (*(vu8*)0x213C) // ---- ---h hhhh hhhh, Horizontal scanline location
#define REG_OPVCT   (*(vu8*)0x213D) // ---- ---v vvvv vvvv, Vertical scanline location

#define REG_STAT77  (*(vu8*)0x213E) // trm- vvvv, OAM overflow flags
#define REG_STAT78  (*(vu8*)0x213F) // fl-p vvvv, if p is 1 then PAL else NTSC

//SPC700 IO
#define REG_APUIO0  (*(vu8*)0x2140) // Always write to $2140 / $2141 with 8-bit writes
#define REG_APUIO1  (*(vu8*)0x2141) // 
#define REG_APUIO2  (*(vu8*)0x2142) // 
#define REG_APUIO3  (*(vu8*)0x2143) // 

//WRAM B-bus registers
#define REG_WMDATA  (*(vu8*)0x2180) // Read/write to/from WRAM on B bus (fast access), one byte at a time
#define REG_WMADD   (*(vu16*)0x2181 // WMADDL&M pointer to 16 bits
#define REG_WMADDL  (*(vu8*)0x2181) // low byte of address into wram
#define REG_WMADDM  (*(vu8*)0x2182) // high byte
#define REG_WMADDH  (*(vu8*)0x2183) // bank byte, only one bit because there are only two banks ($7E/$7F)

#define REG_DEBUG   (*(vu8*)0x21FC) //no$sns debug register, need to investigate usage

//Slow joypad access port
#define REG_JOYSER0 (*(vu8*)0x4016) // 
#define REG_JOYSER1 (*(vu8*)0x4017) // 

//CPU internal registers
#define REG_NMITIMEN (*(vu8*)0x4200) // n-vh ---a 
#define REG_WRIO    (*(vu8*)0x4201) // 
// Hardware multiplication, A*B, 16 bit product in REG_RDMPY
// Both multiplication and division are unsigned (see matrix registers for signed mult)
#define REG_WRMPYA  (*(vu8*)0x4202) // A
#define REG_WRMPYB  (*(vu8*)0x4203) // B
// Hardware division, A/B, quotient in REG_RDDIV, remanider in REG_RDMPY
#define REG_WRDIVA  (*(vu16*)0x4204) // Dividend, pointer to 16 bits
#define REG_WRDIVL  (*(vu8*)0x4204) // Dividend low byte
#define REG_WRDIVH  (*(vu8*)0x4205) // Dividend high byte
#define REG_WRDIVB  (*(vu8*)0x4206) // Divisor

#define REG_HTIME   (*(vu16*)0x4207)// pointer to 16 bits
#define REG_HTIMEL  (*(vu8*)0x4207) // H-IRQ position
#define REG_HTIMEH  (*(vu8*)0x4208) // msb of H-IRQ position 9 bit value
#define REG_VTIME   (*(vu16*)0x4209)// pointer to 16 bit
#define REG_VTIMEL  (*(vu8*)0x4209) // V-IRQ
#define REG_VTIMEH  (*(vu8*)0x420A) // msb of V-IRQ scanline 9 bit value
#define REG_MDMAEN  (*(vu8*)0x420B) // Enable DMA transfer on channels 7654 3210
#define REG_HDMAEN  (*(vu8*)0x420C) // Enable HDMA transfer, uses same channels as DMA
#define REG_MEMSEL  (*(vu8*)0x420D) // ---- ---s 0 = slow, 1 = fast rom access

#define REG_RDNMI   (*(vu8*)0x4210) // read to clear NMI
#define REG_TIMEUP  (*(vu8*)0x4211) // read to clear IRQ (actually important)
#define REG_HVBJOY  (*(vu8*)0x4212) // vh-- ---a v=vblank h=hblank a=autojoy status
#define REG_RDIO    (*(vu8*)0x4213) // ab-- ----
// a = controller 1 pin 6, b = controller 2 pin 6
// Reading this register reads data from the I/O Port.
// The way the I/O Port works, any bit set to 0 in $4201 will be 0 here.
// Any bit set to 1 in $4201 may be 1 or 0 here, depending on whether any
// other device connected to the I/O Port has set a 0 to that bit.
// Bit b is connected to pin 6 of Controller Port 1. Bit a is connected
// to pin 6 of Controller Port 2, and to the PPU Latch line.
// See register $4201 for the O side of the I/O Port

#define REG_RDDIV   (*(vu16*)0x4214) // Quotient of divide, pointer to 16 bit
#define REG_RDDIVL  (*(vu8*)0x4214) // qqqq qqqq, low byte of quotient
#define REG_RDDIVH  (*(vu8*)0x4215) // qqqq qqqq, high byte of quotient

#define REG_RDMPY   (*(vu16*)0x4216) // Product/remainder pointer to 16 bit
#define REG_RDMPYL  (*(vu8*)0x4216) // Multiplication product or divide remainder low byte
#define REG_RDMPYH  (*(vu8*)0x4217) // high byte

// Joypad read registers
#define REG_JOY1L   (*(vu8*)0x4218) // axlr0000 = A/X/L/R button status
#define REG_JOY1H   (*(vu8*)0x4219) // byetUDLR = B/Y/SELECT/START/UP/DOWN/LEFT/RIGHT
#define REG_JOY2L   (*(vu8*)0x421A) // 
#define REG_JOY2H   (*(vu8*)0x421B) // 
#define REG_JOY3L   (*(vu8*)0x421C) // 
#define REG_JOY3H   (*(vu8*)0x421D) // 
#define REG_JOY4L   (*(vu8*)0x421E) // 
#define REG_JOY4H   (*(vu8*)0x421F) // 


/*
//DMA/HDMA control registers
//Just use u8/16_reg(0x43xx)
#define REG_        (*(vu8*)0x4300) // 
#define REG_        (*(vu8*)0x4301) // 
#define REG_        (*(vu8*)0x4302) // 
#define REG_        (*(vu8*)0x4303) // 
#define REG_        (*(vu8*)0x4304) // 
#define REG_        (*(vu8*)0x4305) // 
#define REG_        (*(vu8*)0x4306) // 
#define REG_        (*(vu8*)0x4307) // 
#define REG_        (*(vu8*)0x4308) // 
#define REG_        (*(vu8*)0x4309) // 
#define REG_        (*(vu8*)0x430A) // 
*/

/*  <---Registers used for DMA--->
 DMA transfers take 8 master cycles per byte transferred, no matter the FastROM setting.
 There is also an overhead of 8 master cycles per channel, and an overhead of 12-24 cycles for the whole transfer.
DMA Control Register            $43x0 ; d--i fttt
    d = Transfer direction, 0 = A bus to B bus, 1 = B to A.
    i = Address increment, 0 = increment DMA address for each byte. 1 = decrement
    f = DMA fixed transfer
        0 = source address will be adjusted as specified by bit 4
        1 = source address will not be adjusted
    t = How data is written/read, see table below (dest = byte in $43x1):
    000 => 1 register write once             (1 byte:  p               )
    001 => 2 registers write once            (2 bytes: p, p+1          )
    010 => 1 register write twice            (2 bytes: p, p            )
    011 => 2 registers write twice each      (4 bytes: p, p,   p+1, p+1)
    100 => 4 registers write once            (4 bytes: p, p+1, p+2, p+3)
    101 => 2 registers write twice alternate (4 bytes: p, p+1, p,   p+1)
    110 => 1 register write twice            (2 bytes: p, p            )
    111 => 2 registers write twice each      (4 bytes: p, p,   p+1, p+1)

DMA Destination             $43x1 ; aaaa aaaa
    a = Destination register for DMA write on address bus B ($21xx)

DMA Source Address (Low)        $43x2 ; llll llll
DMA Source Address (High)       $43x3 ; hhhh hhhh
DMA Source Bank             $43x4 ; bbbb bbbb
    bhl = bank:address for the source on A bus in DMA transfer (eg. $80BEEF)

DMA Transfer Size (Low)     $43x5 ; llll llll
DMA Transfer Size (High)        $43x6 ; hhhh hhhh
 Size of the transfer, 2 bytes ($0000 = 64k transfer,$8000 = 32k transfer)


    <---Registers used for HDMA--->
 HDMA overhead is ~18 master cycles, plus 8 master cycles for each channel set for direct HDMA and 24 master cycles for each channel set for indirect HDMA.

HDMA Control Register           $43x0 ; da-- -ttt
    d and t are same as DMA
    a = Set to 1 for indirect addressing, 0 for direct.

HDMA Destination                $43x1 ; aaaa aaaa
    a = same as DMA
HDMA Table Address (Low)        $43x2 ; llll llll
HDMA Table Address (High)       $43x3 ; hhhh hhhh
HDMA Table Bank             $43x4 ; bbbb bbbb
    bhl = same as DMA

HDMA Indirect Address (Low) $43x5 ; llll llll
HDMA Indirect Address (High)    $43x6 ; hhhh hhhh
HDMA Indirect Bank              $43x7 ; bbbb bbbb
    bhl = bank:address for indirect HDMA (eg. $7EFACE)
    You only need to set the bank byte for indirect HDMA.
    The resulting bank:address is the pointer used in the HDMA transfer.

HDMA Mid Frame Table Address (Low) $43x8 ; aaaa aaaa, 
HDMA Mid Frame Table Address (High)$43x9 ; aaaa aaaa, 
    At the beginning of the frame $43x2/3 are copied into $43x8/9 registers for all active HDMA channels,
    and then this register is updated as the table is read.

HDMA Line Counter Register          $43xA ; rccc cccc, r = Repeat, c = Line count
    About $43xA:
    The register is decremeted before being checked for r status or c==0.
    Thus, setting a value of $80 is really "128 lines with no repeat" rather than "0 lines with repeat".
    Similarly, a value of $00 will be "128 lines with repeat" when it doesn't mean "terminate the channel"(assuming this can only happen if you manually write $43xA?).
    This register is initialized at the end of V-Blank for every active HDMA channel.
    Note that if a game wishes to begin HDMA during the frame, it will most likely have to initalize this register.
*/

#endif //__SNES_REGS_H

