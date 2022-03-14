#pragma once
#include <iostream>
#include <string>
#include <cstdio>
#include "Common.h"
#include "Semantic.h"


static char symbol_data[][ID_LEN]{
	// 空，标识符，异常，数字
	"null", "ident", "excep", "number",
	// 加，减，乘，除，赋值
	"add", "sub", "mult", "divi", "assign",
	// >，>=，<，<=，==，!=
	"gt", "ge", "lt", "le", "equ", "nequ",
	// 输入>>，输出<<
	"input", "output",
	// 界符，char,string,;,(,),{,}
	"comma", "chara", "strings", "semicon", "lparen", "rparen", "lbrac", "rbrac",
	// 保留字
	"min",
	"break", "char", "continue", "else", "extern",
	"if", "in", "int", "out",
	"return", "string", "void", "while",
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
	static int line_number;
public:
	static void over();
	bool static Init(const char* file_name);
	static string GenerateName(string header, symbol type, string name);
	static VarRecord* GenerateExp(VarRecord* f1, symbol op, VarRecord* f2, int& var_number, FunRecord& fun);
	static VarRecord* GenerateAssign(VarRecord* des, VarRecord* src, int& var_number, FunRecord& fun);
	static void GenerateReturn(VarRecord* ret, int& var_number, FunRecord& fun);
	static void GenerateFunHead(FunRecord& fun);
	static void GenerateFunTail(FunRecord& fun);
	static void GenerateLocalVar(int value);
	static void GenerateCondition(VarRecord* condition);
	static int GenerateBlock(int in, FunRecord& fun);
	static void GenerateInput(VarRecord* p, int& var_number, FunRecord& fun);
	static void GenerateOutput(VarRecord* p, int& var_number, FunRecord& fun);

	static void SemanticError(error_c code, string info = "");
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
	static void other_ass(string content);
	static void other(string content);
	static void db(string content);
	
	// 
	static void sys_exit(int code);
	static void sys_write(int fd, string buf, int count);
	static void sys_write(int fd, string buf, string count);
	static void exchg_esp(string reg = eax);
};

