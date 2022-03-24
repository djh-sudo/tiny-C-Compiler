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
	push 0
	push 0
	mov eax,[ebp-4]
	mov ebx,[ebp-4]
	imul ebx
	mov [ebp-32],eax
	push 0
	mov eax,[ebp-4]
	mov ebx,[ebp-32]
	imul ebx
	mov [ebp-36],eax
	lea eax,[ebp-28]
	mov ebx,[ebp-36]
	mov [eax],ebx
	push 8
	push 0
	mov eax,[ebp-4]
	mov ebx,[ebp-40]
	cmp eax,ebx
	jle @lab_base_cmp_12
	mov eax,0
	jmp @lab_base_cmp_exit_13
@lab_base_cmp_12:
	mov eax,1
@lab_base_cmp_exit_13:
	mov [ebp-44],eax
	mov eax,[ebp-44]
	cmp eax,0
	je @if_1_middle
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
	je @lab_cpystr2_exit_17
	mov ecx,@str_1_len
	dec ecx
	mov esi,@str_1
@lab_cpystr2_16:
	cmp ecx,-1
	je @lab_cpystr2_exit_17
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_16
@lab_cpystr2_exit_17:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[ebp-28]
	mov esi,[ebp-52]
	mov edi,0
	cmp eax,0
	jge @lab_numsign1_exit_21
@lab_numsign1_20:
	neg eax
	mov edi,1
@lab_numsign1_exit_21:
	mov ebx,10
@lab_num2str1_18:
	mov edx,0
	idiv ebx
	mov cl,[esi]
	inc cl
	mov [esi],cl
	sub esp,1
	add dl,48
	mov [esp],dl
	cmp eax,0
	jne @lab_num2str1_18
	cmp edi,0
	je @lab_numsign_add_22
	sub esp,1
	mov ecx,45
	mov [esp],cl
	mov cl,[esi]
	inc cl
	mov [esi],cl
@lab_numsign_add_22:
	cmp cl,255
	jna @lab_num2str1_exit_19
	call @str2long
@lab_num2str1_exit_19:
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
	je @lab_cpystr2_exit_25
	mov ecx,0
	mov esi,ebx
	dec esi
	neg eax
@lab_cpystr2_24:
	cmp ecx,eax
	je @lab_cpystr2_exit_25
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_24
@lab_cpystr2_exit_25:
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
	lea esp,[ebp-36]
	jmp @if_1_end
@if_1_middle:
	lea esp,[ebp-36]
	push 0
	mov eax,[ebp-28]
	mov ebx,[ebp-4]
	imul ebx
	mov [ebp-40],eax
	push 2
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,@str_2_len
	sub esp,1
	mov [esp],al
	mov [ebp-48],esp
	cmp eax,0
	je @lab_cpystr2_exit_32
	mov ecx,@str_2_len
	dec ecx
	mov esi,@str_2
@lab_cpystr2_31:
	cmp ecx,-1
	je @lab_cpystr2_exit_32
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_31
@lab_cpystr2_exit_32:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[ebp-40]
	mov esi,[ebp-48]
	mov edi,0
	cmp eax,0
	jge @lab_numsign1_exit_36
@lab_numsign1_35:
	neg eax
	mov edi,1
@lab_numsign1_exit_36:
	mov ebx,10
@lab_num2str1_33:
	mov edx,0
	idiv ebx
	mov cl,[esi]
	inc cl
	mov [esi],cl
	sub esp,1
	add dl,48
	mov [esp],dl
	cmp eax,0
	jne @lab_num2str1_33
	cmp edi,0
	je @lab_numsign_add_37
	sub esp,1
	mov ecx,45
	mov [esp],cl
	mov cl,[esi]
	inc cl
	mov [esi],cl
@lab_numsign_add_37:
	cmp cl,255
	jna @lab_num2str1_exit_34
	call @str2long
@lab_num2str1_exit_34:
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
	je @lab_cpystr2_exit_40
	mov ecx,0
	mov esi,ebx
	dec esi
	neg eax
@lab_cpystr2_39:
	cmp ecx,eax
	je @lab_cpystr2_exit_40
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_39
@lab_cpystr2_exit_40:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov ecx,[ebp-52]
	mov edx,0
	mov dl,[ecx]
	sub ecx,edx
	mov ebx,1
	mov eax,4
	int 128
	lea esp,[ebp-8]
	jmp @for_1_exit
	lea esp,[ebp-36]
@if_1_end:
	jmp @for_1_iter
@for_1_exit:
	push 3
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,@str_3_len
	sub esp,1
	mov [esp],al
	mov [ebp-24],esp
	cmp eax,0
	je @lab_cpystr2_exit_46
	mov ecx,@str_3_len
	dec ecx
	mov esi,@str_3
@lab_cpystr2_45:
	cmp ecx,-1
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
	mov esi,[ebp-24]
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
	mov ebx,[ebp-24]
	mov eax,0
	mov al,[ebx]
	sub esp,1
	mov [esp],al
	mov [ebp-28],esp
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
	mov ecx,[ebp-28]
	mov edx,0
	mov dl,[ecx]
	sub ecx,edx
	mov ebx,1
	mov eax,4
	int 128
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
	@str_2 db 10
	@str_2_len equ 1
	@str_3 db 10
	@str_3_len equ 1
section .bss
