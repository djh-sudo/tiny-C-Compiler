#pragma once
#include <unordered_map>
#include <iostream>
#include <vector>
#include <string>
#include "Elf_File.h"

using namespace std;

class VarRecord {
private:
	string seg_name;
	string sym_name;
	bool is_equ;
	bool is_externed;
	int time;
	int addr;
	int len_type;
	int* sym_array;
	int sym_length;
public:
	static int current_addr;
	VarRecord(string name,bool externed,string cur_seg_name);
	VarRecord(string name, int addr, string cur_seg_name);
	VarRecord(string name, int time, int len_type, int content[], int content_len, string cur_seg_name);
	~VarRecord();
	// default API
	int get_current_addr();
	string get_seg_name();
	string get_sym_name();
	bool get_is_equ();
	bool get_is_externed();
	int get_times();
	int get_addr();
	int get_len_type();
	int get_sym_length();
};

class Table {
private:
	static unordered_map<string, VarRecord*>var_map;
	static vector<VarRecord*>def_labs;
	static bool scan_first;
public:
	static bool HasName(string name);
	static void AddVar(VarRecord* var);
	static VarRecord* GetVar(string name, string cur_seg_name);
	static void SwtichSeg(int& data_len, string& cur_seg_name, string seg_name);
	// default API
	static void set_scan_first(bool scan);
	static bool get_scan_first();
};

class ModRM {
private:
	int mod;
	int reg;
	int rm;
public:
	ModRM();
	void Init();
	// default API
	int get_mod();
	int get_reg();
	int get_rm();
	void set_mod(int mod);
	void set_reg(int reg);
	void set_rm(int rm);
};

class SIB {
private:
	int scale;
	int index;
	int base;

public:
	SIB();
	void Init();
	// default API
	int get_scale();
	int get_index();
	int get_base();
	void set_scale(int scale);
	void set_index(int index);
	void set_base(int base);
};

class Inst {
private:
	unsigned char opr;
	int disp;
	int imm32;
	int disp_len;
public:
	ModRM mod_rm;
	SIB sib;
	Inst();
	void Init();
	void SetDisp(int disp, int len);
	void WriteDisp();
	// default API
	int get_imm32();
	int get_disp();
	int get_disp_len();
	unsigned char get_opr();
	void set_opr(unsigned char opr);
	void set_imm32(int imm32);
};