#include "Semantic.h"

/* ********** VarRecord **********  */
VarRecord::VarRecord() {
	this->type = null;
	this->name = "";
	this->value = 0;
	this->local_addr = 0;
	this->externed = false;
}

VarRecord::VarRecord(const VarRecord& src) {
	this->type = src.type;
	this->name = src.name;
	this->value = src.value;
	this->local_addr = src.local_addr;
	this->externed = src.externed;
}

void VarRecord::Copy(const VarRecord* src) {
	this->type = src->type;
	this->name = src->name;
	this->value = src->value;
	this->local_addr = src->local_addr;
	this->externed = src->externed;
}

void VarRecord::Init(symbol dec_type, string name) {
	this->type = dec_type;
	this->name = name;
	this->value = 0;
	this->local_addr = 0;
	this->externed = false;
}

symbol VarRecord::get_type(){
	return this->type;
}

string VarRecord::get_name(){
	return this->name;
}

int VarRecord::get_value() {
	return this->value;
}

int VarRecord::get_local_addr() {
	return this->local_addr;
}

bool VarRecord::get_externed() {
	return this->externed;
}

void VarRecord::set_type(symbol type){
	this->type = type;
}

void VarRecord::set_name(string name){
	this->name = name;
}

void VarRecord::set_value(int value) {
	this->value = value;
}

void VarRecord::set_local_addr(int addr) {
	this->local_addr = addr;
}

void VarRecord::set_externed(bool externed) {
	this->externed = externed;
}

/* ********* FunRecord ********* */

FunRecord::FunRecord() {
	this->type = null;
	this->name = "";
	this->arguments = new vector<symbol>();
	this->arguments->clear();
	this->local_vars = new vector<VarRecord*>();
	this->local_vars->clear();
	this->defined = false;
	this->flushed = false;
	this->has_return = false;
}
// 拷贝构造函数，仅仅拷贝函数声明
FunRecord::FunRecord(const FunRecord& src) {
	this->type = src.type;
	this->name = src.name;
	this->arguments = new vector<symbol>();
	this->arguments->clear();
	for (int i = 0; i < arguments->size(); i++) {
		this->arguments->push_back((*(src.arguments))[i]);
	}
	this->local_vars = nullptr;
	this->defined = src.defined;
}

void FunRecord::Init(symbol dec_type, string name) {
	this->type = dec_type;
	this->name = name;
	this->arguments->clear();
	this->local_vars->clear();
	this->defined = false;
	this->flushed = false;
	this->has_return = false;
}

void FunRecord::AddArgs(VarRecord var) {
	this->arguments->push_back(var.get_type());
	this->PushLocalVar(var);
}


bool FunRecord::HasName(string name) {
	return false;
	if (local_vars) {
		int len = this->local_vars->size();
		for (int i = 0; i < len; i++) {
			if ((*local_vars)[i]->get_name() == name)
				return true;
		}
	}
	return false;
}

void FunRecord::PushLocalVar(VarRecord var) {
	if (!defined) {
		this->local_vars->push_back(new VarRecord(var));
	}
	else {
		VarRecord* p_var = new VarRecord(var);
		this->local_vars->push_back(p_var);
		int arg_length = this->arguments->size();
		int local_var_length = this->local_vars->size();
		p_var->set_local_addr(-4 * (local_var_length - arg_length));
	}
}

int FunRecord::GetCurrentAddr() {
	if (arguments && local_vars) {
		int args_len = this->arguments->size();
		int loc_len = this->local_vars->size();
		return -4 * (loc_len - args_len);
	}
	else {
		// assert
	}
}

void FunRecord::FlushArgs() {
	if (arguments && local_vars) {
		int args_len = this->arguments->size();
		for (int i = args_len - 1; i >= 0; i--) {
			VarRecord* p = (*local_vars)[i];
			// ebp + 4 * count + ret_addr
			p->set_local_addr(4 * (i + 2));
			if (p->get_type() == rev_string) {
				p->set_value(GLOBAL_STRING);
			}
			// table add
		}
		this->flushed = true;
	}
	else {
		// assert
		return;
	}
	
}

void FunRecord::PopLocalVars(int var_number) {
	if (local_vars && arguments) {
		for (int i = 0; i < var_number; i++) {
			if ((*local_vars)[local_vars->size() - 1]->get_name()[0] != '@') {
				// table del
				this->local_vars->pop_back();
			}
		}
		if (var_number == -1) {
			// 函数定义结束
			int args_len = arguments->size();
			int var_len = local_vars->size();
			for (int i = 0; i < args_len; i++) {
				// table del
			}
			local_vars->clear();
		}
	}
	else {
		// assert
	}
	return;
}

bool FunRecord::equal(FunRecord& function) {
	bool flag = true;
	if (arguments && function.arguments) {
		if (arguments->size() == function.arguments->size()) {
			for (int i = 0; i < function.arguments->size(); i++) {
				if ((*function.arguments)[i] != (*arguments)[i]) {
					return false;
				}
			}
		}
		else
			return false;
	}
	else {
		// assert
	}
	return (type == function.get_type() &&
		    name == function.get_name());
}

VarRecord* FunRecord::CreateTmpVar(symbol type, bool has_val, int& var_number, int num) {
	VarRecord* tmp = new VarRecord();
	switch (type) {
		case rev_int:{
			if (has_val) {
				tmp->set_value(num);
				break;
			}
		}
		case rev_char: {
			if (has_val) {
				tmp->set_value(num);
			}
			break;
		}
		case rev_string: {
			if (has_val) {
				// table add str
			}
			else {
				tmp->set_value(TEMP_STRING);
			}
			break;
		}
	}
	// gen name
	tmp->set_type(type);
	var_number++;
	this->local_vars->push_back(tmp);
	// table add
	int args_len = this->arguments->size();
	int loc_len = this->local_vars->size();
	tmp->set_local_addr(-4 * (loc_len - args_len));
	// gen var
	return tmp;
}
	
FunRecord::~FunRecord() {
	this->type = null;
	this->name = "";
	if (local_vars) {
		if (!flushed) {
			int args_len = arguments->size();
			for (int i = 0; i < args_len; i++) {
				delete (*local_vars)[i];
			}
		}
		this->local_vars->clear();
		delete this->local_vars;
	}
	this->arguments->clear();
	delete this->arguments;
	this->defined = false;
	this->has_return = false;
}


symbol FunRecord::get_type() {
	return type;
}

string FunRecord::get_name() {
	return name;
}

bool FunRecord::get_defined() {
	return defined;
}

bool FunRecord::get_flushed() {
	return flushed;
}

bool FunRecord::get_has_return() {
	return has_return;
}

void FunRecord::set_defined(bool defined) {
	this->defined = defined;
}

void FunRecord::set_flushed(bool flushed) {
	this->flushed = flushed;
}
void FunRecord::set_has_return(bool has_return) {
	this->has_return = has_return;
}