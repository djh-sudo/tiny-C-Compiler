#include "Elf_File.h"
#include "Common.h"


RelItem::RelItem() {
	this->rel = nullptr;
	this->seg_name = "";
	this->rel_name = "";
}

RelItem::RelItem(string seg_name, Elf32_Rel* r, string rel_name) {
	this->seg_name = seg_name;
	this->rel = r;
	this->rel_name = rel_name;
}

RelItem::~RelItem() {
	delete rel;
}

string RelItem::get_rel_name() {
	return rel_name;
}

string RelItem::get_seg_name() {
	return seg_name;
}


Elf_File::Elf_File() {
	this->ehdr = Elf32_Ehdr();
	this->file_name = "";
	this->str_tab = nullptr;
	this->shstr_tab = nullptr;
	this->shstr_tab_size = 0;
	this->str_tab_size = 0;
	this->phdr_tab.clear();
	this->shdr_tab.clear();
	this->shdr_names.clear();
	this->sym_names.clear();
	this->rel_tab.clear();
	this->sym_tab.clear();
}

void Elf_File::ReadElf(const char* file_name) {
	this->file_name = file_name;
	FILE* fp = fopen(file_name, "rb");
	if (fp != nullptr) {
		rewind(fp);
		fread(&ehdr, sizeof(Elf32_Ehdr), 1,fp);
		// excutive file
		if (ehdr.e_type == ET_EXEC) {
			fseek(fp, ehdr.e_phoff, 0);
			for (int i = 0; i < ehdr.e_phnum; i++) {
				Elf32_Phdr* phdr = new Elf32_Phdr();
				fread(phdr, sizeof(Elf32_Phdr), 1, fp);
				phdr_tab.push_back(phdr);
			}
		}
		
		// shstrtab
		fseek(fp, ehdr.e_shoff + ehdr.e_shentsize * ehdr.e_shstrndx, 0);
		Elf32_Shdr shstr_tab;
		fread(&shstr_tab, sizeof(Elf32_Shdr), 1, fp);
		char* shstr_tab_data = new char[shstr_tab.sh_size];
		fseek(fp, shstr_tab.sh_offset, 0);
		fread(shstr_tab_data, shstr_tab.sh_size, 1, fp);
		
		// shdr
		fseek(fp, ehdr.e_shoff, 0);
		for (int i = 0; i < ehdr.e_shnum; i++) {
			Elf32_Shdr* shdr = new Elf32_Shdr();
			fread(shdr, ehdr.e_shentsize, 1, fp);
			string tmp_name(shstr_tab_data + shdr->sh_name);
			shdr_names.push_back(tmp_name);
			if (tmp_name.empty()) {
				delete shdr;
			}
			else {
				shdr_tab[tmp_name] = shdr;
			}
		}
		delete[] shstr_tab_data;

		// strtab
		Elf32_Shdr* strtab = shdr_tab[STR_SEG];
		char* str_tab_data = new char[strtab->sh_size];
		fseek(fp, strtab->sh_offset, 0);
		fread(str_tab_data, strtab->sh_size, 1, fp);
		
		// symtab
		Elf32_Shdr* sh_symtab = shdr_tab[SYM_SEG];
		fseek(fp, sh_symtab->sh_offset, 0);
		int sym_number = sh_symtab->sh_size / (sizeof(Elf32_Sym));
		vector<Elf32_Sym*>sym_list;
		for (int i = 0; i < sym_number; i++) {
			Elf32_Sym* sym_tmp = new Elf32_Sym();
			fread(sym_tmp, sizeof(Elf32_Sym), 1, fp);
			sym_list.push_back(sym_tmp);
			string tmp_name(str_tab_data + sym_tmp->st_name);
			if (tmp_name.empty()) {
				delete sym_tmp;
			}
			else {
				sym_tab[tmp_name] = sym_tmp;
			}
		}

		// reltab
		unordered_map<string, Elf32_Shdr*>::iterator i;
		for (i = shdr_tab.begin(); i != shdr_tab.end(); i++) {
			if (i->first.find(REL_SEG) == 0) {
				Elf32_Shdr* shrel_tab = shdr_tab[i->first];
				fseek(fp, shrel_tab->sh_offset, 0);
				int rel_number = shrel_tab->sh_size / sizeof(Elf32_Rel);
				for (int k = 0; k < rel_number; k++) {
					Elf32_Rel* r = new Elf32_Rel();
					fread(r, sizeof(Elf32_Rel), 1, fp);
					string tmp_name(str_tab_data + sym_list[ELF32_R_SYM(r->r_info)]->st_name);
					rel_tab.push_back(new RelItem(i->first.substr(4), r, tmp_name));
				}
			}
		}

		delete[] str_tab_data;
		fclose(fp);
		fp = nullptr;
	}
	else {
		xPANIC("%s%s%s", "file ", file_name, " not exit!\n");
		exit(0);
	}
}

void Elf_File::GetData(char* buffer, Elf32_Off offset, Elf32_Word size) {
	FILE* fp = fopen(file_name.c_str(), "rb");
	if (fp != nullptr) {
		rewind(fp);
		fseek(fp, offset, 0);
		fread(buffer, size, 1, fp);
		fclose(fp);
		fp = nullptr;
	}
	else {
		xPANIC("%s%s%s", "file ", file_name, " not exit!\n");
		exit(0);
	}
}

int Elf_File::GetSegIndex(string seg_name) {
	int index = 0;
	for (int i = 0; i < shdr_names.size(); i++) {
		if (seg_name == shdr_names[i]) {
			break;
		}
		index++;
	}
	return index;
}

int Elf_File::GetSymIndex(string sym_name) {
	int index = 0;
	for (int i = 0; i < sym_names.size(); i++) {
		if (sym_name == shdr_names[i]) {
			break;
		}
		index++;
	}
	return index;
}

void Elf_File::AddPhdr(Elf32_Word type, Elf32_Off offset, Elf32_Addr vir_addr,
			           Elf32_Word file_size, Elf32_Word mem_size, 
	                   Elf32_Word flags, Elf32_Word align) {
	Elf32_Phdr* ph = new Elf32_Phdr();
	ph->p_type = type;
	ph->p_offset = offset;
	ph->p_vaddr = vir_addr;
	ph->p_paddr = vir_addr;
	ph->p_filesz = file_size;
	ph->p_memsz = mem_size;
	ph->p_flags = flags;
	ph->p_align = align;
	phdr_tab.push_back(ph);
}

void Elf_File::AddShdr(string sh_name, Elf32_Word sh_type, Elf32_Word sh_flags,
	                   Elf32_Addr sh_addr, Elf32_Off sh_offset, Elf32_Word sh_size, 
	                   Elf32_Word sh_link,Elf32_Word sh_info, Elf32_Word sh_align, 
	                   Elf32_Word sh_entsize) {
	Elf32_Shdr* sh = new Elf32_Shdr();
	sh->sh_name = 0;
	sh->sh_type = sh_type;
	sh->sh_flags = sh_flags;
	sh->sh_addr = sh_addr;
	sh->sh_offset = sh_offset;
	sh->sh_size = sh_size;
	sh->sh_link = sh_link;
	sh->sh_info = sh_info;
	sh->sh_addralign = sh_align;
	sh->sh_entsize = sh_entsize;
	shdr_tab[sh_name] = sh;
	shdr_names.push_back(sh_name);
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
	sym_names.push_back(name);
}

void Elf_File::Write(const char* output, bool flag) {
	if (flag) {
		FILE* fp = fopen(output, "wb");
		fwrite(&ehdr, ehdr.e_ehsize, 1, fp);
		fflush(fp);
		if (!phdr_tab.empty()) {
			for (int i = 0; i < phdr_tab.size(); i++) {
				fwrite(phdr_tab[i], sizeof(Elf32_Phdr), 1, fp);
				fflush(fp);
			}
		}
		fclose(fp);
		fp = nullptr;
	}
	else {
		FILE* fp = fopen(output,"ab");
		fwrite(shstr_tab, shstr_tab_size, 1, fp);
		fflush(fp);
		
		for (int i = 0; i < shdr_names.size(); i++) {
			Elf32_Shdr* sh = shdr_tab[shdr_names[i]];
			fwrite(sh, sizeof(Elf32_Shdr), 1, fp);
			fflush(fp);
		}

		for (int i = 0; i < sym_names.size(); i++) {
			Elf32_Sym* s = sym_tab[sym_names[i]];
			fwrite(s, sizeof(Elf32_Sym), 1, fp);
			fflush(fp);
		}

		fwrite(str_tab, str_tab_size, 1, fp);
		fflush(fp);
		fclose(fp);
		fp = nullptr;
	}
}