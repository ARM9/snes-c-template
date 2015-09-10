
; SNES startup

; TODO indirect nmi/irq routines
    .data
;    .xdef    ~~NmiHandlerr
;    .xdef    ~~IrqHandlerr
;~~NmiHandlerr: ds 4
;~~IrqHandlerr: ds 4
    .ends

    .code

    .xref    _BEG_DATA
    .xref    _END_DATA
    .xref    _ROM_BEG_DATA
    .xref    _BEG_UDATA
    .xref    _END_UDATA
    .xref    ~~NmiHandler
    .xref    ~~IrqHandler
    .xref    ~~main

_STACK_TOP  .equ $0FFF      ; 4K stack

_Reset:
    sei                     ; Disable interrupts

    clc                     ; Switch to native mode
    xce

    .longi  on              ; Set 16 bit index
    .longa  on              ; Set 16 bit accumulator
    rep     #$39            ; Also clear decimal flag

    ldx     #_STACK_TOP     ; Set stack address
    txs

    jml     ?init_snes      ; Reset hardware to known state

; Initialize SNES
?init_snes:
    phk
    plb     ; Set data bank to $00 or $80, enable absolute mmio addressing

    .longi  on
    .longa  off
    rep     #$10            ; Set 16 bit index
    sep     #$20            ; Set 8 bit accumulator

    lda     #$8F            ; Screen off, full brightness
    sta     $2100

    stz     $2101           ; Sprite register (size + address in VRAM)
    stz     $2102           ; Sprite registers (address of sprite memory)
    stz     $2103           ; Sprite registers (address of sprite memory)
    stz     $2105           ; Set graphics Mode 0
    stz     $2106           ; No planes, no mosiac
    stz     $2107           ; Plane 0 map VRAM location ($0000 vram)
    stz     $2108           ; Plane 1 map VRAM location
    stz     $2109           ; Plane 2 map VRAM location
    stz     $210A           ; Plane 3 map VRAM location
    stz     $210B           ; Plane 0 and 1 tile data location
    stz     $210C           ; Plane 2 and 3 tile data location
    stz     $210D           ; Plane 0 scroll x (first 8 bits)
    stz     $210D           ; Plane 0 scroll x (last 3 bits) write to reg twice
    stz     $210E           ; Plane 0 scroll y (first 8 bits)
    stz     $210E           ; Plane 0 scroll y (last 3 bits) write to reg twice
    stz     $210F           ; Plane 1 scroll x (first 8 bits)
    stz     $210F           ; Plane 1 scroll x (last 3 bits) write to reg twice
    stz     $2110           ; Plane 1 scroll y (first 8 bits)
    stz     $2110           ; Plane 1 scroll y (last 3 bits) write to reg twice
    stz     $2111           ; Plane 2 scroll x (first 8 bits)
    stz     $2111           ; Plane 2 scroll x (last 3 bits) write to reg twice
    stz     $2112           ; Plane 2 scroll y (first 8 bits)
    stz     $2112           ; Plane 2 scroll y (last 3 bits) write to reg twice
    stz     $2113           ; Plane 3 scroll x (first 8 bits)
    stz     $2113           ; Plane 3 scroll x (last 3 bits) write to reg twice
    stz     $2114           ; Plane 3 scroll y (first 8 bits)
    stz     $2114           ; Plane 3 scroll y (last 3 bits) write to reg twice

    stz     $211B           ; Mode 7 matrix parameter A register (low)
    lda     #$01
    sta     $211B           ; Mode 7 matrix parameter A register (high)
    stz     $211C           ; Mode 7 matrix parameter B register (low)
    stz     $211C           ; Mode 7 matrix parameter B register (high)
    stz     $211D           ; Mode 7 matrix parameter C register (low)
    stz     $211D           ; Mode 7 matrix parameter C register (high)
    stz     $211E           ; Mode 7 matrix parameter D register (low)
    sta     $211E           ; Mode 7 matrix parameter D register (high)

    stz     $211F           ; Mode 7 matrix parameter D register (high)
    stz     $211F           ; Mode 7 center position X register (high)
    stz     $2120           ; Mode 7 center position Y register (low)
    stz     $2120           ; Mode 7 center position Y register (high)
    stz     $2123           ; BG1 & BG2 Window mask setting register
    stz     $2124           ; BG3 & BG4 Window mask setting register
    stz     $2125           ; OBJ & Color Window mask setting register
    stz     $2126           ; Window 1 left position register
    stz     $2127           ; Window 2 left position register
    stz     $2128           ; Window 3 left position register
    stz     $2129           ; Window 4 left position register
    stz     $212A           ; BG1, BG2, BG3, BG4 Window Logic register
    stz     $212B           ; OBJ, Color Window Logic Register (or,and,xor,xnor)
    stz     $212C           ; Main Screen designation (planes, sprites enable)
    stz     $212D           ; Sub Screen designation
    stz     $212E           ; Window mask for Main Screen
    stz     $212F           ; Window mask for Sub Screen

    lda     #$30
    sta     $2130           ; Color addition & Screen addition init setting

    stz     $2131           ; Add/Sub sub designation for screen, sprite, color

    lda     #$E0            ; Color data for addition/subtraction
    sta     $2132

    stz     $2133           ; Color data for addition/subtraction

    stz     $4016

    stz     $4200           ; Reset V-blank, interrupt, joypad register

    lda     #$FF
    sta     $4201           ; Programmable I/O Port

    stz     $4202           ; Multiplicand A
    stz     $4203           ; Multiplier B
    stz     $4204           ; Multiplier C
    stz     $4205           ; Multiplicand C
    stz     $4206           ; Divisor B
    stz     $4207           ; HTIMEL
    stz     $4208           ; HTIMEH
    stz     $4209           ; VTIMEL
    stz     $420A           ; VTIMEH
    stz     $420B           ; General DMA enable (bits 0-7)
    stz     $420C           ; Horizontal DMA (HDMA) enable (bits 0-7)
    lda     #(MAP_MODE>>4)&1
    sta     $420D           ; ROM speed (slow/fast)

    ; Clear WRAM
    ldy     #$0000
    sty     $2181           ; Transfer to $7E:0000
    stz     $2183           ; Select first WRAM bank ($7E)

    ldx     #$8008          ; Fixed source byte write to $2180
    stx     $4300
    ldx     #CONST_ZERO
    lda     #^CONST_ZERO

    stx     $4302           ; DMA destination address
    sta     $4304           ; Destination bank
    sty     $4305           ; Transfer 64KiB

    lda     #$01
    sta     $420B
    nop
    sta     $420B           ; $2181-$2183 and $4305 wrap appropriately

    ldx     #$0000
    ldy     #$0000          ; 64KiB
    jsl     ClearVRAM
    jsl     ClearCGRAM
    jsl     ClearOAM

    ;lda        #$C0        ; Automatic read first two joysticks
    ;sta        $4201

    ;lda        #$81
    ;sta        $4200       ; Enable V-blank, interrupt, joypad register NOPE let the programmer do this

?init_data_section:
    .longi  on
    .longa  on
    rep     #$30
; Next, we want to copy the initialized data section from ROM to RAM. Data must been stored in ROM at address _ROM_BEG_DATA
    lda     #_END_DATA-_BEG_DATA        ; Number of bytes to copy
    beq     ?skip                       ; If none, just skip

    dec     a                           ; Decrement size for mvn

    ldx     #<_ROM_BEG_DATA             ; Load source into X
    ldy     #<_BEG_DATA                 ; Load dest into Y
    mvn     #^_ROM_BEG_DATA,#^_BEG_DATA ; Copy bytes

; Now, clear out the uninitialized data area. We assume that it is in the same bank as DATA.
; Edit: no need, clear entire wram on reset.
?skip:
;   ldx     #_END_UDATA-_BEG_UDATA  ; Get number of bytes to clear
;   beq     ?DONE                   ; Nothing to do
;
;   lda     #0                      ; Get a zero for storing
;
;   .longa  off                     ; Set 8 bit accumulator
;   sep     #$20
;
;   ldy     #_BEG_UDATA             ; Get beginning of zeros
;
;?LOOP:
;   sta     |0,Y                    ; Clear memory
;   iny                             ; Bump pointer
;   dex                             ; Decrement count
;   bne     ?LOOP                   ; Continue till done

?done:
    ;phk
    ;plb            ; MVN modifies dbr so change it back
    ; On second thought the C compiler might prefer dbr to stay in the data bank
    ; There are a number of things to take into consideration when deciding
    ; where the dbr should be located when mixing C and assembly.
    ; Including but not limited to (u)data section, far addressing,
    ; mmio register access etc.
    ; Keep dbr in data bank

    .longi  on
    .longa  on
    rep     #$30

    lda     #$0000          ; Set direct page to $0000 for no apparent reason
    tcd

    ;cli    ; leave interrupts disabled

    jsl     ~~main
    stp

; Clear VRAM (DMA Channel 0)
; X - Destination
; Y - Transfer size in bytes
ClearVRAM:
    .longi  on
    .longa  off

    lda     #$80
    sta     $2115           ; Set VRAM port to increment on writes to $2119

    stx     $2116           ; Set VRAM address

    ldx     #$1809
    stx     $4300           ; Set DMA mode to fixed source, WORD to $2118/9

    ldx #CONST_ZERO
    lda #^CONST_ZERO

    stx     $4302           ; Set source address to address of 0 word in rom
    sta     $4304           ; Set source bank
    sty     $4305           ; Set transfer size in bytes

    lda     #$01            ; Initiate transfer
    sta     $420B

    rtl

; Clear palettes (DMA Channel 0)
ClearCGRAM:
    .longi  on
    .longa  off

    stz     $2121           ; Set CGRAM Index

    ldx     #$2208
    stx     $4300           ; Set DMA mode to fixed source, BYTE to $2122

    ldx     #CONST_ZERO
    lda     #^CONST_ZERO

    ldy     #$0200

    stx     $4302
    sta     $4304
    sty     $4305

    lda     #$01
    sta     $420B

    rtl

; Clear OAM
ClearOAM:
    pha
    php

    .longi  on
    .longa  off
    rep #$10
    sep #$20

    ldx     #$0000
    stx     $2102

    lda     #$E0
?ClearOamLow:
    stz     $2104
    sta     $2104
    stz     $2104
    stz     $2104
    inx
    cpx     #$0080
    bne     ?ClearOamLow

    ldx     #$0000
    lda     #%00000000

?ClearOamHigh:
    sta     $2104
    inx
    cpx     #$0020
    bne     ?ClearOamHigh

    plp
    pla
    rtl

CONST_ZERO: dw 0

; FillVRAM();
~~FillVRAM:
    pha                 ; Push accumulator
    phx                 ; Push x
    phy                 ; Push x
    php                 ; Push processor status

    .longa  off
    sep     #$20

    lda     #$80
    sta     $2115       ; Set VRAM port to WORD access

    .longi  on
    .longa  on
    rep     #$30

    lda     3+7+1, s
    sta     $2116       ; Set VRAM address

    ldx     #$1809
    stx     $4300       ; Set DMA mode to fixed source, WORD to $2118/9

    lda     3+7+2+1, s
    sta     $0000       ; Set $00:0000 to clear value

    ldx     #$0000
    stx     $4302       ; Set source address to $xx:0000

    lda     #$00
    sta     $4304       ; Set source bank to $00

    lda     3+7+2+2+1, s
    sta     $4305       ; Set transfer size in bytes

    lda     #$01        ; Initiate transfer
    sta     $420B

    plp                 ; Pull processor status
    ply                 ; Pull y
    plx                 ; Pull x
    pla                 ; Pull accumulator

    rtl

_NmiVector:
    jml ?faster
?faster:
    .longi on
    .longa on
    ; Rationale: an interrupt may be triggered while M or I is 8 bit but
    ; it is unknown whether the contents of the high byte in a register is
    ; important to the subroutine at a later point.
    rep     #$30        ; Safest way to preserve registers on irq
    pha
    phx
    phy
    phb

    .longa  off
    sep     #$20

    lda     >$4210      ; Clear NMI Flag

    lda     #$7E
    pha
    plb                 ; Make sure dbr is $7E

;   jsr     UpdateSprites   ; Update sprites
;   jsr     ReadJoysticks   ; Read joysticks
    .longa  on
    rep     #$20

    jsl     ~~NmiHandler

    rep     #$30        ; Need to restore entire word in registers
    plb
    ply
    plx
    pla
    rti                 ; psr is already saved on irq, hence no need to php/plp

_IrqVector:
    jml ?faster
?faster:
    .longi on
    .longa on
    rep     #$30
    pha
    phx
    phy
    phb

    .longa  off
    sep     #$20

    lda     >$4211      ; Clear IRQ flag

    lda     #$7E
    pha
    plb                 ; Make sure dbr is $7E

    .longa  on
    rep     #$20

    jsl     ~~IrqHandler

    rep     #$30
    plb
    ply
    plx
    pla

_native_cop:
_native_brk:
_native_abort:
_native_unused:
    stp
_emu_nmi:
_emu_cop:
_emu_unused:
_emu_abort:
_emu_irq:
    rti

    .include "header.inc" ;../include/

    .ends
    .end

