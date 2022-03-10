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
		return_name += "_" + (id);
	}
	return return_name;
}

VarRecord* Generator::GenerateExp(VarRecord* f1, symbol op, VarRecord* f2, int& var_number) {
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
	string lab_loop, lab_exit;
	switch (ret_type) {
		case rev_string: {
			if (f2->get_type() == rev_string) {
				lab_loop = GenerateName("lab", null, "cpystr2");
				lab_exit = GenerateName("lab", null, "cpystr2_exit");

			}
			break;
		}
	{
	default:
		break;
	}
	}
}


VarRecord* Generator::GenerateAssign(VarRecord* des, VarRecord* src, int& var_number) {
	return nullptr;
}

void Generator::GenerateReturn(VarRecord* ret, int& var_number) {
	
}

void Generator::GenerateFunHead(){
	
}

void Generator::GenerateFunTail(){
	
}

void Generator::GenerateLocalVar(int value) {
	
}

void Generator::GenerateBlock(int n) {
	
}

void Generator::GenerateInput(VarRecord* p, int& var_number) {
	
}

void Generator::GenerateOutput(VarRecord* p, int& var_number) {
	
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
		label("@str2long");
		sys_write(1, STR2LONG_DATA, STR2LONG_DATA_LEN);
		sys_exit(0);
		// 计算缓冲区的字符个数
		label("@proc_buf");
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
		sub(eax, 48);
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
		other_ass(STR2LONG_DATA_LEN + string(" equ 26\n"));
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

void Generator::sub(string des, string src){
	fprintf(fout, ("\tsub " + des + "," + src + "\n").c_str());
}

void Generator::sub(string des, int src) {
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
	fprintf(fout, ("\dec " + des + "\n").c_str());
}

void Generator::inc(string des){
	fprintf(fout, ("\inc " + des + "\n").c_str());
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

void Generator::label(string lab) {
	fprintf(fout, (lab + ":\n").c_str());
}

void Generator::section(string lab) {
	fprintf(fout, (lab + "\n").c_str());
}

void Generator::other_ass(string content) {
	fprintf(fout, ("\t" + content).c_str());
}

void Generator::db(string content) {
	fprintf(fout, (" db " + content + "\n").c_str());
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
	
}