
; Size predefs for ROM/RAM/SRAM fields
SIZE_NONE	.equ $00
SIZE_2KiB	.equ $01 ; amount of cart RAM in Super Mario World
SIZE_4KiB	.equ $02
SIZE_8KiB	.equ $03
SIZE_16KiB	.equ $04
SIZE_32KiB	.equ $05 ; Amount of cart RAM in Mario Paint
SIZE_64KiB	.equ $06 ; Amount of cart RAM in Stunt Race FX
SIZE_128KiB	.equ $07 ; Amount of cart RAM in Dezaemon - Kaite Tsukutte Asoberu
SIZE_256KiB	.equ $08
SIZE_512KiB	.equ $09 ; Amount of ROM in Super Mario World
SIZE_1MiB	.equ $0A ; Amount of ROM in Mario Paint
SIZE_2MiB	.equ $0B ; Amount of ROM in Super Mario World 2
SIZE_4MiB	.equ $0C ; Amount of ROM in Donkey Kong Country 2 and 3
; Valid range for ROM: $08-$0C, most emulators don't mind $05
; Valid range for cart RAM: $00-$07

; Map modes
MODE_20		.equ $20 ; LoROM
MODE_21		.equ $21 ; HiROM
MODE_23		.equ $23 ; SA-1 ROM
MODE_25		.equ $25 ; ExHiROM + SlowROM
MODE_30		.equ $30 ; LoROM + FastROM
MODE_31		.equ $31 ; HiROM + FastROM
MODE_35		.equ $35 ; ExHiROM + FastROM

; Cartridge types
CART_ROM			.equ $00 ; ROM only
CART_ROM_RAM		.equ $01 ; ROM+RAM
CART_ROM_RAM_BAT	.equ $02 ; ROM+RAM+Battery
; Coprocessors (add with one of the above)
CART_DSP1			.equ $03 ; DSP-1
CART_GSU			.equ $13 ; SuperFX
CART_OBC1			.equ $23 ; OBC-1
CART_SA1			.equ $33 ; SA-1
CART_OTHER			.equ $E3	; \ Todo: determine out how/if emulators use these
CART_CUSTOM			.equ $F3	; | to distinguish between obscure stuff like the
								; / CX4, ST-0018, SPC7110 etc.

; Destination codes, most emulators use this to determine PAL/NTSC
DEST_JAPAN			.equ $00
DEST_USA_CANADA		.equ $01
DEST_EUROPE			.equ $02
DEST_SCANDANAVIA	.equ $03
DEST_FRENCH_EUROPE	.equ $06
DEST_DUTCH			.equ $07
DEST_SPANISH		.equ $08
DEST_GERMAN			.equ $09
DEST_ITALIAN		.equ $0A
DEST_CHINESE		.equ $0B
DEST_KOREAN			.equ $0D
DEST_COMMON			.equ $0E
DEST_CANADA			.equ $0F
DEST_BRAZIL			.equ $10
DEST_AUSTRALIA		.equ $11

SNESHEADER .section offset $FFB0
; Extended header
	dw	$0000		; B0 - Maker code
	db	"ARM9"		; B2 - Game code
	ds	7,0			; B6 - fill 7 bytes
	db	$00			; BD - Ext RAM, used by emulators to detect GSU cart RAM

	.org $FFC0
;SNESHEADER	SECTION OFFSET $FFC0
	db	"SNES C template      "	; C0 - ROM Title 21 bytes
;		"                     "
	.org $FFD5
	db	MODE_31				; D5 - Map mode
	db	CART_ROM			; D6 - Cart type
	db	SIZE_256KiB			; D7 - ROM size
	db	SIZE_NONE			; D8 - SRAM size
	db	DEST_USA_CANADA		; D9 - Country code
	db	$33					; DA - License code, some emulators care about this in edge cases, 0 or $33
	db	$00					; DB - Version (v 1.00)
	dw	$0000				; DC - Checksum
	dw	$0000				; and complement
	.ENDS

VECTORS	.section offset $FFE4
	dw	_native_cop
	dw	_native_brk
	dw	_native_abort
	dw	_NmiVector
	dw	_native_unused
	dw	_IrqVector
	ds	4

	dw	_emu_cop
	dw	_emu_unused
	dw	_emu_abort
	dw	_emu_nmi
	dw	_Reset
	dw	_emu_irq

	.ENDS
    .END

