#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Semantic.h"
#include "Generate.h"
#include "elf.h"

class VarRecord;


using namespace std;

class Reloc {
private:
	string target_seg;
	int offset;
	string name;
	int type;
public:
	Reloc();
	Reloc(string seg, int addr, string name, int type);

	string get_target_seg();
	int get_offset();
	string get_name();
	int get_type();

	void set_target_seg(string seg);
	void set_offset(int offset);
	void set_name(string name);
	void set_type(int type);
};


class Elf_File
{
public:
	static Elf32_Ehdr ehdr;
	static unordered_map<string, Elf32_Shdr*>shdr_tab;
	static vector<string>shdr_name;
	static unordered_map<string, Elf32_Sym*>sym_tab;
	static vector<string>sym_name;
	static vector<Reloc*>rel_tab;

	static char* sh_str_tab;
	static int shstrtab_size;
	static char* str_tab;
	static int strtab_size;
	static vector<Elf32_Rel*>rel_text_tab;
	static vector<Elf32_Rel*>rel_data_tab;
	static FILE* fin;
public:
	static void Init();
	static int GetSegIndex(string seg_name);
	static int GetSymIndex(string sym_name);
	static void AddShdr(string name, int size, int data_length);
	static void AddShdr(string sh_name, Elf32_Word sh_type, Elf32_Word sh_flag,
			     Elf32_Addr sh_addr, Elf32_Off sh_offset, Elf32_Word sh_size,
			     Elf32_Word sh_link, Elf32_Word sh_info, Elf32_Word sh_addr_align,
			     Elf32_Word sh_enter_size);
	static void AddSym(VarRecord* var);
	static void AddSym(string name, Elf32_Sym*);
	static Reloc* AddReloc(string seg, int addr, string name, int type);
	static void PaddingSeg(string prev,string next);
	static void AssembleObj(int data_length);
	static void WriteElfTail();
	static void WriteElf(string name, int data_length, bool scan);

};

