#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "elf.h"
#include "Semantic.h"


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
};


class Elf_File
{
private:
	Elf32_Ehdr ehdr;
	unordered_map<string, Elf32_Shdr*>shdr_tab;
	vector<string>shdr_name;
	unordered_map<string, Elf32_Sym*>sym_tab;
	vector<string>sym_name;
	vector<Reloc*>rel_tab;

	char* sh_str_tab;
	int shstrtab_size;
	char* str_tab;
	int strtab_size;
	vector<Elf32_Rel*>rel_text_tab;
	vector<Elf32_Rel*>rel_data_tab;

public:
	Elf_File();
	int GetSegIndex(string seg_name);
	int GetSymIndex(string sym_name);
	void AddShdr(string name, int size, int data_length);
	void AddShdr(string sh_name, Elf32_Word sh_type, Elf32_Word sh_flag,
			     Elf32_Addr sh_addr, Elf32_Off sh_offset, Elf32_Word sh_size,
			     Elf32_Word sh_link, Elf32_Word sh_info, Elf32_Word sh_addr_align,
			     Elf32_Word sh_enter_size);
	void AddSym(VarRecord* var);
	void AddSym(string name, Elf32_Sym*);
	Reloc* AddReloc(string seg, int addr, string name, int type);

};

