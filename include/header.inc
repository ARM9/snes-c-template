
SNESHEADER	.section offset $FFB0
; Extended header
	dw	$0000
	db	"ARM9"
	ds	7,0		; fill 7 bytes
	db	$00		; Ext RAM, only used by emulators to identify SuperFX RAM
	;ENDS

	.org $FFC0
;SNESHEADER	SECTION OFFSET $FFC0
	db	"SNES C template"	; ROM Title
;		"                     "
	.org $FFD5
	db	$20		; Memory mode (slow & lo rom)
	db	$00		; Cart type (ROM only)
	db	$08		; ROM size (2Mbit)
	db	$00		; SRAM size (0Kbit)

	db	$01		; Country code (USA)
	db	$33		; License code, some emulators care about this in edge cases, 0 or $33
	db	$00		; Version (v 1.00)
	
	dw	$0000	; Checksum
	dw	$0000	; and complement
	.ENDS

VECTORS	.section offset $FFE4
	dw	native_cop
	dw	native_brk
	dw	native_abort
	dw	NMIVector
	dw	native_unused
	dw	IRQVector
	ds	4

	dw	emu_cop
	dw	emu_unused
	dw	emu_abort
	dw	emu_nmi
	dw	Start
	dw	emu_irq

	.ENDS
    
