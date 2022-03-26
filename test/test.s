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
	mov ecx,@buffer
	mov edx,255
	mov ebx,0
	mov eax,3
	int 128
	call @procBuf
	mov [ebp-4],eax
	jmp @switch_1_table
@case_1_100_lab:
@for_1_start:
	push 0
	push 0
	lea eax,[ebp-8]
	mov ebx,[ebp-12]
	mov [eax],ebx
@for_1_loop:
	push 20
	push 0
	mov eax,[ebp-8]
	mov ebx,[ebp-16]
	cmp eax,ebx
	jl @lab_base_cmp_4
	mov eax,0
	jmp @lab_base_cmp_exit_5
@lab_base_cmp_4:
	mov eax,1
@lab_base_cmp_exit_5:
	mov [ebp-20],eax
	mov eax,[ebp-20]
	cmp eax,0
	je @for_1_exit
	push 1
	push 0
	jmp @for_1_blcok
@for_1_iter:
	mov eax,[ebp-8]
	mov ebx,[ebp-24]
	add eax,ebx
	mov [ebp-28],eax
	lea eax,[ebp-8]
	mov ebx,[ebp-28]
	mov [eax],ebx
	lea esp,[ebp-12]
	jmp @for_1_loop
@for_1_blcok:
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
	je @lab_cpystr2_exit_13
	mov ecx,@str_1_len
	dec ecx
	mov esi,@str_1
@lab_cpystr2_12:
	cmp ecx,-1
	je @lab_cpystr2_exit_13
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_12
@lab_cpystr2_exit_13:
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
	jge @lab_numsign1_exit_17
@lab_numsign1_16:
	neg eax
	mov edi,1
@lab_numsign1_exit_17:
	mov ebx,10
@lab_num2str1_14:
	mov edx,0
	idiv ebx
	mov cl,[esi]
	inc cl
	mov [esi],cl
	sub esp,1
	add dl,48
	mov [esp],dl
	cmp eax,0
	jne @lab_num2str1_14
	cmp edi,0
	je @lab_numsign_add_18
	sub esp,1
	mov ecx,45
	mov [esp],cl
	mov cl,[esi]
	inc cl
	mov [esi],cl
@lab_numsign_add_18:
	cmp cl,255
	jna @lab_num2str1_exit_15
	call @str2long
@lab_num2str1_exit_15:
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
	je @lab_cpystr2_exit_21
	mov ecx,0
	mov esi,ebx
	dec esi
	neg eax
@lab_cpystr2_20:
	cmp ecx,eax
	je @lab_cpystr2_exit_21
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_20
@lab_cpystr2_exit_21:
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
	lea esp,[ebp-12]
	jmp @for_1_exit
	jmp @for_1_iter
@for_1_exit:
	lea esp,[ebp-4]
	lea esp,[ebp-4]
	jmp @switch_tab_1_end
	lea esp,[ebp-4]
@case_1_1_lab:
@for_2_start:
	push 0
	push 0
	lea eax,[ebp-8]
	mov ebx,[ebp-12]
	mov [eax],ebx
@for_2_loop:
	push 20
	push 0
	mov eax,[ebp-8]
	mov ebx,[ebp-16]
	cmp eax,ebx
	jl @lab_base_cmp_27
	mov eax,0
	jmp @lab_base_cmp_exit_28
@lab_base_cmp_27:
	mov eax,1
@lab_base_cmp_exit_28:
	mov [ebp-20],eax
	mov eax,[ebp-20]
	cmp eax,0
	je @for_2_exit
	push 1
	push 0
	jmp @for_2_blcok
@for_2_iter:
	mov eax,[ebp-8]
	mov ebx,[ebp-24]
	add eax,ebx
	mov [ebp-28],eax
	lea eax,[ebp-8]
	mov ebx,[ebp-28]
	mov [eax],ebx
	lea esp,[ebp-12]
	jmp @for_2_loop
@for_2_blcok:
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
	je @lab_cpystr2_exit_36
	mov ecx,@str_2_len
	dec ecx
	mov esi,@str_2
@lab_cpystr2_35:
	cmp ecx,-1
	je @lab_cpystr2_exit_36
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_35
@lab_cpystr2_exit_36:
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
	jge @lab_numsign1_exit_40
@lab_numsign1_39:
	neg eax
	mov edi,1
@lab_numsign1_exit_40:
	mov ebx,10
@lab_num2str1_37:
	mov edx,0
	idiv ebx
	mov cl,[esi]
	inc cl
	mov [esi],cl
	sub esp,1
	add dl,48
	mov [esp],dl
	cmp eax,0
	jne @lab_num2str1_37
	cmp edi,0
	je @lab_numsign_add_41
	sub esp,1
	mov ecx,45
	mov [esp],cl
	mov cl,[esi]
	inc cl
	mov [esi],cl
@lab_numsign_add_41:
	cmp cl,255
	jna @lab_num2str1_exit_38
	call @str2long
@lab_num2str1_exit_38:
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
	mov ecx,[ebp-48]
	mov edx,0
	mov dl,[ecx]
	sub ecx,edx
	mov ebx,1
	mov eax,4
	int 128
	lea esp,[ebp-12]
	jmp @for_2_exit
	jmp @for_2_iter
@for_2_exit:
	lea esp,[ebp-4]
	lea esp,[ebp-4]
	jmp @switch_tab_1_end
	lea esp,[ebp-4]
@case_1_2_lab:
@for_3_start:
	push 0
	push 0
	lea eax,[ebp-8]
	mov ebx,[ebp-12]
	mov [eax],ebx
@for_3_loop:
	push 20
	push 0
	mov eax,[ebp-8]
	mov ebx,[ebp-16]
	cmp eax,ebx
	jl @lab_base_cmp_50
	mov eax,0
	jmp @lab_base_cmp_exit_51
@lab_base_cmp_50:
	mov eax,1
@lab_base_cmp_exit_51:
	mov [ebp-20],eax
	mov eax,[ebp-20]
	cmp eax,0
	je @for_3_exit
	push 1
	push 0
	jmp @for_3_blcok
@for_3_iter:
	mov eax,[ebp-8]
	mov ebx,[ebp-24]
	add eax,ebx
	mov [ebp-28],eax
	lea eax,[ebp-8]
	mov ebx,[ebp-28]
	mov [eax],ebx
	lea esp,[ebp-12]
	jmp @for_3_loop
@for_3_blcok:
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
	je @lab_cpystr2_exit_59
	mov ecx,@str_3_len
	dec ecx
	mov esi,@str_3
@lab_cpystr2_58:
	cmp ecx,-1
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
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[ebp-36]
	mov esi,[ebp-44]
	mov edi,0
	cmp eax,0
	jge @lab_numsign1_exit_63
@lab_numsign1_62:
	neg eax
	mov edi,1
@lab_numsign1_exit_63:
	mov ebx,10
@lab_num2str1_60:
	mov edx,0
	idiv ebx
	mov cl,[esi]
	inc cl
	mov [esi],cl
	sub esp,1
	add dl,48
	mov [esp],dl
	cmp eax,0
	jne @lab_num2str1_60
	cmp edi,0
	je @lab_numsign_add_64
	sub esp,1
	mov ecx,45
	mov [esp],cl
	mov cl,[esi]
	inc cl
	mov [esi],cl
@lab_numsign_add_64:
	cmp cl,255
	jna @lab_num2str1_exit_61
	call @str2long
@lab_num2str1_exit_61:
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
	je @lab_cpystr2_exit_67
	mov ecx,0
	mov esi,ebx
	dec esi
	neg eax
@lab_cpystr2_66:
	cmp ecx,eax
	je @lab_cpystr2_exit_67
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_66
@lab_cpystr2_exit_67:
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
	lea esp,[ebp-12]
	jmp @for_3_exit
	jmp @for_3_iter
@for_3_exit:
	lea esp,[ebp-4]
	lea esp,[ebp-4]
	jmp @switch_tab_1_end
	lea esp,[ebp-4]
@case_1_10_lab:
@for_4_start:
	push 0
	push 0
	lea eax,[ebp-8]
	mov ebx,[ebp-12]
	mov [eax],ebx
@for_4_loop:
	push 20
	push 0
	mov eax,[ebp-8]
	mov ebx,[ebp-16]
	cmp eax,ebx
	jl @lab_base_cmp_73
	mov eax,0
	jmp @lab_base_cmp_exit_74
@lab_base_cmp_73:
	mov eax,1
@lab_base_cmp_exit_74:
	mov [ebp-20],eax
	mov eax,[ebp-20]
	cmp eax,0
	je @for_4_exit
	push 1
	push 0
	jmp @for_4_blcok
@for_4_iter:
	mov eax,[ebp-8]
	mov ebx,[ebp-24]
	add eax,ebx
	mov [ebp-28],eax
	lea eax,[ebp-8]
	mov ebx,[ebp-28]
	mov [eax],ebx
	lea esp,[ebp-12]
	jmp @for_4_loop
@for_4_blcok:
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
	push 4
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,@str_4_len
	sub esp,1
	mov [esp],al
	mov [ebp-44],esp
	cmp eax,0
	je @lab_cpystr2_exit_82
	mov ecx,@str_4_len
	dec ecx
	mov esi,@str_4
@lab_cpystr2_81:
	cmp ecx,-1
	je @lab_cpystr2_exit_82
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_81
@lab_cpystr2_exit_82:
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
	jge @lab_numsign1_exit_86
@lab_numsign1_85:
	neg eax
	mov edi,1
@lab_numsign1_exit_86:
	mov ebx,10
@lab_num2str1_83:
	mov edx,0
	idiv ebx
	mov cl,[esi]
	inc cl
	mov [esi],cl
	sub esp,1
	add dl,48
	mov [esp],dl
	cmp eax,0
	jne @lab_num2str1_83
	cmp edi,0
	je @lab_numsign_add_87
	sub esp,1
	mov ecx,45
	mov [esp],cl
	mov cl,[esi]
	inc cl
	mov [esi],cl
@lab_numsign_add_87:
	cmp cl,255
	jna @lab_num2str1_exit_84
	call @str2long
@lab_num2str1_exit_84:
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
	je @lab_cpystr2_exit_90
	mov ecx,0
	mov esi,ebx
	dec esi
	neg eax
@lab_cpystr2_89:
	cmp ecx,eax
	je @lab_cpystr2_exit_90
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_89
@lab_cpystr2_exit_90:
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
	lea esp,[ebp-12]
	jmp @for_4_exit
	jmp @for_4_iter
@for_4_exit:
	lea esp,[ebp-4]
	lea esp,[ebp-4]
	jmp @switch_tab_1_end
	lea esp,[ebp-4]
@case_1_50_lab:
@for_5_start:
	push 0
	push 0
	lea eax,[ebp-8]
	mov ebx,[ebp-12]
	mov [eax],ebx
@for_5_loop:
	push 20
	push 0
	mov eax,[ebp-8]
	mov ebx,[ebp-16]
	cmp eax,ebx
	jl @lab_base_cmp_96
	mov eax,0
	jmp @lab_base_cmp_exit_97
@lab_base_cmp_96:
	mov eax,1
@lab_base_cmp_exit_97:
	mov [ebp-20],eax
	mov eax,[ebp-20]
	cmp eax,0
	je @for_5_exit
	push 1
	push 0
	jmp @for_5_blcok
@for_5_iter:
	mov eax,[ebp-8]
	mov ebx,[ebp-24]
	add eax,ebx
	mov [ebp-28],eax
	lea eax,[ebp-8]
	mov ebx,[ebp-28]
	mov [eax],ebx
	lea esp,[ebp-12]
	jmp @for_5_loop
@for_5_blcok:
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
	push 5
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,@str_5_len
	sub esp,1
	mov [esp],al
	mov [ebp-44],esp
	cmp eax,0
	je @lab_cpystr2_exit_105
	mov ecx,@str_5_len
	dec ecx
	mov esi,@str_5
@lab_cpystr2_104:
	cmp ecx,-1
	je @lab_cpystr2_exit_105
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_104
@lab_cpystr2_exit_105:
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
	jge @lab_numsign1_exit_109
@lab_numsign1_108:
	neg eax
	mov edi,1
@lab_numsign1_exit_109:
	mov ebx,10
@lab_num2str1_106:
	mov edx,0
	idiv ebx
	mov cl,[esi]
	inc cl
	mov [esi],cl
	sub esp,1
	add dl,48
	mov [esp],dl
	cmp eax,0
	jne @lab_num2str1_106
	cmp edi,0
	je @lab_numsign_add_110
	sub esp,1
	mov ecx,45
	mov [esp],cl
	mov cl,[esi]
	inc cl
	mov [esi],cl
@lab_numsign_add_110:
	cmp cl,255
	jna @lab_num2str1_exit_107
	call @str2long
@lab_num2str1_exit_107:
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
	je @lab_cpystr2_exit_113
	mov ecx,0
	mov esi,ebx
	dec esi
	neg eax
@lab_cpystr2_112:
	cmp ecx,eax
	je @lab_cpystr2_exit_113
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_112
@lab_cpystr2_exit_113:
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
	lea esp,[ebp-12]
	jmp @for_5_exit
	jmp @for_5_iter
@for_5_exit:
	lea esp,[ebp-4]
	lea esp,[ebp-4]
	jmp @switch_tab_1_end
	lea esp,[ebp-4]
@case_1_999_lab:
	push 6
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,@str_6_len
	sub esp,1
	mov [esp],al
	mov [ebp-12],esp
	cmp eax,0
	je @lab_cpystr2_exit_119
	mov ecx,@str_6_len
	dec ecx
	mov esi,@str_6
@lab_cpystr2_118:
	cmp ecx,-1
	je @lab_cpystr2_exit_119
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_118
@lab_cpystr2_exit_119:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[ebp-4]
	mov esi,[ebp-12]
	mov edi,0
	cmp eax,0
	jge @lab_numsign1_exit_123
@lab_numsign1_122:
	neg eax
	mov edi,1
@lab_numsign1_exit_123:
	mov ebx,10
@lab_num2str1_120:
	mov edx,0
	idiv ebx
	mov cl,[esi]
	inc cl
	mov [esi],cl
	sub esp,1
	add dl,48
	mov [esp],dl
	cmp eax,0
	jne @lab_num2str1_120
	cmp edi,0
	je @lab_numsign_add_124
	sub esp,1
	mov ecx,45
	mov [esp],cl
	mov cl,[esi]
	inc cl
	mov [esi],cl
@lab_numsign_add_124:
	cmp cl,255
	jna @lab_num2str1_exit_121
	call @str2long
@lab_num2str1_exit_121:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov ebx,[ebp-12]
	mov eax,0
	mov al,[ebx]
	sub esp,1
	mov [esp],al
	mov [ebp-16],esp
	cmp eax,0
	je @lab_cpystr2_exit_127
	mov ecx,0
	mov esi,ebx
	dec esi
	neg eax
@lab_cpystr2_126:
	cmp ecx,eax
	je @lab_cpystr2_exit_127
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_126
@lab_cpystr2_exit_127:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov ecx,[ebp-16]
	mov edx,0
	mov dl,[ecx]
	sub ecx,edx
	mov ebx,1
	mov eax,4
	int 128
	lea esp,[ebp-4]
@default_1_lab:
	push 7
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,@str_7_len
	sub esp,1
	mov [esp],al
	mov [ebp-12],esp
	cmp eax,0
	je @lab_cpystr2_exit_133
	mov ecx,@str_7_len
	dec ecx
	mov esi,@str_7
@lab_cpystr2_132:
	cmp ecx,-1
	je @lab_cpystr2_exit_133
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_132
@lab_cpystr2_exit_133:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov ecx,[ebp-12]
	mov edx,0
	mov dl,[ecx]
	sub ecx,edx
	mov ebx,1
	mov eax,4
	int 128
	lea esp,[ebp-4]
	jmp @switch_tab_1_end
	lea esp,[ebp-4]
	jmp @switch_1_end
@switch_1_end:
	lea esp,[ebp-4]
	jmp @switch_tab_1_end
@switch_1_table:
	mov eax,[ebp-4]
	cmp eax,999
	jg @default_1_end
	cmp eax,1
	jl @default_1_end
@case_1_2_jmp:
	cmp eax,10
	je @case_1_10_lab
	jl @case_1_0_jmp
	jg @case_1_4_jmp
@case_1_0_jmp:
	cmp eax,1
	je @case_1_1_lab
	jg @case_1_1_jmp
	jmp @default_1_end
@case_1_1_jmp:
	cmp eax,2
	je @case_1_2_lab
	jmp @default_1_end
@case_1_4_jmp:
	cmp eax,100
	je @case_1_100_lab
	jl @case_1_3_jmp
	jg @case_1_5_jmp
@case_1_3_jmp:
	cmp eax,50
	je @case_1_50_lab
	jmp @default_1_end
@case_1_5_jmp:
	cmp eax,999
	je @case_1_999_lab
	jmp @default_1_end
@default_1_end:
	jmp @default_1_lab
@switch_tab_1_end:
	push 0
	lea eax,[ebp-8]
	mov ebx,[ebp-4]
	mov [eax],ebx
	push 8
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,@str_8_len
	sub esp,1
	mov [esp],al
	mov [ebp-16],esp
	cmp eax,0
	je @lab_cpystr2_exit_139
	mov ecx,@str_8_len
	dec ecx
	mov esi,@str_8
@lab_cpystr2_138:
	cmp ecx,-1
	je @lab_cpystr2_exit_139
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_138
@lab_cpystr2_exit_139:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[ebp-8]
	mov esi,[ebp-16]
	mov edi,0
	cmp eax,0
	jge @lab_numsign1_exit_143
@lab_numsign1_142:
	neg eax
	mov edi,1
@lab_numsign1_exit_143:
	mov ebx,10
@lab_num2str1_140:
	mov edx,0
	idiv ebx
	mov cl,[esi]
	inc cl
	mov [esi],cl
	sub esp,1
	add dl,48
	mov [esp],dl
	cmp eax,0
	jne @lab_num2str1_140
	cmp edi,0
	je @lab_numsign_add_144
	sub esp,1
	mov ecx,45
	mov [esp],cl
	mov cl,[esi]
	inc cl
	mov [esi],cl
@lab_numsign_add_144:
	cmp cl,255
	jna @lab_num2str1_exit_141
	call @str2long
@lab_num2str1_exit_141:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov ebx,[ebp-16]
	mov eax,0
	mov al,[ebx]
	sub esp,1
	mov [esp],al
	mov [ebp-20],esp
	cmp eax,0
	je @lab_cpystr2_exit_147
	mov ecx,0
	mov esi,ebx
	dec esi
	neg eax
@lab_cpystr2_146:
	cmp ecx,eax
	je @lab_cpystr2_exit_147
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_146
@lab_cpystr2_exit_147:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov ecx,[ebp-20]
	mov edx,0
	mov dl,[ecx]
	sub ecx,edx
	mov ebx,1
	mov eax,4
	int 128
	push 9
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[ebp-8]
	sub esp,1
	mov ecx,0
	mov [esp],cl
	mov esi,esp
	mov [ebp-28],esp
	mov edi,0
	cmp eax,0
	jge @lab_numsign2_exit_155
@lab_numsign2_154:
	neg eax
	mov edi,1
@lab_numsign2_exit_155:
	mov ebx,10
@lab_num2str2_152:
	mov edx,0
	idiv ebx
	mov cl,[esi]
	inc cl
	mov [esi],cl
	sub esp,1
	add dl,48
	mov [esp],dl
	cmp eax,0
	jne @lab_num2str2_152
	cmp edi,0
	je @lab_num2str2_exit_153
	sub esp,1
	mov ecx,45
	mov [esp],cl
	mov cl,[esi]
	inc cl
	mov [esi],cl
@lab_num2str2_exit_153:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,@str_9_len
	cmp eax,0
	je @lab_cpystr1_exit_157
	mov ebx,[ebp-28]
	mov edx,0
	mov dl,[ebx]
	add edx,eax
	mov [ebx],dl
	mov ecx,@str_9_len
	dec ecx
	mov esi,@str_9
	cmp edx,255
	jna @lab_cpystr1_156
	call @str2long
@lab_cpystr1_156:
	cmp ecx,-1
	je @lab_cpystr1_exit_157
	mov al,[esi+ecx]
	sub esp,1
	mov [esp],al
	dec ecx
	jmp @lab_cpystr1_156
@lab_cpystr1_exit_157:
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov ecx,0
	mov esi,[ebp-28]
	mov cl,[esi]
	cmp ecx,0
	je @lab_cpy2gstr_exit_159
	mov [@str_t_len],cl
	sub esi,ecx
	mov edi,@str_t
	mov edx,0
@lab_cpy2gstr_158:
	mov al,[esi+edx]
	mov [edi+edx],al
	inc edx
	cmp edx,ecx
	je @lab_cpy2gstr_exit_159
	jmp @lab_cpy2gstr_158
@lab_cpy2gstr_exit_159:
	push -1
	mov eax,[@s_esp]
	mov [@s_esp],esp
	mov esp,eax
	mov eax,0
	mov al,[@str_t_len]
	sub esp,1
	mov [esp],al
	mov [ebp-32],esp
	cmp eax,0
	je @lab_cpystr2_exit_162
	dec eax
	mov ecx,eax
	mov esi,@str_t
@lab_cpystr2_161:
	cmp ecx,-1
	je @lab_cpystr2_exit_162
	mov dl,[esi+ecx]
	sub esp,1
	mov [esp],dl
	dec ecx
	jmp @lab_cpystr2_161
@lab_cpystr2_exit_162:
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
	push 0
	mov eax,[ebp-36]
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
	@str_t times 255 db 0
	@str_t_len db 0
	@str_1 db 10
	@str_1_len equ 1
	@str_2 db 10
	@str_2_len equ 1
	@str_3 db 10
	@str_3_len equ 1
	@str_4 db 10
	@str_4_len equ 1
	@str_5 db 10
	@str_5_len equ 1
	@str_6 db 10
	@str_6_len equ 1
	@str_7 db "this is default!",10
	@str_7_len equ 17
	@str_8 db 10
	@str_8_len equ 1
	@str_9 db "213"
	@str_9_len equ 3
section .bss
