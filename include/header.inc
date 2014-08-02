
SNESHEADER .section offset $FFB0
; Extended header
	dw	$0000
	db	"ARM9"
	ds	7,0		; fill 7 bytes
	db	$00		; Ext RAM, used by emulators to detect GSU cart RAM
	;ENDS

	.org $FFC0
;SNESHEADER	SECTION OFFSET $FFC0
	db	"SNES C template      "	; ROM Title 21 bytes
;		"                     "
	.org $FFD5
	db	$21		; Memory mode (slow hirom)
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
	dw	_Boot
	dw	_emu_irq

	.ENDS
    
