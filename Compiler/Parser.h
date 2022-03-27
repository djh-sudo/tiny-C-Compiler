#pragma once
#include <iostream>
#include <string>
#include "Semantic.h"
#include "Lexer.h"


class Parser
{
private:
	VarRecord var;         // Variable
	FunRecord fun;         // function
	vector<int>break_level;// break in for or while?
	vector<int>case_number;// case number
	Lexer lexer;           // lexer
	bool wait;             // skip read next token
	symbol old_token;
	symbol token;
	bool compiler_ok;
	int syntax_error;

	int while_id;
	int if_id;
	int for_id;
	int switch_id;
	bool ident_only;
	bool has_default;
	string file_name;

public:
	Parser();
	~Parser();
	void end();
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
	void WhileState(int& level);
	void IfState(int& level, int loop_id, int addr);
	
	void ForState(int& level);
	void ForInit(int& var_number, int& level);
	void ForCondition(int& var_number, int& level, int addr_end);
	void ForEnd(int& var_number, int& level, int loop_id, int addr, int addr_end);

	void SwitchState(int& level);
	void CaseState(int& level, int& init_number, int addr, VarRecord* var);

	void CaseHandle(VarRecord* var);
	void ReturnState(int& var_number, int& level);
	void ReturnTail(int& var_number, int& level);
	VarRecord* IdentTail(string name, int& var_number);
	void RealArgs(string name, int& var_number);
	void RealArgsList(int& var_number);
	
	VarRecord* OrExpr(int& var_number);
	VarRecord* OrExprTail(VarRecord* factor, int& var_number);
	VarRecord* Expr(int& var_number);
	VarRecord* ExprTail(VarRecord* factor, int& var_number);
	VarRecord* OneExpr(int& var_number);
	VarRecord* Item(int& var_number);
	VarRecord* ItemTail(VarRecord* factor, int& var_number);
	VarRecord* Factor(int& var_number);
	VarRecord* FactorTail(VarRecord* factor, int& var_number);

	void SyntaxError(error_c error_code);
};

