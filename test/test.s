section .text
Fibonacci:
	push ebp
	mov ebp,esp
	mov ebx,[@s_esp]
	mov [@s_esp],esp
	mov esp,ebx
	mov ebx,[@s_ebp]
	push ebx
	mov [@s_ebp],esp
	mov ebx,[@s_esp]
	mov [@s_esp],esp
	mov esp,ebx
	push 1
	push 0
	mov eax,[ebp+8]
	mov ebx,[ebp-4]
	cmp eax,ebx
	je @lab_base_cmp_3
	mov eax,0
	jmp @lab_base_cmp_exit_4
@lab_base_cmp_3:
	mov eax,1
@lab_base_cmp_exit_4:
	mov [ebp-8],eax
	push 0
	push 0
	mov eax,[ebp+8]
	mov ebx,[ebp-12]
	cmp eax,ebx
	je @lab_base_cmp_7
	mov eax,0
	jmp @lab_base_cmp_exit_8
@lab_base_cmp_7:
	mov eax,1
@lab_base_cmp_exit_8:
	mov [ebp-16],eax
	push 0
	mov eax,[ebp-8]
	mov ebx,[ebp-16]
	or eax,ebx
	jne @lab_base_cmp_10
	mov eax,0
	jmp @lab_base_cmp_exit_11
@lab_base_cmp_10:
	mov eax,1
@lab_base_cmp_exit_11:
	mov [ebp-20],eax
	mov eax,[ebp-20]
	cmp eax,0
	je @if_1_middle
	push 1
	mov eax,[ebp-24]
	mov ebx,[@s_ebp]
	mov [@s_esp],ebx
	mov ebx,[@s_esp]
	mov [@s_esp],esp
	mov esp,ebx
	pop ebx
	mov [@s_ebp],ebx
	mov ebx,[@s_esp]
	mov [@s_esp],esp
	mov esp,ebx
	mov esp,ebp
	pop ebp
	ret
	mov esp,ebp
	jmp @if_1_end
@if_1_middle:
	mov esp,ebp
	push 1
	push 0
	mov eax,[ebp+8]
	mov ebx,[ebp-4]
	sub eax,ebx
	mov [ebp-8],eax
	mov eax,[ebp-8]
	push eax
	call Fibonacci
	add esp,4
	push 0
	mov [ebp-12],eax
	push 2
	push 0
	mov eax,[ebp+8]
	mov ebx,[ebp-16]
	sub eax,ebx
	mov [ebp-20],eax
	mov eax,[ebp-20]
	push eax
	call Fibonacci
	add esp,4
	push 0
	mov [ebp-24],eax
	push 0
	mov eax,[ebp-12]
	mov ebx,[ebp-24]
	add eax,ebx
	mov [ebp-28],eax
	mov eax,[ebp-28]
	mov ebx,[@s_ebp]
	mov [@s_esp],ebx
	mov ebx,[@s_esp]
	mov [@s_esp],esp
	mov esp,ebx
	pop ebx
	mov [@s_ebp],ebx
	mov ebx,[@s_esp]
	mov [@s_esp],esp
	mov esp,ebx
	mov esp,ebp
	pop ebp
	ret
	mov esp,ebp
@if_1_end:
	mov esp,ebp
	mov ebx,[@s_ebp]
	mov [@s_esp],ebx
	mov ebx,[@s_esp]
	mov [@s_esp],esp
	mov esp,ebx
	pop ebx
	mov [@s_ebp],ebx
	mov ebx,[@s_esp]
	mov [@s_esp],esp
	mov esp,ebx
	mov esp,ebp
	pop ebp
	ret
main:
	push ebp
	mov ebp,esp
	mov ebx,[@s_esp]
	mov [@s_esp],esp
	mov esp,ebx
	mov ebx,[@s_ebp]
	push ebx
	mov [@s_ebp],esp
	mov ebx,[@s_esp]
	mov [@s_esp],esp
	mov esp,ebx
@for_1_start:
	push 0
	push 1
	lea eax,[ebp-4]
	mov ebx,[ebp-8]
	mov [eax],ebx
@for_1_loop:
	push 20
	push 0
	mov eax,[ebp-4]
	mov ebx,[ebp-12]
	cmp eax,ebx
	jl @lab_base_cmp_23
	mov eax,0
	jmp @lab_base_cmp_exit_24
@lab_base_cmp_23:
	mov eax,1
@lab_base_cmp_exit_24:
	mov [ebp-16],eax
	mov eax,[ebp-16]
	cmp eax,0
	je @for_1_exit
	push 1
	push 0
	jmp @for_1_blcok
@for_1_iter:
	mov eax,[ebp-4]
	mov ebx,[ebp-20]
	add eax,ebx
	mov [ebp-24],eax
	lea eax,[ebp-4]
	mov ebx,[ebp-24]
	mov [eax],ebx
	lea esp,[ebp-8]
	jmp @for_1_loop
@for_1_blcok:
	push 1
	push 2
	mov eax,[ebp-4]
	push eax
	call Fibonacci
	add esp,4
	push 0
	mov [ebp-36],eax
	push 3
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,@str_3_len
	sub esp,1
	mov [esp],al
	mov [ebp-44],esp
	cmp eax,0
	je @lab_cpystr2_exit_33
	mov ecx,@str_3_len
	dec ecx
	mov esi,@str_3
@lab_cpystr2_32:
	cmp ecx,-1
	je @lab_cpystr2_exit_33
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_32
@lab_cpystr2_exit_33:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[ebp-36]
	mov esi,[ebp-44]
	mov edi,0
	cmp eax,0
	jge @lab_numsign1_exit_37
@lab_numsign1_36:
	neg eax
	mov edi,1
@lab_numsign1_exit_37:
	mov ebx,10
@lab_num2str1_34:
	mov edx,0
	idiv ebx
	mov cl,[esi]
	inc cl
	mov [esi],cl
	sub esp,1
	add dl,48
	mov [esp],dl
	cmp eax,0
	jne @lab_num2str1_34
	cmp edi,0
	je @lab_numsign_add_38
	sub esp,1
	mov ecx,45
	mov [esp],cl
	mov cl,[esi]
	inc cl
	mov [esi],cl
@lab_numsign_add_38:
	cmp cl,255
	jna @lab_num2str1_exit_35
	call @str2long
@lab_num2str1_exit_35:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov ebx,[ebp-44]
	mov eax,0
	mov al,[ebx]
	sub esp,1
	mov [esp],al
	mov [ebp-48],esp
	cmp eax,0
	je @lab_cpystr2_exit_41
	mov ecx,0
	mov esi,ebx
	dec esi
	neg eax
@lab_cpystr2_40:
	cmp ecx,eax
	je @lab_cpystr2_exit_41
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_40
@lab_cpystr2_exit_41:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,@str_2_len
	cmp eax,0
	je @lab_cpystr1_exit_43
	mov ebx,[ebp-48]
	mov edx,0
	mov dl,[ebx]
	add edx,eax
	mov [ebx],dl
	mov ecx,@str_2_len
	dec ecx
	mov esi,@str_2
	cmp edx,255
	jna @lab_cpystr1_42
	call @str2long
@lab_cpystr1_42:
	cmp ecx,-1
	je @lab_cpystr1_exit_43
	mov al,[esi+ecx]
	sub esp,1
	mov [esp],al
	dec ecx
	jmp @lab_cpystr1_42
@lab_cpystr1_exit_43:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov ebx,[ebp-48]
	mov eax,0
	mov al,[ebx]
	sub esp,1
	mov [esp],al
	mov [ebp-52],esp
	cmp eax,0
	je @lab_cpystr2_exit_46
	mov ecx,0
	mov esi,ebx
	dec esi
	neg eax
@lab_cpystr2_45:
	cmp ecx,eax
	je @lab_cpystr2_exit_46
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_45
@lab_cpystr2_exit_46:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[ebp-4]
	mov esi,[ebp-52]
	mov edi,0
	cmp eax,0
	jge @lab_numsign1_exit_50
@lab_numsign1_49:
	neg eax
	mov edi,1
@lab_numsign1_exit_50:
	mov ebx,10
@lab_num2str1_47:
	mov edx,0
	idiv ebx
	mov cl,[esi]
	inc cl
	mov [esi],cl
	sub esp,1
	add dl,48
	mov [esp],dl
	cmp eax,0
	jne @lab_num2str1_47
	cmp edi,0
	je @lab_numsign_add_51
	sub esp,1
	mov ecx,45
	mov [esp],cl
	mov cl,[esi]
	inc cl
	mov [esi],cl
@lab_numsign_add_51:
	cmp cl,255
	jna @lab_num2str1_exit_48
	call @str2long
@lab_num2str1_exit_48:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov ebx,[ebp-52]
	mov eax,0
	mov al,[ebx]
	sub esp,1
	mov [esp],al
	mov [ebp-56],esp
	cmp eax,0
	je @lab_cpystr2_exit_54
	mov ecx,0
	mov esi,ebx
	dec esi
	neg eax
@lab_cpystr2_53:
	cmp ecx,eax
	je @lab_cpystr2_exit_54
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_53
@lab_cpystr2_exit_54:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,@str_1_len
	cmp eax,0
	je @lab_cpystr1_exit_56
	mov ebx,[ebp-56]
	mov edx,0
	mov dl,[ebx]
	add edx,eax
	mov [ebx],dl
	mov ecx,@str_1_len
	dec ecx
	mov esi,@str_1
	cmp edx,255
	jna @lab_cpystr1_55
	call @str2long
@lab_cpystr1_55:
	cmp ecx,-1
	je @lab_cpystr1_exit_56
	mov al,[esi+ecx]
	sub esp,1
	mov [esp],al
	dec ecx
	jmp @lab_cpystr1_55
@lab_cpystr1_exit_56:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov ebx,[ebp-56]
	mov eax,0
	mov al,[ebx]
	sub esp,1
	mov [esp],al
	mov [ebp-60],esp
	cmp eax,0
	je @lab_cpystr2_exit_59
	mov ecx,0
	mov esi,ebx
	dec esi
	neg eax
@lab_cpystr2_58:
	cmp ecx,eax
	je @lab_cpystr2_exit_59
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_58
@lab_cpystr2_exit_59:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov ecx,[ebp-60]
	mov edx,0
	mov dl,[ecx]
	sub ecx,edx
	mov ebx,1
	mov eax,4
	int 128
	jmp @for_1_iter
@for_1_exit:
	mov esp,ebp
	push 0
	mov eax,[ebp-4]
	mov ebx,[@s_ebp]
	mov [@s_esp],ebx
	mov ebx,[@s_esp]
	mov [@s_esp],esp
	mov esp,ebx
	pop ebx
	mov [@s_ebp],ebx
	mov ebx,[@s_esp]
	mov [@s_esp],esp
	mov esp,ebx
	mov esp,ebp
	pop ebp
	ret
section .data
	@str_1 db "fib("
	@str_1_len equ 4
	@str_2 db "):"
	@str_2_len equ 2
	@str_3 db 10
	@str_3_len equ 1
section .bss
