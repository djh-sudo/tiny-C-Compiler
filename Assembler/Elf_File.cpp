#include "Elf_File.h"
#include "Common.h"


Reloc::Reloc() {
	this->name = "";
	this->offset = 0;
	this->target_seg = "";
	this->type = 0;
}

Reloc::Reloc(string seg, int addr, string name, int type) {
	this->target_seg = seg;
	this->offset = addr;
	this->name = name;
	this->type = type;
}

string Reloc::get_target_seg() {
	return target_seg;
}

int Reloc::get_offset() {
	return offset;
}

string Reloc::get_name() {
	return name;
}

int Reloc::get_type() {
	return type;
}

void Reloc::set_target_seg(string seg) {
	this->target_seg = seg;
}

void Reloc::set_offset(int offset) {
	this->offset = offset;
}

void Reloc::set_name(string name) {
	this->name = name;
}

void Reloc::set_type(int type) {
	this->type = type;
}

Elf32_Ehdr Elf_File::ehdr;
unordered_map<string, Elf32_Shdr*>Elf_File::shdr_tab;
vector<string>Elf_File::shdr_name;
unordered_map<string, Elf32_Sym*>Elf_File::sym_tab;
vector<string>Elf_File::sym_name;
vector<Reloc*>Elf_File::rel_tab;

char* Elf_File::sh_str_tab = nullptr;
int Elf_File::shstrtab_size = 0;
char* Elf_File::str_tab = nullptr;
int Elf_File::strtab_size = 0;
vector<Elf32_Rel*>Elf_File::rel_text_tab;
vector<Elf32_Rel*>Elf_File::rel_data_tab;
FILE* Elf_File::fin = nullptr;


void Elf_File::Init() {
	sh_str_tab = nullptr;
	str_tab = nullptr;
	fin = nullptr;
	shstrtab_size = 0;
	strtab_size = 0;
	shdr_tab.clear();
	shdr_name.clear();
	rel_tab.clear();
	sym_name.clear();
	rel_data_tab.clear();
	rel_text_tab.clear();
	AddShdr("", 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddSym("", nullptr);
}

int Elf_File::GetSegIndex(string name) {
	int index = 0;
	for (int i = 0; i < shdr_name.size(); i++) {
		if (shdr_name[i] == name) {
			break;
		}
		index++;
	}
	return index;
}

int Elf_File::GetSymIndex(string name) {
	int index = 0;
	for (int i = 0; i < sym_name.size(); i++) {
		if (sym_name[i] == name) {
			break;
		}
		index++;
	}
	return index;
}

void Elf_File::AddShdr(string name, int size,int data_length) {
	int offset = 52 + data_length;
	if (name == ".text") {
		AddShdr(name, SHT_PROGBITS, SHF_ALLOC | SHF_EXECINSTR, 0, offset, size, 0, 0, 4, 0);
	}
	else if (name == ".data") {
		AddShdr(name, SHT_PROGBITS, SHF_ALLOC | SHF_WRITE, 0, offset, size, 0, 0, 4, 0);
	}
	else if (name == ".bss") {
		AddShdr(name, SHT_NOBITS, SHF_ALLOC | SHF_WRITE, 0, offset, size, 0, 0, 4, 0);
	}
	return;
}

void Elf_File::AddShdr(string sh_name, Elf32_Word sh_type, Elf32_Word sh_flag,
	         Elf32_Addr sh_addr, Elf32_Off sh_offset, Elf32_Word sh_size,
	         Elf32_Word sh_link, Elf32_Word sh_info, Elf32_Word sh_addr_align,
	         Elf32_Word sh_enter_size) {
	Elf32_Shdr* sh = new Elf32_Shdr();
	sh->sh_name = 0;
	sh->sh_type = sh_type;
	sh->sh_flags = sh_flag;
	sh->sh_addr = sh_addr;
	sh->sh_offset = sh_offset;
	sh->sh_size = sh_size;
	sh->sh_link = sh_link;
	sh->sh_info = sh_info;
	sh->sh_addralign = sh_addr_align;
	sh->sh_entsize = sh_enter_size;
	shdr_tab[sh_name] = sh;
	shdr_name.push_back(sh_name);
}


void Elf_File::AddSym(string name, Elf32_Sym* s) {
	sym_tab[name] = new Elf32_Sym();
	Elf32_Sym* sym = sym_tab[name];
	if (name == "") {
		sym->st_name = 0;
		sym->st_value = 0;
		sym->st_size = 0;
		sym->st_info = 0;
		sym->st_other = 0;
		sym->st_shndx = 0;
	}
	else {
		sym->st_name = 0;
		sym->st_value = s->st_value;
		sym->st_size = s->st_size;
		sym->st_info = s->st_info;
		sym->st_other = s->st_other;
		sym->st_shndx = s->st_shndx;
	}
	sym_name.push_back(name);
}

void Elf_File::AddSym(VarRecord* var) {
	bool is_global = false;
	string name = var->get_sym_name();
	if (name.find(LAB_HEAD) == 0 || name.find(IF_HEAD) == 0 ||
		name.find(WHILE_HEAD) == 0 || name.find(FOR_HEAD) == 0 ||
		name.find(CAL_HEAD) == 0 || name.find(STACK_HEAD) == 0) {
		return;
	}
	if (var->get_seg_name() == TEXT_SEG) {
		if (name == STR2LONG || name == PROC_BUF) {
			is_global = true;
		}
		else if (name[0] != '@') {
			is_global = true;
		}
	}
	else if (var->get_seg_name() == DATA_SEG) {
		int index = name.find("@str_");
		if (index == 0 && name.size() >= 5) {
			is_global = !(name[5] >= '0' && name[5] <= '9');
		}
		else {
			is_global = true;
		}
	}
	else if (var->get_seg_name() == "") {
		is_global = var->get_is_externed();
	}

	Elf32_Sym* sym = new Elf32_Sym();
	sym->st_name = 0;
	sym->st_value = var->get_addr();
	sym->st_size = var->get_times() * var->get_len_type() * var->get_sym_length();
	if (is_global)
		sym->st_info = ELF32_ST_INFO(STB_GLOBAL, STT_NOTYPE);
	else
		sym->st_info = ELF32_ST_INFO(STB_LOCAL, STT_NOTYPE);
	sym->st_other = 0;
	if (var->get_is_externed())
		sym->st_shndx = STN_UNDEF;
	else
		sym->st_shndx = GetSegIndex(var->get_seg_name());
	AddSym(var->get_sym_name(), sym);
	return;
}

Reloc* Elf_File::AddReloc(string seg, int addr, string name, int type) {
	Reloc* rel = new Reloc(seg, addr, name, type);
	rel_tab.push_back(rel);
	return rel;
}

void Elf_File::PaddingSeg(string prev, string next) {
	char padding = 0;
	int padding_number = shdr_tab[next]->sh_offset -
		(shdr_tab[prev]->sh_offset + shdr_tab[prev]->sh_size);
	while (padding_number--) {
		Generate::WriteBytes(padding, 1);
	}
}

void Elf_File::AssembleObj(int data_length) {
	int* pointer = (int*)ehdr.e_ident;
	*pointer = 0x464c457f;
	pointer++;
	*pointer = 0x00010101;
	pointer++;
	*pointer = 0x00;
	pointer++;
	*pointer = 0x00;
	ehdr.e_type = ET_REL;
	ehdr.e_machine = EM_386;
	ehdr.e_version = EV_CURRENT;
	ehdr.e_entry = 0;
	ehdr.e_phoff = 0;
	ehdr.e_flags = 0;
	ehdr.e_ehsize = 52;
	ehdr.e_phentsize = 0;
	ehdr.e_phnum = 0;
	ehdr.e_shentsize = 40;
	ehdr.e_shnum = 9;
	ehdr.e_shstrndx = 4;
	// 
	int cur_offset = 52 + data_length;
	shstrtab_size = 51;
		/* (string(REL_TEXT_SEG) + REL_DATA_SEG + BSS_SEG
			+ SH_STR_SEG + SYM_SEG + STR_SEG).size + 5; */
	sh_str_tab = new char[shstrtab_size];
	char* str = sh_str_tab;
	int index = 0;
	unordered_map<string, int>shstr_index;
	
	shstr_index[REL_TEXT_SEG] = index;
	strcpy(str + index, REL_TEXT_SEG);

	shstr_index[TEXT_SEG] = index + 4;
	index += 10;
	shstr_index[""] = index - 1;
	
	shstr_index[REL_DATA_SEG] = index;
	strcpy(str + index, REL_DATA_SEG);

	shstr_index[DATA_SEG] = index + 4;
	index += 10;

	shstr_index[BSS_SEG] = index;
	strcpy(str + index, BSS_SEG);
	index += 5;

	shstr_index[SH_STR_SEG] = index;
	strcpy(str + index, SH_STR_SEG);
	index += 10;

	shstr_index[SYM_SEG] = index;
	strcpy(str + index, SYM_SEG);
	index += 8;

	shstr_index[STR_SEG] = index;
	strcpy(str + index, STR_SEG);
	index += 8;

	AddShdr(SH_STR_SEG,       /* string sh_name */ 
		    SHT_STRTAB, 	  /* Elf32_Word sh_type */
		    0,				  /* Elf32_Word sh_flag */
		    0,				  /* Elf32_Addr sh_addr */
		    cur_offset,		  /* Elf32_Off sh_offset */
		    shstrtab_size,	  /* Elf32_Word sh_size */
		    SHN_UNDEF,		  /* Elf32_Word sh_link */
		    0,				  /* Elf32_Word sh_info */
		    1,				  /* Elf32_Word sh_addr_align */
		    0);				  /* Elf32_Word sh_enter_size */
	cur_offset += shstrtab_size;
	ehdr.e_shoff = cur_offset;
	cur_offset += 9 * 40;
	AddShdr(SYM_SEG, SHT_SYMTAB, 0, 0, cur_offset, sym_name.size() * 16, 0, 0, 1, 16);
	shdr_tab[SYM_SEG]->sh_link = GetSegIndex(SYM_SEG) + 1;
	strtab_size = 0;
	for (int i = 0; i < sym_name.size(); i++) {
		strtab_size += sym_name[i].size() + 1;
	}
	cur_offset += sym_name.size() * 16;
	AddShdr(STR_SEG, SHT_STRTAB, 0, 0, cur_offset, strtab_size, SHN_UNDEF, 0, 1, 0);
	str_tab = new char[strtab_size];
	str = str_tab;
	index = 0;
	for (int i = 0; i < sym_name.size(); i++) {
		sym_tab[sym_name[i]]->st_name = index;
		strcpy(str + index, sym_name[i].c_str());
		index += (sym_name[i].size() + 1);
	}

	// rel
	for (int i = 0; i < rel_tab.size(); i++) {
		Elf32_Rel* rel = new Elf32_Rel();
		rel->r_offset = rel_tab[i]->get_offset();
		rel->r_info = ELF32_R_INFO((Elf32_Word)GetSymIndex(rel_tab[i]->get_name()), rel_tab[i]->get_type());
		if (rel_tab[i]->get_target_seg() == TEXT_SEG) {
			rel_text_tab.push_back(rel);
		}
		else if (rel_tab[i]->get_target_seg() == DATA_SEG) {
			rel_data_tab.push_back(rel);
		}
	}
	cur_offset += strtab_size;
	AddShdr(REL_TEXT_SEG, SHT_REL, 0, 0, cur_offset, rel_text_tab.size() * 8, GetSegIndex(SYM_SEG), GetSegIndex(TEXT_SEG), 1, 8);
	
	cur_offset += rel_text_tab.size() * 8;
	AddShdr(REL_DATA_SEG, SHT_REL, 0, 0, cur_offset, rel_data_tab.size() * 8, GetSegIndex(SYM_SEG), GetSegIndex(DATA_SEG), 1, 8);

	for (int i = 0; i < shdr_name.size(); i++) {
		int index = shstr_index[shdr_name[i]];
		shdr_tab[shdr_name[i]]->sh_name = index;
	}
}

void Elf_File::WriteElfTail() {
	Generate::WriteBytes(sh_str_tab, shstrtab_size);

	Elf32_Shdr* sh = new Elf32_Shdr();
	for (int i = 0; i < shdr_name.size(); i++) {
		sh = shdr_tab[shdr_name[i]];
		Generate::WriteBytes(sh, ehdr.e_shentsize);
	}
	delete sh;

	Elf32_Sym* sym = new Elf32_Sym();
	for (int i = 0; i < sym_name.size(); i++) {
		sym = sym_tab[sym_name[i]];
		Generate::WriteBytes(sym, sizeof(Elf32_Sym));
	}
	delete sym;

	Generate::WriteBytes(str_tab, strtab_size);

	Elf32_Rel* rel = new Elf32_Rel();
	for (int i = 0; i < rel_text_tab.size(); i++) {
		rel = rel_text_tab[i];
		Generate::WriteBytes(rel, sizeof(Elf32_Rel));
	}

	for (int i = 0; i < rel_data_tab.size(); i++) {
		rel = rel_data_tab[i];
		Generate::WriteBytes(rel, sizeof(Elf32_Rel));
	}
	delete rel;
}

void Elf_File::WriteElf(string name,int data_length,bool scan) {
	Generate::Over();
	Generate::Init(name + ".o");
	AssembleObj(data_length);
	Generate::WriteBytes(&ehdr, ehdr.e_ehsize);
	fin = fopen((name + ".t").c_str(), "rb");
	char buffer[1024] = { 0 };
	int read_length = -1;
	while (read_length != 0) {
		read_length = fread(buffer, 1, 1024, fin);
		Generate::WriteBytes(&buffer, read_length);
	}
	PaddingSeg(TEXT_SEG, DATA_SEG);
	Table::Write(scan);
	PaddingSeg(DATA_SEG, BSS_SEG);
	WriteElfTail();

}