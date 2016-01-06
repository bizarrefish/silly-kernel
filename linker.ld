ENTRY(entry)

SECTIONS {
	. = 0x100000;

	.text : {
		*(.text);
	}

	.data : {
		*(.data);
	}

	.rodata : {
		*(.rodata);
	}

	bss_base = .;
	.bss : {
		*(.bss);
	}
	bss_end = .;


	. += 0x100000;
	stackTop = .;
	_heapStart = .;
	. += 0x1000000;
	_heapEnd = .;
}
