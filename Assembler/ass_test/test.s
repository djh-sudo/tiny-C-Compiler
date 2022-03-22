section .text
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
	push 0
	lea eax,[ebp-4]
	mov ebx,[ebp-8]
	mov [eax],ebx
	push 0
	push 0
	lea eax,[ebp-12]
	mov ebx,[ebp-16]
	mov [eax],ebx
	push 0
	lea eax,[ebp-12]
	mov ebx,[ebp-20]
	mov [eax],ebx
@for_1_loop:
	push 10
	push 0
	mov eax,[ebp-12]
	mov ebx,[ebp-24]
	cmp eax,ebx
	jl @lab_base_cmp_6
	mov eax,0
	jmp @lab_base_cmp_exit_7
@lab_base_cmp_6:
	mov eax,1
@lab_base_cmp_exit_7:
	mov [ebp-28],eax
	mov eax,[ebp-28]
	cmp eax,0
	je @for_1_exit
	push 1
	push 0
	jmp @for_1_blcok
@for_1_iter:
	mov eax,[ebp-12]
	mov ebx,[ebp-32]
	add eax,ebx
	mov [ebp-36],eax
	lea eax,[ebp-12]
	mov ebx,[ebp-36]
	mov [eax],ebx
	lea esp,[ebp-20]
	jmp @for_1_loop
@for_1_blcok:
	push 0
	mov eax,[ebp-12]
	mov ebx,[ebp-12]
	imul ebx
	mov [ebp-40],eax
	push 0
	mov eax,[ebp-12]
	mov ebx,[ebp-40]
	imul ebx
	mov [ebp-44],eax
	lea eax,[ebp-4]
	mov ebx,[ebp-44]
	mov [eax],ebx
	push 1
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,@str_1_len
	sub esp,1
	mov [esp],al
	mov [ebp-52],esp
	cmp eax,0
	je @lab_cpystr2_exit_15
	mov ecx,@str_1_len
	dec ecx
	mov esi,@str_1
@lab_cpystr2_14:
	cmp ecx,-1
	je @lab_cpystr2_exit_15
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_14
@lab_cpystr2_exit_15:
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
	jge @lab_numsign1_exit_19
@lab_numsign1_18:
	neg eax
	mov edi,1
@lab_numsign1_exit_19:
	mov ebx,10
@lab_num2str1_16:
	mov edx,0
	idiv ebx
	mov cl,[esi]
	inc cl
	mov [esi],cl
	sub esp,1
	add dl,48
	mov [esp],dl
	cmp eax,0
	jne @lab_num2str1_16
	cmp edi,0
	je @lab_numsign_add_20
	sub esp,1
	mov ecx,45
	mov [esp],cl
	mov cl,[esi]
	inc cl
	mov [esi],cl
@lab_numsign_add_20:
	cmp cl,255
	jna @lab_num2str1_exit_17
	call @str2long
@lab_num2str1_exit_17:
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
	je @lab_cpystr2_exit_23
	mov ecx,0
	mov esi,ebx
	dec esi
	neg eax
@lab_cpystr2_22:
	cmp ecx,eax
	je @lab_cpystr2_exit_23
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_22
@lab_cpystr2_exit_23:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov ecx,[ebp-56]
	mov edx,0
	mov dl,[ecx]
	sub ecx,edx
	mov ebx,1
	mov eax,4
	int 128
	jmp @for_1_iter
@for_1_exit:
	push 0
	mov eax,[ebp-32]
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
	@str_1 db 10
	@str_1_len equ 1
section .bss
