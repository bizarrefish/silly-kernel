.intel_syntax noprefix

.global getKeyboardChar
.global getKeyboardScanCode
.global uTable
.global lTable

.data
uTable: .incbin "scancodes/ucase"
lTable: .incbin "scancodes/lcase"


.text
getKeyboardScanCode:
	in al,0x60
	and eax,0xFF
	ret
