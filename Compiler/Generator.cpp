#include "Generator.h"

bool Generator::buffer_flag = false;
int Generator::id = 0;
bool Generator::error = false;
FILE* Generator::fout = nullptr;


bool Generator::Init(const char* file_name) {
	fout = fopen(file_name, "w");
	if (!fout) {
		cout << "file [" << file_name << "] open failed!" << endl;
		return false;
	}
	else {
		cout << "file [" << file_name << "] open successfully!" << endl;
		return true;
	}
}

string Generator::GenerateName(string header, symbol type, string name) {
	id++;
	string return_name = "@" + header;
	if (type != null) {
		return_name += "_";
		return_name += symbol_data[type];
	}
	if (name != "") {
		return_name += "_" + name;
	}
	if (header != "str" && header != "fun" && header != "var") {
		return_name += "_" + to_string(id);
	}
	return return_name;
}

VarRecord* Generator::GenerateExp(VarRecord* f1, symbol op, VarRecord* f2, int& var_number, FunRecord& fun) {
	if (error) {
		return nullptr;
	}
	if (!f1 || !f2) {
		return nullptr;
	}
	if (f1->get_type() == rev_void || f2->get_type() == rev_void) {
		SemanticError(void_non_calc);
		return nullptr;
	}
	symbol ret_type = rev_int;
	if (f1->get_type() == rev_string || f2->get_type() == rev_string) {
		if (op == add) {
			ret_type = rev_string;
		}
		else {
			SemanticError(str_non_add);
			return nullptr;
		}
	}
	else {
		if (op == gt || op == ge || op == lt ||
			op == le || op == equ || op == nequ) {
			ret_type = rev_char;
		}
	}

	// create var
	VarRecord* tmp = fun.CreateTmpVar(ret_type, false, var_number);
	string lab_loop, lab_exit;
	switch (ret_type) {
		case rev_string: {
			if (f2->get_type() == rev_string) {
				lab_loop = GenerateName("lab", null, "cpystr2");
				lab_exit = GenerateName("lab", null, "cpystr2_exit");
				if (f2->get_value() == DYNAMIC_STRING) {
					exchg_esp();
					if (f2->get_local_addr() < 0) {
						mov(ebx, __ebp(f2->get_local_addr()));
					}
					else {
						mov(ebx, _ebp(f2->get_local_addr()));
					}
					mov(eax, 0);
					mov(al, __(ebx));
					subi(esp, 1);
					mov(__(esp), al);
					mov(__ebp(tmp->get_local_addr()), esp);// 存入字符串的地址
					cmp(eax, 0);
					je(lab_exit);
					mov(ecx, 0);
					mov(esi, ebx);
					dec(esi);
					neg(eax);
					label(lab_loop);
					cmp(ecx, eax);
					je(lab_exit);
					mov(dl, _(esi, ecx));
					subi(esp, 1);
					mov(__(esp), dl);
					dec(ecx);
					jmp(lab_loop);
					label(lab_exit);
					// 字符串拷贝结束
					exchg_esp();
				}
				else if (f2->get_value() > 0) {
				// 常量String
					exchg_esp();
					mov(eax, "@str_" + to_string(f2->get_value()) + "_len");
					subi(esp, 1);
					mov(__(esp), al);
					mov(__ebp(tmp->get_local_addr()), esp);// 字符串的起始地址的指针放入真正的栈中
					cmp(eax, 0);
					je(lab_exit);
					mov(ecx, "@str_" + to_string(f2->get_value()) + "_len");
					dec(ecx);
					mov(esi, "@str_" + to_string(f2->get_value()));
					label(lab_loop);
					cmp(ecx, -1);
					je(lab_exit);
					mov(dl, _(esi, ecx));
					subi(esp, 1);
					mov(__(esp), dl);
					dec(ecx);
					jmp(lab_loop);
					label(lab_exit);
					exchg_esp();
				}
				else if (f2->get_value() == GLOBAL_STRING) {
					exchg_esp();
					mov(eax, 0);
					if (!buffer_flag) {
						mov(al, __("@str_" + f2->get_name() + "_len"));
					}
					else {
						mov(al, __(f2->get_name() + "_len"));
					}
					subi(esp, 1);
					mov(__(esp), al);
					mov(__ebp(tmp->get_local_addr()), esp);
					cmp(eax, 0);
					je(lab_exit);
					dec(eax);
					mov(ecx, eax);
					if (!buffer_flag) {
						mov(esi, "@str_" + f2->get_name());
					}
					else {
						mov(esi, f2->get_name());
					}
					label(lab_loop);
					cmp(ecx, -1);
					je(lab_exit);
					mov(dl, _(esi, ecx));
					subi(esp, 1);
					mov(__(esp), dl);
					dec(ecx);
					jmp(lab_loop);
					label(lab_exit);
					exchg_esp();
				}
				else if (f2->get_value() == 0) {
					exchg_esp();
					mov(eax, 0);
					subi(esp, 1);
					mov(__(esp), al);
					mov(__ebp(tmp->get_local_addr()), esp);
					exchg_esp();
				}
			}
			else if (f2->get_type() == rev_int) {
				lab_loop = GenerateName("lab", null, "num2str2");
				lab_exit = GenerateName("lab", null, "num2str2_exit");
				string lab_num_sign = GenerateName("lab", null, "numsign2");
				string lab_num_sign_exit = GenerateName("lab", null, "numsign2_exit");
				exchg_esp();
				if (f2->get_local_addr() == 0) {
					// 全局变量
					mov(eax, __("@var_" + f2->get_name()));
				}
				else {
					// 局部变量
					if (f2->get_local_addr() < 0) {
						mov(eax, __ebp(f2->get_local_addr()));
					}
					else {
						mov(eax, _ebp(f2->get_local_addr()));
					}
				}
				subi(esp, 1);
				mov(ecx, 0);
				mov(__(esp), cl);
				mov(esi, esp);
				mov(__ebp(tmp->get_local_addr()), esp);
				mov(edi, 0);// 保存eax符号
				cmp(eax, 0);
				jge(lab_num_sign_exit); // 正数
				label(lab_num_sign);
				neg(eax);
				mov(edi, 1);
				label(lab_num_sign_exit);
				mov(ebx, 10);
				label(lab_loop);
				mov(edx, 0);
				idiv(ebx);
				mov(cl, __(esi));
				inc(cl);
				mov(__(esi), cl);
				subi(esp, 1);
				addi(dl, 48);
				mov(__(esp), dl);
				cmp(eax, 0);
				jne(lab_loop);
				cmp(edi, 0);
				je(lab_exit);
				subi(esp, 1);
				mov(ecx, '-');
				mov(__(esp), cl);
				mov(cl, __(esi));
				inc(cl);
				mov(__(esi), cl);
				label(lab_exit);
				exchg_esp();
			}
			else if (f2->get_type() == rev_char) {
				exchg_esp();
				if (f2->get_local_addr() == 0) {
					mov(eax, _("@var_" + f2->get_name()));
				}
				else {
					if (f2->get_local_addr() < 0) {
						mov(eax, __ebp(f2->get_local_addr()));
					}
					else {
						mov(eax, _ebp(f2->get_local_addr()));
					}
					subi(esp, 1);
					mov(bl, 1);
					mov(_(esp), bl);
					mov(__ebp(tmp->get_local_addr()), esp);
					subi(esp, 1);
					mov(_(esp), al);
					exchg_esp();
				}
			}

			if (f1->get_type() == rev_string) {
				lab_loop = GenerateName("lab", null, "cpystr1");
				lab_exit = GenerateName("lab", null, "cpystr1_exit");
				if (f1->get_value() == DYNAMIC_STRING) {
					exchg_esp();
					if (f1->get_local_addr() < 0) {
						mov(ebx, __ebp(f1->get_local_addr()));
					}
					else {
						mov(ebx, _ebp(f1->get_local_addr()));
					}
					mov(eax, 0);
					mov(al, __(ebx));
					cmp(eax, 0);
					je(lab_exit);
					mov(ebx, __ebp(tmp->get_local_addr()));
					mov(edx, 0);
					mov(dl, __(ebx));
					addi(edx, eax);
					mov(__(ebx), dl);
					mov(ecx, 0);
					if (f1->get_local_addr() < 0) {
						mov(esi, __ebp(f1->get_local_addr()));
					}
					else {
						mov(esi, _ebp(f1->get_local_addr()));
					}
					subi(esi, 1);
					neg(eax);//需要拷贝字符串的长度
					cmp(edx, 255);
					jna(lab_loop);
					call(STR2LONG);
					label(lab_loop);
					cmp(ecx, eax);
					je(lab_exit);
					mov(dl, _(esi, ecx));
					subi(esp, 1);
					mov(__(esp), dl);
					dec(ecx);
					jmp(lab_loop);
					label(lab_exit);
					exchg_esp();

				}
				else if(f1->get_value() > 0) {
					// 常量 string
					exchg_esp();
					mov(eax, ("@str_" + to_string(f1->get_value()) + "_len"));
					cmp(eax, 0);
					je(lab_exit);
					mov(ebx, __ebp(tmp->get_local_addr()));
					mov(edx, 0);
					mov(dl, __(ebx));
					addi(edx, eax);
					mov(__(ebx), dl);

					mov(ecx, "@str_" + to_string(f1->get_value()) + "_len");
					dec(ecx);
					mov(esi, "@str_" + to_string(f1->get_value()));
					cmp(edx, 255);
					jna(lab_loop);
					call(STR2LONG);
					label(lab_loop);
					cmp(ecx, -1);
					je(lab_exit);
					mov(al, _(esi, ecx));
					subi(esp, 1);
					mov(__(esp), al);
					dec(ecx);
					jmp(lab_loop);
					label(lab_exit);
					exchg_esp();
				}
				else if (f1->get_value() == GLOBAL_STRING) {
					exchg_esp();
					mov(eax, 0);
					if (!buffer_flag) {
						mov(al, __("@str_" + f1->get_name() + "_len"));
					}
					else {
						mov(al, __(f1->get_name() + "_len"));
					}
					cmp(eax, 0);
					je(lab_exit);
					mov(ebx, __ebp(tmp->get_local_addr()));
					mov(edx, 0);
					mov(dl, __(ebx));
					addi(edx, eax);
					mov(__(ebx), dl);
					dec(eax);
					mov(ecx, eax);
					if (!buffer_flag) {
						mov(esi, "@str_" + f1->get_local_addr());
					}
					else {
						mov(esi, f1->get_name());
					}
					cmp(edx, 255);
					jna(lab_loop);
					call(STR2LONG);
					label(lab_loop);
					cmp(ecx, -1);
					je(lab_exit);
					mov(al, _(esi, ecx));
					subi(esp, 1);
					mov(__(esp), al);
					dec(ecx);
					jmp(lab_loop);
					label(lab_exit);
					exchg_esp();
				}
			}
			else if (f1->get_type() == rev_int) {
				lab_loop = GenerateName("lab", null, "num2str1");
				lab_exit = GenerateName("lab", null, "num2str1_exit");
				string lab_num_sign = GenerateName("lab", null, "numsign1");
				string lab_num_sign_exit = GenerateName("lab", null, "numsign1_exit");
				string lab2long = GenerateName("lab", null, "numsign_add");
				exchg_esp();
				if (f1->get_local_addr() == 0){
					mov(eax, __("@var_" + f1->get_name()));
				}
				else {
					if (f1->get_local_addr() < 0) {
						mov(eax, __ebp(f1->get_local_addr()));
					}
					else {
						mov(eax, _ebp(f1->get_local_addr()));
					}
					mov(esi, __ebp(tmp->get_local_addr()));
					mov(edi, 0); // 0+ 1-
					cmp(eax, 0);
					jge(lab_num_sign_exit);
					label(lab_num_sign);
					neg(eax);
					mov(edi, 1);
					label(lab_num_sign_exit);
					mov(ebx, 10);
					label(lab_loop);
					mov(edx, 0);
					idiv(ebx);
					mov(cl, __(esi));
					inc(cl);
					mov(__(esi), cl);
					subi(esp, 1);
					addi(dl, 48);
					mov(__(esp), dl);
					cmp(eax, 0);
					jne(lab_loop);
					cmp(edi, 0);
					je(lab2long);
					subi(esp, 1);
					mov(ecx, '-');
					mov(__(esp), cl);
					mov(cl, __(esi));
					inc(cl);
					mov(__(esi), cl);
					label(lab2long);
					cmp(cl, 255);
					jna(lab_exit);
					call(STR2LONG);
					label(lab_exit);
					exchg_esp();
				}

			}
			else if (f1->get_type() == rev_char) {
				lab_loop = GenerateName("lab", null, "char2str_exit");
				exchg_esp();
				if (f1->get_local_addr() == 0) {
					mov(eax, __("@var_" + f1->get_name()));
				}
				else {
					if (f1->get_local_addr() < 0) {
						mov(eax, __ebp(f1->get_local_addr()));
					}
					else {
						mov(eax, _ebp(f1->get_local_addr()));
					}
					mov(esi, __ebp(tmp->get_externed()));
					mov(cl, __(esi));
					inc(cl);
					mov(__(esi), cl);
					subi(esp, 1);
					mov(__(esp), al);

					cmp(cl, 255);
					jna(lab_exit);
					call(STR2LONG);
					label(lab_exit);
					exchg_esp();
				}
			}
			break;
		}
		case rev_int: {
			if(f1->get_local_addr() == 0){
				mov(eax, __("@var_" + f1->get_name()));
			}
			else {
				if (f1->get_local_addr() < 0) {
					mov(eax, __ebp(f1->get_local_addr()));
				}
				else {
					mov(eax, _ebp(f1->get_local_addr()));
				}
			}
			if (f2->get_local_addr() == 0) {
				mov(ebx, __("@var_" + f2->get_name()));
			}
			else {
				if (f2->get_local_addr() < 0) {
					mov(ebx, __ebp(f2->get_local_addr()));
				}
				else {
					mov(ebx, _ebp(f2->get_local_addr()));
				}
			}
			switch (op)
			{
				case add:{
					addi(eax, ebx);
					break;
				}
				case sub: {
					subi(eax, ebx);
					break;
				}
				case mult: {
					multi(ebx);
					break;
				}
				case divi: {
					mov(edx, 0);
					idiv(ebx);
					break;
				}
				default: {
					// assert
				}
				break;
			}
			mov(__ebp(tmp->get_local_addr()), eax);
			break;
		}
		case rev_char: {
			if (f1->get_type() == rev_string) {
				// semantic error
			}
			else {
				lab_loop = GenerateName("lab", null, "base_cmp");
				lab_exit = GenerateName("lab", null, "base_cmp_exit");
				if (f1->get_local_addr() == 0) {
					mov(eax, __("@var_" + f1->get_name()));
				}
				else {
					if (f1->get_local_addr() < 0) {
						mov(eax, __ebp(f1->get_local_addr()));
					}
					else {
						mov(eax, _ebp(f1->get_local_addr()));
					}
				}

				if (f1->get_local_addr() == 0) {
					mov(ebx, __("@var_" + f2->get_name()));
				}
				else {
					if (f2->get_local_addr() < 0) {
						mov(ebx, __ebp(f2->get_local_addr()));
					}
					else {
						mov(ebx, _ebp(f2->get_local_addr()));
					}
				}
				cmp(eax, ebx);
				switch (op) {
					case gt:{
						jg(lab_loop);
						break;
					}
					case ge: {
						jge(lab_loop);
						break;
					}
					case lt: {
						jl(lab_loop);
						break;
					}
					case le: {
						jle(lab_loop);
						break;
					}
					case equ: {
						je(lab_loop);
						break;
					}
					case nequ: {
						jne(lab_loop);
						break;
					}
					default: {
						// assert
						break;
					}
				}
				mov(eax, 0);
				jmp(lab_exit);
				label(lab_loop);
				mov(eax, 1);
				label(lab_exit);
				mov(__ebp(tmp->get_local_addr()), eax);
			}
			break;
		}
		default: {
			// assert
			break;
		}
	}
	return tmp;
}

VarRecord* Generator::GenerateAssign(VarRecord* des, VarRecord* src, int& var_number, FunRecord& fun) {
	if (error)
		return nullptr;
	if (des->get_type() == rev_void) {
		SemanticError(void_non_assi);
		return nullptr;
	}
	if (des->get_type() == rev_int && src->get_type() == rev_char) {
		// warning
	}
	else if (des->get_type() == rev_char && src->get_type() == rev_int) {
		// warning
	}
	else if (des->get_type() != src->get_type()) {
		SemanticError(type_assi);
		return nullptr;
	}

	if (des->get_type() == rev_string) {
		if (src->get_value() != DYNAMIC_STRING) {
			VarRecord tmp;
			string temp_name = "";
			tmp.Init(rev_string, temp_name);
			src = GenerateExp(&tmp, add, src, var_number, fun);
		}
		if (des->get_value() == GLOBAL_STRING) {
			string lab_loop = GenerateName("lab", null, "cpy2gstr");
			string lab_exit = GenerateName("lab", null, "cpy2gstr_exit");
			mov(ecx, 0);
			if (src->get_local_addr() < 0) {
				mov(esi, __ebp(src->get_local_addr()));
			}
			else {
				mov(esi, _ebp(src->get_local_addr()));
			}
			mov(cl, __(esi));
			cmp(ecx, 0);
			je(lab_exit);
			mov(__("@str_" + des->get_name() + "_len"), cl);
			subi(esi, ecx);
			mov(edi, "@str_" + des->get_name());
			mov(edx, 0);
			label(lab_loop);
			mov(al, _(esi, edx));
			mov(_(edi, edx), al);
			inc(edx);
			cmp(edx, ecx);
			je(lab_exit);
			jmp(lab_loop);
			label(lab_exit);
		}
		else {
			des->set_value(DYNAMIC_STRING);
			if (src->get_local_addr() < 0) {
				mov(eax, __ebp(src->get_local_addr()));
			}
			else {
				mov(eax, _ebp(src->get_local_addr()));
			}

			if (des->get_local_addr() < 0) {
				mov(__ebp(des->get_local_addr()), eax);
			}
			else {
				mov(_ebp(des->get_local_addr()), eax);
			}
		}
	}
	else {
		if (des->get_local_addr() == 0) {
			mov(eax, "@var_" + des->get_name());
		}
		else {
			if (des->get_local_addr() < 0) {
				lea(eax, __ebp(des->get_local_addr()));
			}
			else {
				lea(eax, _ebp(des->get_local_addr()));
			}
			if (src->get_local_addr() == 0) {
				mov(ebx, __("@var_" + src->get_name()));
			}
			else {
				if (src->get_local_addr() < 0) {
					mov(ebx, __ebp(src->get_local_addr()));
				}
				else {
					mov(ebx, _ebp(src->get_local_addr()));
				}
			}
			mov(__(eax), ebx);
		}
	}
	return des;
}

void Generator::GenerateReturn(VarRecord* ret, int& var_number,FunRecord&fun) {
	if (error) {
		return;
	}
	if (ret != nullptr) {
		if (ret->get_type() == rev_string) {
			VarRecord tmp;
			string temp_name = "";
			tmp.Init(rev_string, temp_name);
			ret = GenerateExp(&tmp, add, ret, var_number, fun);
			
			if (ret->get_local_addr() < 0) {
				mov(eax, __ebp(ret->get_local_addr()));
			}
			else {
				mov(eax, _ebp(ret->get_local_addr()));
			}
		}
		else {
			if (ret->get_local_addr() == 0) {
				mov(eax, __("@var_" + ret->get_name()));
			}
			else {
				if (ret->get_local_addr() < 0) {
					mov(eax, __ebp(ret->get_local_addr()));
				}
				else {
					mov(eax, _ebp(ret->get_local_addr()));
				}
			}
		}
	}
	// 函数结束清理工作
	mov(ebx, __(EBP));
	mov(__(ESP), ebx);
	exchg_esp(ebx);
	pop(ebx);
	mov(__(EBP), ebx);
	exchg_esp(ebx);
	mov(esp, ebp);
	pop(ebp);
	Generator::ret();
}

void Generator::GenerateFunHead(FunRecord& fun){
	if (error) {
		return;
	}
	label(fun.get_name());
	push(ebp);
	mov(ebp, esp);

	exchg_esp(ebx);
	mov(ebx, __(EBP));
	push(ebx);
	mov(__(EBP), esp);
	exchg_esp(ebx);
	return;
}

void Generator::GenerateFunTail(FunRecord& fun){
	if (error) return;
	if (fun.get_has_return()) return;
	mov(ebx, __(EBP));
	mov(__(ESP), ebx);
	exchg_esp(ebx);
	pop(ebx);
	mov(__(EBP), ebx);
	exchg_esp(ebx);
	mov(esp, ebp);
	pop(ebp);
	Generator::ret();
}

void Generator::GenerateLocalVar(int value) {
	if (error) return;
	push(value);
}

int Generator::GenerateBlock(int in,FunRecord& fun) {
	if (error) return -1;
	if (in == -1) {
		return fun.GetCurrentAddr();
	}
	else {
		if (in != 0) {
			lea(esp, __ebp(in));
		}
		else {
			mov(esp, ebp);
		}
		return -2;
	}
}

void Generator::GenerateCondition(VarRecord* condition) {
	if (error) return;
	if (condition == nullptr)return;
	if (condition->get_type() == rev_string) {
		SemanticError(str_non_cond);
	}
	else if(condition->get_type() == rev_void) {
		SemanticError(void_non_cond);
	}
	else {
		if (condition->get_local_addr() == 0) {
			mov(eax, __("@var_" + condition->get_name()));
		}
		else {
			if (condition->get_local_addr() < 0) {
				mov(eax, __ebp(condition->get_local_addr()));
			}
			else {
				mov(eax, _ebp(condition->get_local_addr()));
			}
		}
		cmp(eax, 0);
	}
	return;
}

void Generator::GenerateInput(VarRecord* p, int& var_number,FunRecord& fun) {
	if (error) return;
	if (p == nullptr) return;
	if (p->get_type() == rev_void) {
		SemanticError(void_non_in);
		return;
	}
	mov(ecx, BUFFER);
	mov(edx, 255);
	mov(ebx, 0);
	mov(eax, 3);
	syscall(128);
	// eax-value bl-char ecx-length
	call(PROC_BUF);
	if (p->get_type() == rev_string) {
		VarRecord buffer;
		string name = BUFFER;
		buffer.Init(rev_string, name);
		buffer.set_value(GLOBAL_STRING);
		buffer_flag = true;
		GenerateAssign(p, &buffer, var_number, fun);
		buffer_flag = false;
	}
	else if (p->get_type() == rev_int) {
		if (p->get_local_addr() == 0) {
			mov(__("@var_" + p->get_name()), eax);
		}
		else {
			if (p->get_local_addr() < 0) {
				mov(__ebp(p->get_local_addr()), eax);
			}
			else {
				mov(_ebp(p->get_local_addr()), eax);
			}
		}
	}
	else {
		if (p->get_local_addr() == 0) {
			mov(__("@var_" + p->get_name()),bl);
		}
		else {
			if (p->get_local_addr() < 0) {
				mov(__ebp(p->get_local_addr()), bl);
			}
			else {
				mov(_ebp(p->get_local_addr()), bl);
			}
		}
	}
}

void Generator::GenerateOutput(VarRecord* p, int& var_number, FunRecord& fun) {
	if (error) return;
	if (p == nullptr) {
		return;
	}
	VarRecord temp;
	string name = "";
	temp.Init(rev_string, name);
	p = GenerateExp(&temp, add, p, var_number, fun);
	mov(ecx, __ebp(p->get_local_addr()));
	mov(edx, 0);
	mov(dl, __(ecx));
	subi(ecx, edx);
	mov(ebx, 1);
	mov(eax, 4);
	syscall(128);
}

bool Generator::GenerateComm() {
	fout = fopen("./common.s", "w");
	if (!fout) {
		cout << "generate common.s failed!" << endl;
		return false;
	}
	else {
		section("section .text");
		// 字符串长度警告输出
		label(STR2LONG);
		sys_write(1, STR2LONG_DATA, STR2LONG_DATA_LEN);
		sys_exit(0);
		// 计算缓冲区的字符个数
		label(PROC_BUF);
		mov(esi, BUFFER);
		mov(edi, 0);
		mov(ecx, 0);
		mov(eax, 0);
		mov(ebx, 10);
		label(CAL_BUF_LEN);
		mov(cl, _(esi,edi));
		cmp(ecx, 10);
		je(CAL_BUF_LEN_EXIT);
		inc(edi);
		multi(ebx);
		addi(eax, ecx);
		subi(eax, 48);
		jmp(CAL_BUF_LEN);
		label(CAL_BUF_LEN_EXIT);
		mov(ecx, edi);
		mov(__(BUFFER_LEN), cl);
		mov(bl, __(esi));
		ret();
		// 
		// main
		section("global _start");
		label(_START);
		call("main");
		sys_exit(0);
		// 
		section("section .data");
		other_ass(STR2LONG_DATA);
		db(" \"Str-Overflow\",10,13");
		other_ass(STR2LONG_DATA_LEN + string(" equ 14\n"));
		other_ass(BUFFER + string(" times 255 db 0\n"));
		other_ass(BUFFER_LEN + string(" db 0\n"));
		other_ass(ESP + string(" dd ") + BASE + "\n");
		other_ass(EBP + string(" dd 0\n"));
		section("section .bss");
		other_ass(STACK + string(" times 65536 db 0\n"));
		label(BASE);
		fclose(fout);
		cout << "generate common.s successfully!" << endl;
		return true;
	}
}

void Generator::subi(string des, string src){
	fprintf(fout, ("\tsub " + des + "," + src + "\n").c_str());
}

void Generator::subi(string des, int src) {
	fprintf(fout, ("\tsub " + des + "," + to_string(src) + "\n").c_str());
}

void Generator::addi(string des, string src){
	fprintf(fout, ("\tadd " + des + "," + src + "\n").c_str());
}

void Generator::addi(string des, int src) {
	fprintf(fout, ("\tadd " + des + "," + to_string(src) + "\n").c_str());
}

void Generator::mov(string des, string src){
	fprintf(fout, ("\tmov " + des + "," + src + "\n").c_str());
}

void Generator::mov(string des, int src) {
	fprintf(fout, ("\tmov " + des + "," + to_string(src) + "\n").c_str());
}

void Generator::cmp(string des, string src){
	fprintf(fout, ("\tcmp " + des + "," + src + "\n").c_str());
}

void Generator::cmp(string des, int src) {
	fprintf(fout, ("\tcmp " + des + "," + to_string(src) + "\n").c_str());
}

void Generator::je(string des){
	fprintf(fout, ("\tje " + des + "\n").c_str());
}

void Generator::jg(string des) {
	fprintf(fout, ("\tjg " + des + "\n").c_str());
}

void Generator::jl(string des) {
	fprintf(fout, ("\tjl " + des + "\n").c_str());
}

void Generator::jle(string des){
	fprintf(fout, ("\tjle " + des + "\n").c_str());
}	

void Generator::jge(string des){
	fprintf(fout, ("\tjge " + des + "\n").c_str());
}	

void Generator::jne(string des){
	fprintf(fout, ("\tjne " + des + "\n").c_str());
}	

void Generator::jna(string des){
	fprintf(fout, ("\tjna " + des + "\n").c_str());
}

void Generator::jmp(string des) {
	fprintf(fout, ("\tjmp " + des + "\n").c_str());
}

void Generator::neg(string des){
	fprintf(fout, ("\tneg " + des + "\n").c_str());
}	

void Generator::dec(string des) {
	fprintf(fout, ("\tdec " + des + "\n").c_str());
}

void Generator::inc(string des){
	fprintf(fout, ("\tinc " + des + "\n").c_str());
}

void Generator::lea(string des,string src) {
	fprintf(fout, ("\tlea " + des + "," + src + "\n").c_str());
}

void Generator::call(string des){	
	fprintf(fout, ("\tcall " + des + "\n").c_str());
}

void Generator::multi(string des){
	fprintf(fout, ("\timul " + des + "\n").c_str());
}

void Generator::idiv(string des){
	fprintf(fout, ("\tidiv " + des + "\n").c_str());
}

void Generator::pop(string reg) {
	fprintf(fout, ("\tpop " + reg + "\n").c_str());
}

void Generator::push(string reg) {
	fprintf(fout, ("\tpush " + reg + "\n").c_str());
}

void Generator::push(int val) {
	fprintf(fout, ("\tpush " + to_string(val) + "\n").c_str());
}

void Generator::label(string lab) {
	fprintf(fout, (lab + ":\n").c_str());
}

void Generator::section(string lab) {
	fprintf(fout, (lab + "\n").c_str());
}

void Generator::other_ass(string content) {
	fprintf(fout, ("\t" + content).c_str());
}

void Generator::other(string content) {
	fprintf(fout, (content).c_str());
}

void Generator::db(string content) {
	fprintf(fout, (" db " + content + "\n").c_str());
}

void Generator::exchg_esp(string reg) {
	mov(reg, __(ESP));
	mov(__(ESP), esp);
	mov(esp, reg);
}

void Generator::syscall(string code) {
	fprintf(fout, ("\tint " + code + "\n").c_str());
}

void Generator::syscall(int code) {
	fprintf(fout, ("\tint " + to_string(code) + "\n").c_str());
}

void Generator::ret() {
	fprintf(fout, "\tret\n");
}

void Generator::sys_exit(int code) {
	mov(ebx, code);
	mov(eax, 1);
	syscall(128);
	ret();
}

void Generator::sys_write(int fd, string buf, int count) {
	mov(edx, count);
	mov(ecx, buf);
	mov(ebx, fd);
	mov(eax, 4);
	syscall(128);
}

void Generator::sys_write(int fd, string buf, string count) {
	mov(edx, count);
	mov(ecx, buf);
	mov(ebx, fd);
	mov(eax, 4);
	syscall(128);
}

void Generator::SemanticError(error_c code) {
	cout << "存在语义错误!" << code << endl;
	exit(1);
	switch (code)
	{
		case void_non_calc:
			break;
		case str_non_add:
			break;
		case void_non_assi:
			break;
		case type_assi:
			break;
		case void_non_in:
			break;
		default:
			break;
	}
}

void Generator::over() {
	if (fout) {
		fclose(fout);
		fout = nullptr;
	}
}