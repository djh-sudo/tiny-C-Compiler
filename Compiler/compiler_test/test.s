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
	push 0
	lea eax,[ebp-8]
	mov ebx,[ebp-12]
	mov [eax],ebx
@for_1_loop:
	push 1
	push 0
	jmp @for_1_blcok
@for_1_iter:
	mov eax,[ebp-8]
	mov ebx,[ebp-16]
	add eax,ebx
	mov [ebp-20],eax
	lea eax,[ebp-8]
	mov ebx,[ebp-20]
	mov [eax],ebx
	lea esp,[ebp-12]
	jmp @for_1_loop
@for_1_blcok:
	push 1
	push 0
	mov eax,[ebp-8]
	mov ebx,[ebp-24]
	add eax,ebx
	mov [ebp-28],eax
	lea eax,[ebp-4]
	mov ebx,[ebp-28]
	mov [eax],ebx
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
	push 1
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,@str_1_len
	sub esp,1
	mov [esp],al
	mov [ebp-44],esp
	cmp eax,0
	je @lab_cpystr2_exit_11
	mov ecx,@str_1_len
	dec ecx
	mov esi,@str_1
@lab_cpystr2_10:
	cmp ecx,-1
	je @lab_cpystr2_exit_11
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_10
@lab_cpystr2_exit_11:
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
	jge @lab_numsign1_exit_15
@lab_numsign1_14:
	neg eax
	mov edi,1
@lab_numsign1_exit_15:
	mov ebx,10
@lab_num2str1_12:
	mov edx,0
	idiv ebx
	mov cl,[esi]
	inc cl
	mov [esi],cl
	sub esp,1
	add dl,48
	mov [esp],dl
	cmp eax,0
	jne @lab_num2str1_12
	cmp edi,0
	je @lab_numsign_add_16
	sub esp,1
	mov ecx,45
	mov [esp],cl
	mov cl,[esi]
	inc cl
	mov [esi],cl
@lab_numsign_add_16:
	cmp cl,255
	jna @lab_num2str1_exit_13
	call @str2long
@lab_num2str1_exit_13:
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
	je @lab_cpystr2_exit_19
	mov ecx,0
	mov esi,ebx
	dec esi
	neg eax
@lab_cpystr2_18:
	cmp ecx,eax
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
	mov ecx,[ebp-48]
	mov edx,0
	mov dl,[ecx]
	sub ecx,edx
	mov ebx,1
	mov eax,4
	int 128
	push 5
	push 0
	mov eax,[ebp-8]
	mov ebx,[ebp-52]
	cmp eax,ebx
	jg @lab_base_cmp_24
	mov eax,0
	jmp @lab_base_cmp_exit_25
@lab_base_cmp_24:
	mov eax,1
@lab_base_cmp_exit_25:
	mov [ebp-56],eax
	mov eax,[ebp-56]
	cmp eax,0
	je @if_1_middle
	lea esp,[ebp-12]
	jmp @for_1_exit
	lea esp,[ebp-48]
	jmp @if_1_end
@if_1_middle:
	lea esp,[ebp-48]
	lea esp,[ebp-48]
@if_1_end:
	jmp @for_1_iter
@for_1_exit:
	push 0
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
section .data
	@str_1 db 10
	@str_1_len equ 1
section .bss
