//OS Head file

.intel_syntax noprefix


.text
.align 4
//multiboot header:
multiboot:
.long 0x1BADB002, 0x00000003, 0xE4524FFB

.globl entry

entry:
	mov esp,offset flat:stackTop
	mov ebp,esp

	mov eax,0x0
	mov ecx,offset flat:bss_base

zerobss:
	mov [ecx],eax
	inc ecx
	cmp ecx,offset flat:bss_end
	je done
	jmp zerobss

done:
	call kMain
