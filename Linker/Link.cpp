#include <iostream>
#include "Link.h"
#include "Common.h"
#include "elf.h"


using namespace std;

Block::Block(char* data, int offset, int size) {
	this->data = data;
	this->offset = offset;
	this->block_size = size;
}

Block::~Block() {
	if (data) {
		delete[] data;
		data = nullptr;
	}
}

int Block::get_offset() {
	return this->offset;
}

int Block::get_block_size() {
	return this->block_size;
}

char* Block::get_data() {
	return data;
}

SegList::SegList() {
	this->begin = 0;
	this->base_addr = 0;
	this->offset = 0;
	this->size = 0;
	this->blocks.clear();
	this->file_list.clear();
}

SegList::~SegList() {
	file_list.clear();
	file_list.shrink_to_fit();
	for (int i = 0; i < blocks.size(); i++) {
		delete blocks[i];
	}
	blocks.clear();
	blocks.shrink_to_fit();
}

void SegList::AllocAddr(string name, int& base, int& offset) {
	begin = offset;
	if (name != BSS_SEG)
		base += (MEM_ALIGN - base % MEM_ALIGN) % MEM_ALIGN;
	int align = DISC_ALIGN;
	if (name == TEXT_SEG) {
		align = TEXT_ALIGN;
	}
	offset += (align - offset % align) % align;
	base = base - base % MEM_ALIGN + offset % MEM_ALIGN;
	this->base_addr = base;
	this->offset = offset;
	this->size = 0;
	for (int i = 0; i < file_list.size(); i++) {
		size += (DISC_ALIGN - size % DISC_ALIGN) % DISC_ALIGN;
		Elf32_Shdr* seg = file_list[i]->shdr_tab[name];
		if (name != BSS_SEG) {
			char* buffer = new char[seg->sh_size];
			file_list[i]->GetData(buffer, seg->sh_offset, seg->sh_size);
			blocks.push_back(new Block(buffer, size, seg->sh_size));
		}
		seg->sh_addr = base + size;
		size += seg->sh_size;
	}
	base += size;
	if (name != BSS_SEG) {
		offset += size;
	}
}

void SegList::RelocAddr(int rel_addr, int type, int sym_addr) {
	int rel_offset = rel_addr - base_addr;
	Block* block = nullptr;
	for (int i = 0; i < blocks.size(); i++) {
		if (blocks[i]->get_offset() <= rel_offset &&
			blocks[i]->get_offset() + blocks[i]->get_block_size() > rel_offset) {
			block = blocks[i];
			break;
		}
	}

	if (block) {
		int* addr = (int*)(block->get_data() + rel_offset - block->get_offset());
		if (type == R_386_32) {
			*addr = sym_addr;

		}
		else if (type == R_386_PC32) {
			*addr = sym_addr - rel_addr + *addr;
		}
	}
}

int SegList::get_base_addr() {
	return base_addr;
}

int SegList::get_offset() {
	return offset;
}

int SegList::get_size() {
	return size;
}

int SegList::get_begin() {
	return begin;
}

Link::Link() {
	this->start = nullptr;
	this->files.clear();
	this->all_sym.clear();
	this->sym_def.clear();
	this->seg_name.clear();
	this->seg_list.clear();
	seg_name.push_back(TEXT_SEG);
	seg_name.push_back(DATA_SEG);
	seg_name.push_back(BSS_SEG);
	for (int i = 0; i < seg_name.size(); i++) {
		seg_list[seg_name[i]] = new SegList();
	}
}
	
void Link::AddElf(const char* file_name) {
	Elf_File* elf = new Elf_File();
	elf->ReadElf(file_name);
	files.push_back(elf);
}

void Link::CollectInfo() {
	for (int i = 0; i < files.size(); i++) {
		Elf_File* elf = files[i];
		for (int i = 0; i < seg_name.size(); i++) {
			if (elf->shdr_tab.find(seg_name[i]) != elf->shdr_tab.end()) {
				seg_list[seg_name[i]]->file_list.push_back(elf);
			}
		}

		unordered_map<string, Elf32_Sym*>::iterator it;
		for (it = elf->sym_tab.begin(); it != elf->sym_tab.end(); it++) {
			SymLink* slink = new SymLink();
			slink->name = it->first;
			if (it->second->st_shndx == STN_UNDEF) {
				slink->recv = elf;
				slink->prov = nullptr;
				all_sym.push_back(slink);
			}
			else {
				slink->prov = elf;
				slink->recv = nullptr;
				sym_def.push_back(slink);
			}
		}
	}
}

bool Link::CheckSymIsValid() {
	bool res_flag = true;
	start = nullptr;
	for (int i = 0; i < sym_def.size(); i++) {
		if (ELF32_ST_BIND(sym_def[i]->prov->sym_tab[sym_def[i]->name]->st_info)
			!= STB_GLOBAL)
			continue;
		if (sym_def[i]->name == START) {
			start = sym_def[i]->prov;
		}
		for (int k = i + 1; k < sym_def.size(); k++) {
			if (ELF32_ST_BIND(sym_def[k]->prov->sym_tab[sym_def[k]->name]->st_info)
				!= STB_GLOBAL)
				continue;
			if (sym_def[i]->name == sym_def[k]->name) {
				// error
				cout << "symname [" << sym_def[i]->name << "] in file <" << sym_def[i]->prov->file_name
					 << "> conflict with file <" << sym_def[k]->prov->file_name << ">" << endl;
				res_flag = false;
			}
		}

	}
	if (start == nullptr) {
		// error
		cout << "Can't find excutive entry!" << endl;
		res_flag = false;
	}
	for (int i = 0; i < all_sym.size(); i++) {
		for (int k = 0; k < sym_def.size(); k++) {
			if (ELF32_ST_BIND(sym_def[k]->prov->sym_tab[sym_def[k]->name]->st_info)
				!= STB_GLOBAL)
				continue;
			if (sym_def[k]->name == all_sym[i]->name) {
				all_sym[i]->prov = sym_def[k]->prov;
				sym_def[k]->recv = sym_def[k]->prov;
			}
		}
		if (all_sym[i]->prov == nullptr) {
			// error
			int info = all_sym[i]->recv->sym_tab[all_sym[i]->name]->st_info;
			string type = "";
			if (ELF32_ST_TYPE(info) == STT_OBJECT) type = "variable";
			else if (ELF32_ST_TYPE(info) == STT_FUNC) type = "func";
			else type = "symbol";
			cout << type << " [" << all_sym[i]->name << "] is not defined in file <" 
				 << all_sym[i]->recv->file_name << ">" << endl;

			res_flag = false;
		}
	}
	return res_flag;
}

void Link::AllocAddr() {
	int cur_addr = BASE_ADDR;
	int cur_offset = sizeof(Elf32_Ehdr) + sizeof(Elf32_Phdr) * seg_name.size();

	for (int i = 0; i < seg_name.size(); i++) {
		seg_list[seg_name[i]]->AllocAddr(seg_name[i], cur_addr, cur_offset);
	}
}

void Link::SymParser() {
	for (int i = 0; i < sym_def.size(); i++) {
		Elf32_Sym* s = sym_def[i]->prov->sym_tab[sym_def[i]->name];
		string seg_name = sym_def[i]->prov->shdr_names[s->st_shndx];
		s->st_value += sym_def[i]->prov->shdr_tab[seg_name]->sh_addr;
	}

	for (int i = 0; i < all_sym.size(); i++) {
		Elf32_Sym* s_pro = all_sym[i]->prov->sym_tab[all_sym[i]->name];
		Elf32_Sym* s_rev = all_sym[i]->recv->sym_tab[all_sym[i]->name];
		s_rev->st_value = s_pro->st_value;
	}
}

void Link::Relocation() {
	for (int i = 0; i < files.size(); i++) {
		vector<RelItem*>rel_tabs = files[i]->rel_tab;
		for (int k = 0; k < rel_tabs.size(); k++) {
			Elf32_Sym* s = files[i]->sym_tab[rel_tabs[k]->get_rel_name()];
			int sym_addr = s->st_value;
			int rel_addr = files[i]->shdr_tab[rel_tabs[k]->get_seg_name()]->sh_addr +
				rel_tabs[k]->rel->r_offset;
			seg_list[rel_tabs[k]->get_seg_name()]->RelocAddr(rel_addr,
				ELF32_R_TYPE(rel_tabs[k]->rel->r_info), sym_addr);
		}
	}
}

void Link::AssemblyObj() {
	int* pointer = (int*)obj.ehdr.e_ident;
	*pointer = 0x464c457f;
	pointer++;
	*pointer = 0x00010101;
	pointer++;
	*pointer = 0x00;
	pointer++;
	*pointer = 0x00;
	obj.ehdr.e_type = ET_EXEC;
	obj.ehdr.e_machine = EM_386;
	obj.ehdr.e_version = EV_CURRENT;
	obj.ehdr.e_flags = 0;
	obj.ehdr.e_ehsize = 52;
	int cur_offset = sizeof(Elf32_Ehdr) + sizeof(Elf32_Phdr) * seg_name.size();
	obj.AddShdr("", 0, 0, 0, 0, 0, 0, 0, 0, 0);
	int shstr_tab_size = 26;
	/* SH_STR_SEG + SYM_;SEG + STR_SRG + 3 */
	for (int i = 0; i < seg_name.size(); i++) {
		string name = seg_name[i];
		shstr_tab_size += (name.size() + 1);
		Elf32_Word flags = PF_W | PF_R;
		Elf32_Word filesz = seg_list[name]->get_size();
		if (name == TEXT_SEG) flags = PF_X | PF_R;
		if (name == BSS_SEG) filesz = 0;
		obj.AddPhdr(PT_LOAD, seg_list[name]->get_offset(), seg_list[name]->get_base_addr(),
			filesz, seg_list[name]->get_size(), flags, MEM_ALIGN);
		cur_offset = seg_list[name]->get_offset();


		Elf32_Word sh_type = SHT_PROGBITS;
		Elf32_Word sh_flags = SHF_ALLOC | SHF_WRITE;
		Elf32_Word sh_align = DISC_ALIGN;
		if (name == BSS_SEG) sh_type = SHT_NOBITS;
		if (name == TEXT_SEG) {
			sh_flags = SHF_ALLOC | SHF_EXECINSTR;
			sh_align = TEXT_ALIGN;
		}
		obj.AddShdr(name, sh_type, sh_flags, seg_list[name]->get_base_addr(),
			seg_list[name]->get_offset(), seg_list[name]->get_size(), SHN_UNDEF, 0, sh_align, 0);
	}
	obj.ehdr.e_phoff = sizeof(Elf32_Ehdr);
	obj.ehdr.e_phentsize = sizeof(Elf32_Phdr);
	obj.ehdr.e_phnum = seg_name.size();

	// shstrtab
	obj.shstr_tab = new char[shstr_tab_size];
	char* str = obj.shstr_tab;
	obj.shstr_tab_size = shstr_tab_size;
	int index = 0;
	unordered_map<string, int>shstr_index;
	shstr_index.clear();

	shstr_index[SH_STR_SEG] = index;
	strcpy(str + index, SH_STR_SEG);
	index += 10;

	shstr_index[SYM_SEG] = index;
	strcpy(str + index, SYM_SEG);
	index += 8;

	shstr_index[STR_SEG] = index;
	strcpy(str + index, STR_SEG);
	index += 8;

	shstr_index[""] = index - 1;

	for (int i = 0; i < seg_name.size(); i++) {
		shstr_index[seg_name[i]] = index;
		strcpy(str + index, seg_name[i].c_str());
		index += seg_name[i].size() + 1;
	}

	obj.AddShdr(SH_STR_SEG, SHT_STRTAB, 0, 0, cur_offset, shstr_tab_size, SHN_UNDEF, 0, 1, 0);
	obj.ehdr.e_shstrndx = obj.GetSegIndex(SH_STR_SEG);

	cur_offset += shstr_tab_size;

	obj.ehdr.e_shoff = cur_offset;
	obj.ehdr.e_shentsize = sizeof(Elf32_Shdr);
	obj.ehdr.e_shnum = 4 + seg_name.size();
	cur_offset += sizeof(Elf32_Shdr) * (obj.ehdr.e_shnum);

	obj.AddShdr(SYM_SEG, SHT_SYMTAB, 0, 0, cur_offset, 
			   (1 + sym_def.size()) * sizeof(Elf32_Sym), 0, 0, 1, sizeof(Elf32_Sym));
	obj.shdr_tab[SYM_SEG]->sh_link = obj.GetSegIndex(SYM_SEG) + 1;

	int str_tab_size = 0;
	obj.AddSym("", nullptr);
	for (int i = 0; i < sym_def.size(); i++) {
		string name = sym_def[i]->name;
		str_tab_size += name.size() + 1;
		Elf32_Sym* s = sym_def[i]->prov->sym_tab[name];
		s->st_shndx = obj.GetSegIndex(sym_def[i]->prov->shdr_names[s->st_shndx]);
		obj.AddSym(name, s);
	}

	obj.ehdr.e_entry = obj.sym_tab[START]->st_value;
	cur_offset += (1 + sym_def.size()) * sizeof(Elf32_Sym);
	obj.AddShdr(STR_SEG, SHT_STRTAB, 0, 0, cur_offset, str_tab_size, SHN_UNDEF, 0, 1, 0);

	obj.str_tab = new char[str_tab_size];
	str = obj.str_tab;
	obj.str_tab_size = str_tab_size;
	index = 0;
	unordered_map<string, int>str_index;
	str_index.clear();
	str_index[""] = str_tab_size - 1;
	for (int i = 0; i < sym_def.size(); i++) {
		str_index[sym_def[i]->name] = index;
		strcpy(str + index, sym_def[i]->name.c_str());
		index += sym_def[i]->name.size() + 1;
	}

	unordered_map<string, Elf32_Sym*>::iterator it;
	for (it = obj.sym_tab.begin(); it != obj.sym_tab.end(); it++) {
		it->second->st_name = str_index[it->first];
	}

	unordered_map<string, Elf32_Shdr*>::iterator is;
	for (is = obj.shdr_tab.begin(); is != obj.shdr_tab.end(); is++) {
		is->second->sh_name = shstr_index[is->first];
	}
}

void Link::ExportElf(const char* file_name) {
	obj.Write(file_name, true);
	FILE* fp = fopen(file_name, "ab");
	char pading = 0;
	for (int i = 0; i < seg_name.size(); i++) {
		SegList* sl = seg_list[seg_name[i]];
		int padding_number = sl->get_offset() - sl->get_begin();
		while (padding_number--) {
			fwrite(&pading, 1, 1, fp);
			fflush(fp);
		}
		if (seg_name[i] != BSS_SEG) {
			Block* old_block = nullptr;
			char instruction_padding = 0x90;
			for (int k = 0; k < sl->blocks.size(); k++) {
				Block* new_block = sl->blocks[k];
				if (old_block != nullptr) {
					padding_number = new_block->get_offset() -
						             (old_block->get_offset() + 
								     old_block->get_block_size());
					while (padding_number--) {
						fwrite(&instruction_padding, 1, 1, fp);
						fflush(fp);
					}
				}
				old_block = new_block;
				fwrite(new_block->get_data(), new_block->get_block_size(), 1, fp);
				fflush(fp);
			}
		}
	}
	fclose(fp);
	fp = nullptr;
	obj.Write(file_name, false);
	return;
}

bool Link::Excute(const char*file_name) {
	CollectInfo();
	if (!CheckSymIsValid()) {
		cout << "Linking failed!" << endl;
		return false;
	}
	AllocAddr();
	SymParser();
	Relocation();
	AssemblyObj();
	ExportElf(file_name);
	cout << "Linking successfully!" << endl;
	cout << "output file >>> " << file_name << endl;
	return true;
}