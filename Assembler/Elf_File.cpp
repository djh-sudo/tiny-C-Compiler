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

Elf_File::Elf_File() {
	this->sh_str_tab = nullptr;
	this->str_tab = nullptr;
	this->shstrtab_size = 0;
	this->strtab_size = 0;
	this->shdr_tab.clear();
	this->shdr_name.clear();
	this->rel_tab.clear();
	this->sym_name.clear();
	this->rel_data_tab.clear();
	this->rel_text_tab.clear();
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
		AddShdr(name, SHT_PROGBITS, SHF_ALLOC | SHF_WRITE, 0, offset, size, 0, 0, 4, 0);
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
	Elf32_Sym* sym = sym_tab[name] = new Elf32_Sym();
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

