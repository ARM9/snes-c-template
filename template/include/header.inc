
; Size predefs for ROM/RAM/SRAM fields
SIZE_NONE       .set $00
SIZE_2KiB       .set $01 ; amount of cart RAM in Super Mario World
SIZE_4KiB       .set $02
SIZE_8KiB       .set $03
SIZE_16KiB      .set $04
SIZE_32KiB      .set $05 ; Amount of cart RAM in Mario Paint
SIZE_64KiB      .set $06 ; Amount of cart RAM in Stunt Race FX
SIZE_128KiB     .set $07 ; Amount of cart RAM in Dezaemon - Kaite Tsukutte Asoberu
SIZE_256KiB     .set $08
SIZE_512KiB     .set $09 ; Amount of ROM in Super Mario World
SIZE_1MiB       .set $0A ; Amount of ROM in Mario Paint
SIZE_2MiB       .set $0B ; Amount of ROM in Super Mario World 2
SIZE_4MiB       .set $0C ; Amount of ROM in Donkey Kong Country 2 and 3

; Map modes
MAP_LOROM   .set $20
MAP_HIROM   .set $21
MAP_SDD1    .set $22
MAP_EXHIROM .set $25
MAP_SPC7110 .set $2A
MAP_FASTROM .set $10    ; OR with one of the above for FastROM (3.58MHz)
MAP_SA1ROM  .set $23

; Cartridge types
CART_ROM            .set $00 ; ROM only
CART_ROM_RAM        .set $01 ; ROM+RAM
CART_ROM_RAM_BAT    .set $02 ; ROM+RAM+Battery
; Coprocessors (add with one of the above)
CART_DSP1           .set $03 ; DSP-1
CART_GSU            .set $13 ; SuperFX
CART_OBC1           .set $23 ; OBC-1
CART_SA1            .set $33 ; SA-1
CART_SDD1           .set $43 ; S-DD1
CART_SRTC           .set $53 ; S-RTC
CART_OTHER          .set $E3 ; \ Todo: determine how/if emulators use these
CART_CUSTOM         .set $F3 ; | to distinguish between obscure stuff like the
                             ; / CX4, ST-0018, SPC7110 etc.
CART_ROM_RAM_BAT_RTC .set $06 ; ROM+RAM+Battery+RTC-4513 for SPC7110

; Destination codes, most emulators use this to determine PAL/NTSC
DEST_JAPAN          .set $00
DEST_USA            .set $01
DEST_EUROPE         .set $02
DEST_SCANDANAVIA    .set $03
DEST_FRENCH_EUROPE  .set $06
DEST_DUTCH          .set $07
DEST_SPANISH        .set $08
DEST_GERMAN         .set $09
DEST_ITALIAN        .set $0A
DEST_CHINESE        .set $0B
DEST_KOREAN         .set $0D
DEST_COMMON         .set $0E
DEST_CANADA         .set $0F
DEST_BRAZIL         .set $10
DEST_AUSTRALIA      .set $11

MAP_MODE    .equ MAP_HIROM|MAP_FASTROM

SNESHEADER .section offset $FFB0
; Extended header
    dw  $0000       ; B0 - Maker code
    db  "ARM9"      ; B2 - Game code
    ds  7,0         ; B6 - fill 7 bytes
    db  SIZE_NONE   ; BD - Ext cart RAM, used by emulators to detect GSU cart RAM
    db  0           ; BE - Special version(?)
    db  0           ; BF - Cartridge sub-number(?)
    .org $FFC0

    db  "SNES C template      " ; C0 - ROM Title 21 bytes
;       "                     "
    .org $FFD5
    db  MAP_MODE            ; D5 - Map mode
    db  CART_ROM            ; D6 - Cart type
    db  SIZE_256KiB         ; D7 - ROM size
    db  SIZE_NONE           ; D8 - Cart RAM size
    db  DEST_USA            ; D9 - Destination code (NTSC/PAL)
    db  $33                 ; DA - License code, some emulators care about this in edge cases, 0 or $33
    db  $00                 ; DB - Version number (v 1.xx)
    dw  $0000               ; DC - Checksum
    dw  $0000               ; DE - Complement
    .ENDS

VECTORS .section offset $FFE4
    dw  _native_cop
    dw  _native_brk
    dw  _native_abort
    dw  _NmiVector
    dw  _native_unused
    dw  _IrqVector
    ds  4

    dw  _emu_cop
    dw  _emu_unused
    dw  _emu_abort
    dw  _emu_nmi
    dw  _Reset
    dw  _emu_irq

    .ENDS

    .END

