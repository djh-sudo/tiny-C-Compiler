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
@for_1_start:
	push 0
	push 0
	lea eax,[ebp-4]
	mov ebx,[ebp-8]
	mov [eax],ebx
@for_1_loop:
	push 10
	push 0
	mov eax,[ebp-4]
	mov ebx,[ebp-12]
	cmp eax,ebx
	jl @lab_base_cmp_4
	mov eax,0
	jmp @lab_base_cmp_exit_5
@lab_base_cmp_4:
	mov eax,1
@lab_base_cmp_exit_5:
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
@for_2_start:
	push 0
	push 0
	lea eax,[ebp-28]
	mov ebx,[ebp-32]
	mov [eax],ebx
@for_2_loop:
	push 10
	push 0
	mov eax,[ebp-28]
	mov ebx,[ebp-36]
	cmp eax,ebx
	jl @lab_base_cmp_11
	mov eax,0
	jmp @lab_base_cmp_exit_12
@lab_base_cmp_11:
	mov eax,1
@lab_base_cmp_exit_12:
	mov [ebp-40],eax
	mov eax,[ebp-40]
	cmp eax,0
	je @for_2_exit
	push 1
	push 0
	jmp @for_2_blcok
@for_2_iter:
	mov eax,[ebp-28]
	mov ebx,[ebp-44]
	add eax,ebx
	mov [ebp-48],eax
	lea eax,[ebp-28]
	mov ebx,[ebp-48]
	mov [eax],ebx
	lea esp,[ebp-32]
	jmp @for_2_loop
@for_2_blcok:
	push 0
	mov eax,[ebp-4]
	mov ebx,[ebp-28]
	imul ebx
	mov [ebp-52],eax
	push 1
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,@str_1_len
	sub esp,1
	mov [esp],al
	mov [ebp-60],esp
	cmp eax,0
	je @lab_cpystr2_exit_19
	mov ecx,@str_1_len
	dec ecx
	mov esi,@str_1
@lab_cpystr2_18:
	cmp ecx,-1
	je @lab_cpystr2_exit_19
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_18
@lab_cpystr2_exit_19:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[ebp-52]
	mov esi,[ebp-60]
	mov edi,0
	cmp eax,0
	jge @lab_numsign1_exit_23
@lab_numsign1_22:
	neg eax
	mov edi,1
@lab_numsign1_exit_23:
	mov ebx,10
@lab_num2str1_20:
	mov edx,0
	idiv ebx
	mov cl,[esi]
	inc cl
	mov [esi],cl
	sub esp,1
	add dl,48
	mov [esp],dl
	cmp eax,0
	jne @lab_num2str1_20
	cmp edi,0
	je @lab_numsign_add_24
	sub esp,1
	mov ecx,45
	mov [esp],cl
	mov cl,[esi]
	inc cl
	mov [esi],cl
@lab_numsign_add_24:
	cmp cl,255
	jna @lab_num2str1_exit_21
	call @str2long
@lab_num2str1_exit_21:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov ebx,[ebp-60]
	mov eax,0
	mov al,[ebx]
	sub esp,1
	mov [esp],al
	mov [ebp-64],esp
	cmp eax,0
	je @lab_cpystr2_exit_27
	mov ecx,0
	mov esi,ebx
	dec esi
	neg eax
@lab_cpystr2_26:
	cmp ecx,eax
	je @lab_cpystr2_exit_27
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_26
@lab_cpystr2_exit_27:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov ecx,[ebp-64]
	mov edx,0
	mov dl,[ecx]
	sub ecx,edx
	mov ebx,1
	mov eax,4
	int 128
	jmp @for_2_iter
@for_2_exit:
	lea esp,[ebp-24]
	push 2
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,@str_2_len
	sub esp,1
	mov [esp],al
	mov [ebp-32],esp
	cmp eax,0
	je @lab_cpystr2_exit_33
	mov ecx,@str_2_len
	dec ecx
	mov esi,@str_2
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
	mov ecx,[ebp-32]
	mov edx,0
	mov dl,[ecx]
	sub ecx,edx
	mov ebx,1
	mov eax,4
	int 128
	push 5
	push 0
	mov eax,[ebp-4]
	mov ebx,[ebp-36]
	cmp eax,ebx
	jg @lab_base_cmp_38
	mov eax,0
	jmp @lab_base_cmp_exit_39
@lab_base_cmp_38:
	mov eax,1
@lab_base_cmp_exit_39:
	mov [ebp-40],eax
	mov eax,[ebp-40]
	cmp eax,0
	je @if_1_middle
	lea esp,[ebp-32]
	jmp @for_1_exit
	lea esp,[ebp-32]
	jmp @if_1_end
@if_1_middle:
	lea esp,[ebp-32]
@if_1_end:
	lea esp,[ebp-32]
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
	@str_1 db " "
	@str_1_len equ 1
	@str_2 db 10
	@str_2_len equ 1
section .bss
