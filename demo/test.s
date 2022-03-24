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
	mov eax,[ebp-8]
	cmp eax,0
	je @if_1_middle
	push 1
	mov eax,[ebp-12]
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
	push 0
	push 0
	mov eax,[ebp+8]
	mov ebx,[ebp-4]
	cmp eax,ebx
	je @lab_base_cmp_8
	mov eax,0
	jmp @lab_base_cmp_exit_9
@lab_base_cmp_8:
	mov eax,1
@lab_base_cmp_exit_9:
	mov [ebp-8],eax
	mov eax,[ebp-8]
	cmp eax,0
	je @if_2_middle
	push 0
	mov eax,[ebp-12]
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
	jmp @if_2_end
@if_2_middle:
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
@if_2_end:
	mov esp,ebp
@if_1_end:
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
	jl @lab_base_cmp_21
	mov eax,0
	jmp @lab_base_cmp_exit_22
@lab_base_cmp_21:
	mov eax,1
@lab_base_cmp_exit_22:
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
	je @lab_cpystr2_exit_31
	mov ecx,@str_3_len
	dec ecx
	mov esi,@str_3
@lab_cpystr2_30:
	cmp ecx,-1
	je @lab_cpystr2_exit_31
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_30
@lab_cpystr2_exit_31:
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
	jge @lab_numsign1_exit_35
@lab_numsign1_34:
	neg eax
	mov edi,1
@lab_numsign1_exit_35:
	mov ebx,10
@lab_num2str1_32:
	mov edx,0
	idiv ebx
	mov cl,[esi]
	inc cl
	mov [esi],cl
	sub esp,1
	add dl,48
	mov [esp],dl
	cmp eax,0
	jne @lab_num2str1_32
	cmp edi,0
	je @lab_numsign_add_36
	sub esp,1
	mov ecx,45
	mov [esp],cl
	mov cl,[esi]
	inc cl
	mov [esi],cl
@lab_numsign_add_36:
	cmp cl,255
	jna @lab_num2str1_exit_33
	call @str2long
@lab_num2str1_exit_33:
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
	je @lab_cpystr2_exit_39
	mov ecx,0
	mov esi,ebx
	dec esi
	neg eax
@lab_cpystr2_38:
	cmp ecx,eax
	je @lab_cpystr2_exit_39
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_38
@lab_cpystr2_exit_39:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,@str_2_len
	cmp eax,0
	je @lab_cpystr1_exit_41
	mov ebx,[ebp-48]
	mov edx,0
	mov dl,[ebx]
	add edx,eax
	mov [ebx],dl
	mov ecx,@str_2_len
	dec ecx
	mov esi,@str_2
	cmp edx,255
	jna @lab_cpystr1_40
	call @str2long
@lab_cpystr1_40:
	cmp ecx,-1
	je @lab_cpystr1_exit_41
	mov al,[esi+ecx]
	sub esp,1
	mov [esp],al
	dec ecx
	jmp @lab_cpystr1_40
@lab_cpystr1_exit_41:
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
	je @lab_cpystr2_exit_44
	mov ecx,0
	mov esi,ebx
	dec esi
	neg eax
@lab_cpystr2_43:
	cmp ecx,eax
	je @lab_cpystr2_exit_44
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_43
@lab_cpystr2_exit_44:
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
	jge @lab_numsign1_exit_48
@lab_numsign1_47:
	neg eax
	mov edi,1
@lab_numsign1_exit_48:
	mov ebx,10
@lab_num2str1_45:
	mov edx,0
	idiv ebx
	mov cl,[esi]
	inc cl
	mov [esi],cl
	sub esp,1
	add dl,48
	mov [esp],dl
	cmp eax,0
	jne @lab_num2str1_45
	cmp edi,0
	je @lab_numsign_add_49
	sub esp,1
	mov ecx,45
	mov [esp],cl
	mov cl,[esi]
	inc cl
	mov [esi],cl
@lab_numsign_add_49:
	cmp cl,255
	jna @lab_num2str1_exit_46
	call @str2long
@lab_num2str1_exit_46:
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
	je @lab_cpystr2_exit_52
	mov ecx,0
	mov esi,ebx
	dec esi
	neg eax
@lab_cpystr2_51:
	cmp ecx,eax
	je @lab_cpystr2_exit_52
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_51
@lab_cpystr2_exit_52:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,@str_1_len
	cmp eax,0
	je @lab_cpystr1_exit_54
	mov ebx,[ebp-56]
	mov edx,0
	mov dl,[ebx]
	add edx,eax
	mov [ebx],dl
	mov ecx,@str_1_len
	dec ecx
	mov esi,@str_1
	cmp edx,255
	jna @lab_cpystr1_53
	call @str2long
@lab_cpystr1_53:
	cmp ecx,-1
	je @lab_cpystr1_exit_54
	mov al,[esi+ecx]
	sub esp,1
	mov [esp],al
	dec ecx
	jmp @lab_cpystr1_53
@lab_cpystr1_exit_54:
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
	je @lab_cpystr2_exit_57
	mov ecx,0
	mov esi,ebx
	dec esi
	neg eax
@lab_cpystr2_56:
	cmp ecx,eax
	je @lab_cpystr2_exit_57
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_56
@lab_cpystr2_exit_57:
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
	push 0
	mov eax,[ebp-20]
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
