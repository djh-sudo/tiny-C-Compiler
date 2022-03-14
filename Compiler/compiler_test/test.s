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
@while_1_loop:
	push 10
	push 0
	mov eax,[ebp-4]
	mov ebx,[ebp-20]
	cmp eax,ebx
	jl @lab_base_cmp_5
	mov eax,0
	jmp @lab_base_cmp_exit_6
@lab_base_cmp_5:
	mov eax,1
@lab_base_cmp_exit_6:
	mov [ebp-24],eax
	mov eax,[ebp-24]
	cmp eax,0
	je @while_1_exit
	push 1
	push 0
	mov eax,[ebp-4]
	mov ebx,[ebp-28]
	add eax,ebx
	mov [ebp-32],eax
	lea eax,[ebp-4]
	mov ebx,[ebp-32]
	mov [eax],ebx
	push 0
	lea eax,[ebp-12]
	mov ebx,[ebp-36]
	mov [eax],ebx
@while_2_loop:
	push 10
	push 0
	mov eax,[ebp-12]
	mov ebx,[ebp-40]
	cmp eax,ebx
	jl @lab_base_cmp_12
	mov eax,0
	jmp @lab_base_cmp_exit_13
@lab_base_cmp_12:
	mov eax,1
@lab_base_cmp_exit_13:
	mov [ebp-44],eax
	mov eax,[ebp-44]
	cmp eax,0
	je @while_2_exit
	push 1
	push 0
	mov eax,[ebp-12]
	mov ebx,[ebp-48]
	add eax,ebx
	mov [ebp-52],eax
	lea eax,[ebp-12]
	mov ebx,[ebp-52]
	mov [eax],ebx
	push 0
	mov eax,[ebp-4]
	mov ebx,[ebp-12]
	imul ebx
	mov [ebp-56],eax
	push 1
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,@str_1_len
	sub esp,1
	mov [esp],al
	mov [ebp-64],esp
	cmp eax,0
	je @lab_cpystr2_exit_20
	mov ecx,@str_1_len
	dec ecx
	mov esi,@str_1
@lab_cpystr2_19:
	cmp ecx,-1
	je @lab_cpystr2_exit_20
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_19
@lab_cpystr2_exit_20:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[ebp-56]
	mov esi,[ebp-64]
	mov edi,0
	cmp eax,0
	jge @lab_numsign1_exit_24
@lab_numsign1_23:
	neg eax
	mov edi,1
@lab_numsign1_exit_24:
	mov ebx,10
@lab_num2str1_21:
	mov edx,0
	idiv ebx
	mov cl,[esi]
	inc cl
	mov [esi],cl
	sub esp,1
	add dl,48
	mov [esp],dl
	cmp eax,0
	jne @lab_num2str1_21
	cmp edi,0
	je @lab_numsign_add_25
	sub esp,1
	mov ecx,45
	mov [esp],cl
	mov cl,[esi]
	inc cl
	mov [esi],cl
@lab_numsign_add_25:
	cmp cl,255
	jna @lab_num2str1_exit_22
	call @str2long
@lab_num2str1_exit_22:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov ebx,[ebp-64]
	mov eax,0
	mov al,[ebx]
	sub esp,1
	mov [esp],al
	mov [ebp-68],esp
	cmp eax,0
	je @lab_cpystr2_exit_28
	mov ecx,0
	mov esi,ebx
	dec esi
	neg eax
@lab_cpystr2_27:
	cmp ecx,eax
	je @lab_cpystr2_exit_28
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_27
@lab_cpystr2_exit_28:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov ecx,[ebp-68]
	mov edx,0
	mov dl,[ecx]
	sub ecx,edx
	mov ebx,1
	mov eax,4
	int 128
	lea esp,[ebp-36]
	jmp @while_2_loop
@while_2_exit:
	push 2
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,@str_2_len
	sub esp,1
	mov [esp],al
	mov [ebp-44],esp
	cmp eax,0
	je @lab_cpystr2_exit_34
	mov ecx,@str_2_len
	dec ecx
	mov esi,@str_2
@lab_cpystr2_33:
	cmp ecx,-1
	je @lab_cpystr2_exit_34
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_33
@lab_cpystr2_exit_34:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov ecx,[ebp-44]
	mov edx,0
	mov dl,[ecx]
	sub ecx,edx
	mov ebx,1
	mov eax,4
	int 128
	lea esp,[ebp-16]
	jmp @while_1_loop
@while_1_exit:
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
	@str_1 db "-"
	@str_1_len equ 1
	@str_2 db 10
	@str_2_len equ 1
section .bss
