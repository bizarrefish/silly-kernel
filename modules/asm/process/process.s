.intel_syntax noprefix

.data

kernel_stack:
.long 0
process_stack_area:
.long 0

.text
.global callProcess
.global _process_init
.global retFromProcess



#process' stack area(starting at high memory):
#[PROGRAM COUNTER]
#[ALL REGISTERS]
#[UNUSED MEMORY]
#[STACK POINTER]


_process_init:
	ret


callProcess:
	enter 0,0

	#pop address of stack area
	mov eax,[ebp+8]

	mov [process_stack_area],eax	#save address of process' stack area

	#save kernel registers:
	pushad

	#save kernel's stack pointer:
	mov [kernel_stack],esp

	mov esp,[eax]	#load saved stack pointer from bottom of stack area
	
	#load process' registers from process stack:
	popad

#context switch complete

	#JUMP TO PROCESS' CODE
	ret



#code to switch out of current process, and back into kernel space
retFromProcess:

	#save process' registers:
	pushad
	
	mov eax,[process_stack_area]	#get location of process' stack area

	#save process' stack pointer in bottom of process' stack area
	mov [eax],esp



	#get kernel stack pointer from data segment
	mov eax,[kernel_stack]
	mov esp,eax

	#pop kernel registers
	popad

	#context switch complete

	#return to kernel code(the end of the callProcess call)
	leave
	ret
