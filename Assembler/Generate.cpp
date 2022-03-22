#include <iostream>
#include <string>
#include "Generate.h"



using namespace std;
FILE* Generate::fout = nullptr;
int Generate::total_length = 0;
int Generate::error_number = 0;
int Generate::line_number = 0;
bool Generate::Init(string file_name) {
	fout = fopen(file_name.c_str(), "wb");
	if (fout != nullptr) {
		cout << "file[ " << file_name << "] open successfully!" << endl;
		return true;
	}
	else {
		cout << "file [" << file_name << "] open failed!" << endl;
		return false;
	}
	
}

void Generate::WriteModRM(ModRM modrm,bool scan){
	if (modrm.get_mod() != -1) {
		unsigned char mrm = (unsigned char)(
			((modrm.get_mod() & 3) << 6) +
			((modrm.get_reg() & 7) << 3) +
			(modrm.get_rm() & 7)
			);
		WriteBytes(mrm, 1,scan);
	}
}

void Generate::WriteSIB(SIB sib, bool scan){
	if (sib.get_scale() != -1){
		unsigned char s = (unsigned char)(
			((sib.get_scale() & 3) << 6) +
			((sib.get_index() & 7) << 3) +
			(sib.get_base() & 7)
			);
		WriteBytes(s, 1, scan);
	}
}

void Generate::WriteBytes(int value, int len, bool scan){
	VarRecord::current_addr += len;
	if (!scan) {
		fwrite(&value, len, 1, fout);
		fflush(fout);
	}
	return;
}

void Generate::WriteBytes(int value, int len) {
	fwrite(&value, len, 1, fout);
	fflush(fout);
}

void Generate::WriteBytes(const void* buffer, size_t len) {
	fwrite(buffer, len, 1, fout);
	fflush(fout);
}

bool Generate::HandleRelocation(int type, bool scan, VarRecord** rel,string cur_seg) {
	if (scan || (*rel) == nullptr) {
		(*rel) = nullptr;
		return false;
	}
	bool flag = false;
	if (type == R_386_32) {
		if (!(*rel)->get_is_equ()) {
			//obj
			Elf_File::AddReloc(cur_seg, VarRecord::current_addr, (*rel)->get_sym_name(), type);
			flag = true;
		}
	}
	else if (type == R_386_PC32) {
		if ((*rel)->get_is_externed()) {
			// obj
			Elf_File::AddReloc(cur_seg, VarRecord::current_addr, (*rel)->get_sym_name(), type);
			flag = true;
		}
	}
	(*rel) = nullptr;
	return flag;
}

void Generate::Generate2Op(symbol op, int des, int src, int len, Inst instructure, bool scan, VarRecord** rel, string cur_seg) {
	int index = -1;
	if (des == IMMD) {
		Error(immd_non_des);
	}
	else if (des == MEMO && src == MEMO) {
		Error(MEMO_non_both);
	}

	if (src == IMMD) index = 3;
	else index = (des - 2) * 2 + (src - 2);
	index = (op - rev_mov) * 8 + (1 - len % 4) * 4 + index;
	unsigned char op_code = op2code[index];
	unsigned char ex_char;
	switch (instructure.mod_rm.get_mod()) {
		case -1: {
			switch (op) {
				case rev_mov: {
					op_code += (unsigned char)(instructure.mod_rm.get_reg());
					WriteBytes(op_code, 1,scan);
					break;
				}
				case rev_cmp: {
					WriteBytes(op_code,1,scan);
					ex_char = 0xf8;
					ex_char += (unsigned char)(instructure.mod_rm.get_reg());
					WriteBytes(ex_char, 1, scan);
					break;
				}
				case rev_add: {
					WriteBytes(op_code, 1, scan);
					ex_char = 0xc0;
					ex_char += (unsigned char)(instructure.mod_rm.get_reg());
					WriteBytes(ex_char, 1, scan);
					break;
				}
				case rev_sub: {
					WriteBytes(op_code, 1, scan);
					ex_char = 0xe8;
					ex_char += (unsigned char)(instructure.mod_rm.get_reg());
					WriteBytes(ex_char, 1, scan);
					break;
				}
			}
			HandleRelocation(R_386_32, scan, rel, cur_seg);
			WriteBytes(instructure.get_imm32(), len, scan);
			break;
		}
		case 0: {
			WriteBytes(op_code, 1, scan);
			WriteModRM(instructure.mod_rm, scan);
			if (instructure.mod_rm.get_rm() == 5) {
				HandleRelocation(R_386_32,scan,rel,cur_seg);
				instructure.WriteDisp(scan);
			}
			else if (instructure.mod_rm.get_rm() == 4) {
				WriteSIB(instructure.sib, scan);
			}
			break;
		}
		case 1: {
			WriteBytes(op_code, 1, scan);
			WriteModRM(instructure.mod_rm, scan);
			if (instructure.mod_rm.get_rm() == 4)
				WriteSIB(instructure.sib, scan);
			instructure.WriteDisp(scan);
			break;
		}
		case 2: {
			WriteBytes(op_code, 1, scan);
			WriteModRM(instructure.mod_rm, scan);
			if (instructure.mod_rm.get_rm() == 4)
				WriteSIB(instructure.sib,scan);
			instructure.WriteDisp(scan);
			break;
		}
		case 3: {
			WriteBytes(op_code, 1, scan);
			WriteModRM(instructure.mod_rm, scan);
			break;
		}
	}
}

void Generate::Generate1Op(symbol op, int op_type, int len,  Inst instructure, bool scan, VarRecord** relo, string cur_seg) {
	unsigned char ex_char;
	unsigned short int op_code = op1code[op - rev_call];
	if (op == rev_call || op >= rev_jmp && op <= rev_jna) {
		if (op == rev_call || op == rev_jmp) {
			WriteBytes(op_code, 1, scan);
		}
		else {
			WriteBytes((op_code >> 8), 1, scan);
			WriteBytes(op_code, 1, scan);
		}
		int rel = instructure.get_imm32() - (VarRecord::current_addr + 4);
		bool ret_flag = HandleRelocation(R_386_PC32, scan, relo,cur_seg);
		if (ret_flag) rel = -4;
		WriteBytes(rel, 4,scan);
		return;
	}
	switch (op) {
		case rev_sys_int: {
			WriteBytes(op_code, 1, scan);
			WriteBytes(instructure.get_imm32(), 1, scan);
			break;
		}
		case rev_push: {
			if (op_type == IMMD) {
				op_code = 0x68;
				WriteBytes(op_code, 1, scan);
				WriteBytes(instructure.get_imm32(), 4, scan);
			}
			else
			{
				op_code += (unsigned char)(instructure.mod_rm.get_reg());
				WriteBytes(op_code, 1, scan);
			}
			break;
		}
		case rev_inc: {
			if (len == 1) {
				op_code = 0xfe;
				WriteBytes(op_code, 1, scan);
				ex_char = 0xc0;
				ex_char += (unsigned char)(instructure.mod_rm.get_reg());
				WriteBytes(ex_char, 1, scan);
			}
			else {
				op_code += (unsigned char)(instructure.mod_rm.get_reg());
				WriteBytes(op_code, 1, scan);
			}
			break;
		}
		case rev_dec: {
			if (len == 1) {
				op_code = 0xfe;
				WriteBytes(op_code, 1, scan);
				ex_char = 0xc8;
				ex_char += (unsigned char)(instructure.mod_rm.get_reg());
				WriteBytes(ex_char, 1, scan);
			}
			else {
				op_code += (unsigned char)(instructure.mod_rm.get_reg());
				WriteBytes(op_code, 1, scan);
			}
			break;
		}
		case rev_neg: {
			if (len == 1) {
				op_code = 0xf6;
			}
			ex_char = 0xd8;
			ex_char += (unsigned char)(instructure.mod_rm.get_reg());
			WriteBytes(op_code, 1, scan);
			WriteBytes(ex_char, 1, scan);
			break;
		}
		case rev_pop: {
			op_code += (unsigned char)(instructure.mod_rm.get_reg());
			WriteBytes(op_code, 1, scan);
			break;
		}
		case rev_imul: {
			WriteBytes(op_code, 1, scan);
			ex_char = 0xe8;
			ex_char += (unsigned char)(instructure.mod_rm.get_reg());
			WriteBytes(ex_char, 1, scan);
			break;
		}
		case rev_idiv: {
			WriteBytes(op_code, 1, scan);
			ex_char = 0xf8;
			ex_char += (unsigned char)(instructure.mod_rm.get_reg());
			WriteBytes(ex_char, 1, scan);
			break;
		}
	}
}

void Generate::Generate0Op(symbol op,bool scan) {
	unsigned char op_code = op0code[0];
	WriteBytes(op_code, 1, scan);
}

void Generate::Over() {
	if (fout) {
		fclose(fout);
		fout = nullptr;
	}
}

void Generate::Error(error_c code) {
	error_number++;
	cout << "error at line [" << line_number << "] ";
	switch (code) {
		case ident_lost:{
			cout << "ident may lost!" << endl;
			break;
		}
		case times_wrong: {
			cout << "`times` can only with number!" << endl;
			break;
		}
		case equ_wrong: {
			cout << "`equ` can only with number!" << endl;
			break;
		}
		case len_type_wrong: {
			cout << "type length only can be 1,2,4 (Byte)!" << endl;
			break;
		}
		case type_wrong: {
			cout << "type only can be db,dd,dw!" << endl;
		}
		case comma_lost: {
			cout << "comma(,) maybe lost!" << endl;
			break;
		}
		case instruction_wrong: {
			cout << "unrecognizied instruction!" << endl;
			break;
		}
		case subs_non_number: {
			cout << "sub(-) can only with number!" << endl;
			break;
		}
		case regs_wrong: {
			cout << "reister wrong! not an effective regs!" << endl;
			break;
		}
		case rbrac_lost: {
			cout << "rbrac(]) maybe lost!" << endl;
			break;
		}
		case lbrac_lost: {
			cout << "lbrac([) maybe lost!" << endl;
			break;
		}
	}
}