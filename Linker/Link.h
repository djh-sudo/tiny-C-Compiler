#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "Elf_File.h"


using namespace std;
class Block {
private:
	char* data;
	int offset;
	int block_size;
public:
	Block(char* data, int offset, int size);
	~Block();
	int get_offset();
	int get_block_size();
	char* get_data();
};

class SegList {
private:
	int base_addr;
	int offset;
	int size;
	int begin;
public:
	vector<Elf_File*>file_list;
	vector<Block*>blocks;
public:
	~SegList();
	SegList();
	void AllocAddr(string name, int& base, int& offset);
	void RelocAddr(int rel_addr, int type, int sym_addr);
	// default API
	int get_base_addr();
	int get_offset();
	int get_size();
	int get_begin();
};

class SymLink {
public:
	string name;
	Elf_File* recv;
	Elf_File* prov;
};


class Link
{
private:
	vector<string>seg_name;
	Elf_File obj;
	Elf_File* start;
public:
	vector<Elf_File*>files;
	unordered_map < string, SegList*>seg_list;
	vector<SymLink*>all_sym;
	vector<SymLink*>sym_def;
public:
	Link();
	void AddElf(const char* file_name);
	void CollectInfo();
	bool CheckSymIsValid();
	void AllocAddr();
	void SymParser();
	void Relocation();
	void AssemblyObj();
	void ExportElf(const char*file_name);
	bool Excute(const char* file_name);
};

