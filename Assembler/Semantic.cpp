#include "Semantic.h"

int VarRecord::current_addr = 0x00000000;

VarRecord::VarRecord(string name, bool externed,string cur_seg_name) {
	this->sym_name = name;
	this->addr = current_addr;
	this->is_externed = externed;
	this->is_equ = false;
	this->seg_name = cur_seg_name;
	this->times = 0;
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
	this->times = 0;
	this->len_type = 0;
	this->sym_array = nullptr;
	this->sym_length = 0;
}

VarRecord::VarRecord(string name, int times, int len_type, 
	                 int content[], int content_len, string cur_seg_name) {
	this->sym_name = name;
	this->addr = current_addr;
	this->seg_name = cur_seg_name;
	this->is_equ = false;
	this->times = times;
	this->len_type = len_type;
	this->sym_length = content_len;
	this->sym_array = new int[content_len];
	for (int i = 0; i < content_len; i++) {
		this->sym_array[i] = content[i];
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
	return times;
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