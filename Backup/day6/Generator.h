#pragma once
#include <iostream>
#include <string>
#include <cstdio>
#include "Common.h"
#include "Semantic.h"


static char symbol_data[][ID_LEN]{
	// �գ���ʶ�����쳣������
	"null", "ident", "excep", "number",
	// �ӣ������ˣ�������ֵ
	"add", "sub", "mult", "divi", "assign",
	// >��>=��<��<=��==��!=
	"gt", "ge", "lt", "le", "equ", "nequ",
	// ����>>�����<<
	"input", "output",
	// �����char,string,;,(,),{,}
	"comma", "chara", "strings", "semicon", "lparen", "rparen", "lbrac", "rbrac",
	// ������
	"min",
	"rev_break", "rev_char", "rev_continue", "rev_else", "rev_extern",
	"rev_if", "rev_in", "rev_int", "rev_out",
	"rev_return", "rev_string", "rev_void", "rev_while",
	// end
	"max"
};

using namespace std;


class Generator
{
private:
	static bool buffer_flag;
	static FILE* fout;
	static int id;
public:
	static bool error;
	bool static Init(const char* file_name);
	static string GenerateName(string header, symbol type, string name);
	static VarRecord* GenerateExp(VarRecord* f1, symbol op, VarRecord* f2, int& var_number, FunRecord& fun);
	static VarRecord* GenerateAssign(VarRecord* des, VarRecord* src, int& var_number, FunRecord& fun);
	static void GenerateReturn(VarRecord* ret, int& var_number, FunRecord& fun);
	static void GenerateFunHead(FunRecord& fun);
	static void GenerateFunTail(FunRecord& fun);
	static void GenerateLocalVar(int value);
	static int GenerateBlock(int in, FunRecord& fun);
	static void GenerateInput(VarRecord* p, int& var_number, FunRecord& fun);
	static void GenerateOutput(VarRecord* p, int& var_number, FunRecord& fun);

	static void SemanticError(error_c code);
	static bool GenerateComm();
	// assemble instruction
	static void subi(string des,string src);
	static void subi(string des, int src);
	static void addi(string des, string src);
	static void addi(string des, int src);
	static void mov(string des, string src);
	static void mov(string des, int src);
	static void cmp(string des, string src);
	static void cmp(string des, int src);
	static void je(string des);
	static void jg(string des);
	static void jl(string des);
	static void jle(string des);
	
	static void jge(string des);
	static void jne(string des);
	static void jna(string des);
	static void jmp(string des);
	static void neg(string des);
	static void dec(string des);
	static void inc(string des);
	static void lea(string des,string src);
	static void call(string des);
	static void multi(string des);
	static void idiv(string des);
	static void pop(string reg);
	static void push(string reg);
	static void push(int val);
	static void label(string lab);
	static void section(string lab);
	static void syscall(string code);
	static void syscall(int code);
	static void ret();
	// 
	static void sys_exit(int code);
	static void sys_write(int fd, string buf, int count);
	static void sys_write(int fd, string buf, string count);
	static void other_ass(string content);
	static void db(string content);
	static void exchg_esp(string reg = eax);
};

