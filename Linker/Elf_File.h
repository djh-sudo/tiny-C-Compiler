#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "elf.h"


using namespace std;

class RelItem {
private:
	string seg_name;
	string rel_name;
public:
	Elf32_Rel* rel;
public:
	RelItem();
	RelItem(string seg_name, Elf32_Rel* r, string rel_name);
	~RelItem();
	// default API
	string get_seg_name();
	string get_rel_name();
};


class Elf_File
{
public:
	Elf32_Ehdr ehdr;
	vector<Elf32_Phdr*>phdr_tab;
	unordered_map<string, Elf32_Shdr*>shdr_tab;
	vector<string>shdr_names;
	unordered_map<string, Elf32_Sym*>sym_tab;
	vector<string>sym_names;
	vector<RelItem*>rel_tab;
	string file_name;
	char* shstr_tab;
	int shstr_tab_size;
	char* str_tab;
	int str_tab_size;
public:
	Elf_File();
	void ReadElf(const char* file_name);
	void GetData(char*buffer,Elf32_Off offset,Elf32_Word size);
	int GetSegIndex(string seg_name);
	int GetSymIndex(string sym_name);
	void AddPhdr(Elf32_Word type, Elf32_Off offset, Elf32_Addr vir_addr,
		Elf32_Word file_size, Elf32_Word mem_size, Elf32_Word flags, Elf32_Word align);
	void AddShdr(string sh_name, Elf32_Word sh_type, Elf32_Word sh_flags,
		Elf32_Addr sh_addr, Elf32_Off sh_offset, Elf32_Word sh_size, Elf32_Word sh_link,
		Elf32_Word sh_info, Elf32_Word sh_align, Elf32_Word sh_entsize);

	void AddSym(string name, Elf32_Sym* s);
	void Write(const char* output, bool flag);
};

