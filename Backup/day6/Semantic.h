#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Common.h"

using namespace std;


class VarRecord {
private:
	symbol type; // ����
	string name; // ����
	int value;   // ֵ
	int local_addr;// �ֲ��������ebpָ��ĵ�ַ
	bool externed;// �ⲿ����

public:
	VarRecord();
	VarRecord(const VarRecord& src);
	~VarRecord() = default;
	void Init(symbol dec_type,string name);
	void Copy(const VarRecord* src);
	
	// get and set API
	symbol get_type();
	string get_name();
	int get_value();
	int get_local_addr();
	bool get_externed();

	void set_type(symbol type);
	void set_name(string name);
	void set_value(int value);
	void set_local_addr(int addr);
	void set_externed(bool externed);

};


class FunRecord {
private:
	symbol type;
	string name;
	vector<symbol>* arguments;// �������������б�
	vector<VarRecord*>* local_vars;// �����ֲ������б�
	bool defined;
	bool flushed;
	bool has_return;

public:
	FunRecord();
	FunRecord(const FunRecord& src);
	~FunRecord();
	void Init(symbol dec_type,string name);
	void AddArgs(VarRecord var);
	bool HasName(string name);
	void PushLocalVar(VarRecord var);
	int GetCurrentAddr();
	void FlushArgs();
	void PopLocalVars(int var_number);
	bool equal(FunRecord& function);
	VarRecord* CreateTmpVar(symbol type, bool has_val, int& var_number, int num = -1);
	
	// get and set API
	symbol get_type();
	string get_name();
	bool get_defined();
	bool get_flushed();
	bool get_has_return();

	void set_defined(bool defined);
	void set_flushed(bool flushed);
	void set_has_return(bool has_return);

};


class VarTable {
private:
	unordered_map<string, VarRecord*>var_map;
	unordered_map<string, FunRecord*>fun_map;
	vector<string*>string_table; // �ַ����ռ�
	vector<VarRecord*>parameter; // ʵ���б�
public:

};
