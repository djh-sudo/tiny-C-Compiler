section .text
@str2long:
	mov edx,@str2long_data_len
	mov ecx,@str2long_data
	mov ebx,1
	mov eax,4
	int 128
	mov ebx,0
	mov eax,1
	int 128
	ret
@procBuf:
	mov esi,@buffer
	mov edi,0
	mov ecx,0
	mov eax,0
	mov ebx,10
@cal_buf_len:
	mov cl,[esi+edi]
	cmp ecx,10
	je @cal_buf_len_exit
	inc edi
	imul ebx
	add eax,ecx
	sub eax,48
	jmp @cal_buf_len
@cal_buf_len_exit:
	mov ecx,edi
	mov [@buffer_len],cl
	mov bl,[esi]
	ret
global _start
_start:
	call main
	mov ebx,0
	mov eax,1
	int 128
	ret
section .data
	@str2long_data db  "Str-Overflow",10,13
	@str2long_data_len equ 14
	@buffer times 255 db 0
	@buffer_len db 0
	@s_esp dd @s_base
	@s_ebp dd 0
section .bss
	@s_stack times 65536 db 0
@s_base:
