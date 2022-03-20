#include "Semantic.h"



int VarRecord::current_addr = 0x00000000;

VarRecord::VarRecord(string name, bool externed,string cur_seg_name) {
	this->sym_name = name;
	this->addr = current_addr;
	this->is_externed = externed;
	this->is_equ = false;
	this->seg_name = cur_seg_name;
	this->time = 0;
	this->len_type = 0;
	this->sym_array = nullptr;
	this->sym_length = 0;
	if (externed) {
		this->addr = 0;
		seg_name = "";
	}
}

VarRecord::VarRecord(string name, int addr,string cur_seg_name) {
	this->sym_name = name;
	this->is_equ = true;
	this, seg_name = cur_seg_name;
	this->addr = addr;
	this->time = 0;
	this->len_type = 0;
	this->sym_array = nullptr;
	this->sym_length = 0;
}

VarRecord::VarRecord(string name, int time, int len_type, 
	                 int content[], int content_len, string cur_seg_name) {
	this->sym_name = name;
	this->addr = current_addr;
	this->seg_name = cur_seg_name;
	this->is_equ = false;
	this->time = time;
	this->len_type = len_type;
	this->sym_length = content_len;
	this->sym_array = new int[content_len];
	for (int i = 0; i < content_len; i++) {
		this->sym_array[i] = content[i];
	}
}

void VarRecord::Write(bool scan) {
	for (int i = 0; i < time; i++) {
		for (int j = 0; j < sym_length; j++) {
			Generate::WriteBytes(sym_array[i], len_type, scan);
		}
	}
}

VarRecord::~VarRecord(){
	if (sym_array) {
		delete[] sym_array;
	}
}

int VarRecord::get_current_addr() {
	return current_addr;
}

string VarRecord::get_seg_name() {
	return seg_name;
}

string VarRecord::get_sym_name() {
	return sym_name;
}

bool VarRecord::get_is_equ() {
	return is_equ;
}

bool VarRecord::get_is_externed() {
	return is_externed;
}

int VarRecord::get_times() {
	return time;
}

int VarRecord::get_addr() {
	return addr;
}

int VarRecord::get_len_type() {
	return len_type;
}

int VarRecord::get_sym_length() {
	return sym_length;
}

// =====================================================
unordered_map<string, VarRecord*>Table::var_map = unordered_map<string, VarRecord*>();
vector<VarRecord*>Table::def_labs = vector<VarRecord*>();
bool Table::scan_first = true;

bool Table::HasName(string name) {
	return (var_map.find(name) != var_map.end());
}

void Table::AddVar(VarRecord* var) {
	if (!scan_first) {
		delete var;
		return;
	}
	if (HasName(var->get_sym_name())) {
		if (var_map[var->get_sym_name()]->get_is_externed() && !var->get_is_externed()) {
			delete var_map[var->get_sym_name()];
			var_map[var->get_sym_name()] = var;
		}
	}
	else {
		var_map[var->get_sym_name()] = var;
	}

	if (var->get_times() != 0 && var->get_seg_name() == ".data") {
		def_labs.push_back(var);
	}
}

VarRecord* Table::GetVar(string name,string cur_seg_name) {
	VarRecord* var;
	if (HasName(name)) {
		var = var_map[name];
	}
	else {
		var_map[name] = new VarRecord(name, true, cur_seg_name);
		var = var_map[name];
	}
	return var;
}

void Table::SwtichSeg(int& data_len, string& cur_seg_name,string seg_name) {
	if (scan_first) {
		data_len += (4 - data_len % 4) % 4;
		Elf_File::AddShdr(cur_seg_name, VarRecord::current_addr, data_len);
		if (cur_seg_name != ".bss") {
			data_len += VarRecord::current_addr;
		}
	}
	cur_seg_name = seg_name;
	VarRecord::current_addr = 0x00000000;
}
	
void Table::ExportSym() {
	unordered_map<string, VarRecord*>::iterator vi, vend;
	vend = var_map.end();
	for (vi = var_map.begin(); vi != vend; vi++) {
		VarRecord* var = vi->second;
		if (!var->get_is_equ()) {
			Elf_File::AddSym(var);
		}
	}
}

void Table::Write(bool scan) {
	for (int i = 0; i < def_labs.size(); i++) {
		def_labs[i]->Write(scan);
	}
}

void Table::Over() {
	unordered_map<string, VarRecord*>::iterator vi, vend;
	vend = var_map.end();
	for (vi = var_map.begin(); vi != vend; vi++) {
		delete vi->second;
	}
	var_map.clear();
}

void Table::set_scan_first(bool scan) {
	scan_first = scan;
}

bool Table::get_scan_first() {
	return scan_first;
}

// ====================================================
ModRM::ModRM() {
	Init();
}

void ModRM::Init() {
	this->mod = -1;
	this->reg = 0;
	this->rm = 0;
}

int ModRM::get_mod() {
	return this->mod;
}

int ModRM::get_reg() {
	return this->reg;
}

int ModRM::get_rm() {
	return this->rm;
}

void ModRM::set_mod(int mod) {
	this->mod = mod;
}

void ModRM::set_reg(int reg) {
	this->reg = reg;
}

void ModRM::set_rm(int rm) {
	this->rm = rm;
}

SIB::SIB() {
	Init();
}

void SIB::Init() {
	this->scale = -1;
	this->index = 0;
	this->base = 0;
}
	
int SIB::get_scale() {
	return this->scale;
}

int SIB::get_index() {
	return this->index;
}

int SIB::get_base() {
	return this->base;
}

void SIB::set_scale(int scale) {
	this->scale = scale;
}

void SIB::set_index(int index) {
	this->index = index;
}

void SIB::set_base(int base) {
	this->base = base;
}

Inst::Inst() {
	Init();
}

void Inst::Init() {
	this->opr = 0;
	this->disp = 0;
	this->disp_len = 0;
	this->imm32 = 0;
	this->mod_rm.Init();
	this->sib.Init();
}

void Inst::SetDisp(int disp,int len) {
	this->disp = disp;
	this->disp_len = len;
}

void Inst::WriteDisp(bool scan) {
	if (disp_len) {
		Generate::WriteBytes(disp, disp_len, scan);
		disp_len = 0;
	}
}

int Inst::get_imm32() {
	return this->imm32;
}

int Inst::get_disp(){
	return this->disp;
}

int Inst::get_disp_len() {
	return this->disp_len;
}

unsigned char Inst::get_opr() {
	return this->opr;
}

void Inst::set_opr(unsigned char opr) {
	this->opr = opr;
}

void Inst::set_imm32(int imm32) {
	this->imm32 = imm32;
}

