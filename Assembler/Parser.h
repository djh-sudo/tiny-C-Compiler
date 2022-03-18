#pragma once
#include <iostream>
#include <string>
#include "Common.h"
#include "Semantic.h"
#include "Lexer.h"

using namespace std;

class Parser
{
private:
	bool wait;
	enum symbol token;
	string cur_seg;
	int data_length;
	bool scan_first;
	VarRecord* rel_var;
	Inst instructure;
	Lexer lexer;
public:
	// Parser();
	void Init();
	bool NextToken();
	bool Match(symbol t);
	void Program();
	void IdentTail(string name);
	void BaeseTail(string name,int num);
	int Len();
	void Value(string name, int time, int len);
	void Type(int content[], int& content_len, int len);
	void ValTail(int content[], int& content_len, int len);
	void Instruct();
	void Operate(int& reg_num, int& type, int& len);
	int Register();
	void Memory();
	void Addr();
	void RegAddr(symbol base_reg, const int type);
	void RegAddrTail(symbol base_reg, const int type, symbol sign);
};

