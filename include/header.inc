; Vector table

SNESHEADER	.section offset $FFB0
;Extended header
	dw	$0000
	db	"ARM9"
	ds 7,0
	db	$00		;Ext RAM, only used by emulators to identify SuperFX RAM
	;ENDS
	.org $FFC0
;SNESHEADER	SECTION OFFSET $FFC0

	db	"overscan test"		; ROM Title
;		"                     "
	.org $FFD5
	db	$20		; Memory mode (slow & lo rom)
	db	$00		; Cart type (rom only)
	db	$09		; ROM size (4Mbit)
	db	$00		; SRAM size (0)

	db	$01		; Country code (USA)
	db	$00		; License code
	db	$00		; Version (v 1.00)
	
	dw	$0000	; Checksum and complement
	dw	$0000
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
    
