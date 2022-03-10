#pragma once
#include <iostream>
#include <string>
#include "Semantic.h"
#include "Lexer.h"


class Parser
{
private:
	VarRecord var;
	FunRecord fun;
	Lexer lexer;
	bool wait; // 是否读入下一个token
	symbol old_token;
	symbol token;
	bool compiler_ok;
	int syntax_error;
	int while_id;
	int if_id;


public:
	Parser();
	~Parser() = default;
	void Init(const char* file_name);
	bool NextToken();
	bool Match(symbol s);
	void Program();
	void Dec();
	symbol Type();
	void DecTail(symbol type,string name);
	void FunTail(symbol type,string name);
	void VarList(symbol type);
	void Parameter();
	void ParaList();
	void Block(int init_number,int&level,int loop_id,int addr);
	void SubProgram(int& var_number, int& level, int loop_id, int addr);
	void LocalDec(int& var_number, int& level);
	void LocalDecTail(int& var_number, symbol type, int& level);
	void Statement(int& var_number, int& level, int loop_id, int addr);
	void WhileState(int& var_number, int& level);
	void IfState(int& var_number, int& level, int loop_id, int addr);
	void ReturnState(int& var_number, int& level);
	void ReturnTail(int& var_number, int& level);
	VarRecord* IdentTail(string name, int& var_number);
	void RealArgs(string name, int& var_number);
	void RealArgsList(int& var_number);
	VarRecord* Expr(int& var_number);
	VarRecord* ExprTail(VarRecord* factor, int& var_number);
	VarRecord* OneExpr(int& var_number);
	VarRecord* Item(int& var_number);
	VarRecord* ItemTail(VarRecord* factor, int& var_number);
	VarRecord* Factor(int& var_number);
	VarRecord* FactorTail(VarRecord* factor, int& var_number);

	void SyntaxError(error_c error_code);
};

