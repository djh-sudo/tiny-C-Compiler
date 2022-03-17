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
	int times;
	int addr;
	int len_type;
	int* sym_array;
	int sym_length;
public:
	static int current_addr;
	VarRecord(string name,bool externed,string cur_seg_name);
	VarRecord(string name, int addr, string cur_seg_name);
	VarRecord(string name, int times, int len_type, int content[], int content_len, string cur_seg_name);
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


class Semantic
{
};

