#include "Semantic.h"
#include "Generator.h"


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
	for (int i = 0; i < src.arguments->size(); i++) {
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
	if (arguments) {
		this->arguments->push_back(var.get_type());
		this->PushLocalVar(var);
	}
	else {
		// assert
	}
}


bool FunRecord::HasName(string name) {
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
		VarTable::AddVar(p_var);
		int arg_length = this->arguments->size();
		int local_var_length = this->local_vars->size();
		p_var->set_local_addr(-4 * (local_var_length - arg_length));
		Generator::GenerateLocalVar(0);
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
				p->set_value(DYNAMIC_STRING);
			}
			// table add vars
			VarTable::AddVar(p);
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
			string name = (*local_vars)[local_vars->size() - 1]->get_name();
			// table del
			VarTable::DelVar(name);
			this->local_vars->pop_back();
		}
		if (var_number == -1) {
			// 函数定义结束
			int args_len = arguments->size();
			int var_len = local_vars->size();
			for (int i = 0; i < args_len; i++) {
				// table del
				string name = (*local_vars)[i]->get_name();
				VarTable::DelVar(name);
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

VarRecord* FunRecord::CreateTmpVar(symbol type, bool has_val, int& var_number, 
	                               string str, int num) {
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
				tmp->set_value(VarTable::AddString(str));
			}
			else {
				tmp->set_value(DYNAMIC_STRING);
			}
			break;
		}
	}
	// gen temp var name
	tmp->set_name(Generator::GenerateName("tmp", type, ""));
	tmp->set_type(type);
	var_number++;
	this->local_vars->push_back(tmp);
	// table add
	VarTable::AddVar(tmp);
	int args_len = this->arguments->size();
	int loc_len = this->local_vars->size();
	tmp->set_local_addr(-4 * (loc_len - args_len));
	// gen var
	Generator::GenerateLocalVar(tmp->get_value());
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
		this->local_vars = nullptr;
	}
	if (this->arguments) {
		this->arguments->clear();
		delete this->arguments;
		this->arguments = nullptr;
	}
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

// initial
bool VarTable::syntax_error = false;
int VarTable::string_id = 0;
unordered_map<string, VarRecord*> VarTable::var_map = unordered_map<string, VarRecord*>();
unordered_map<string, FunRecord*> VarTable::fun_map = unordered_map<string, FunRecord*>();
vector<VarRecord*> VarTable::parameter = vector<VarRecord*>();
vector<string*> VarTable::string_table = vector<string*>(); // 字符串空间


int VarTable::AddString(string str) {
	if (syntax_error) return 0;
	string_id++;
	string* p = new string(str);
	string_table.push_back(p);
	return string_id;
}

string VarTable::GetString(int index) {
	if (syntax_error) return "";
	if (index >= 0 && index < string_table.size()) {
		return *(string_table[index]);
	}
	else {
		return "";
	}
}

void VarTable::AddVar(VarRecord& var) {
	if (syntax_error)return;
	// 没有找到重复的记录
	if (var_map.find(var.get_name()) == var_map.end()) {
		VarRecord* p_var = new VarRecord(var);
		var_map[var.get_name()] = p_var;
	}
	else {
		VarRecord* p_var = var_map[var.get_name()];
		delete var_map[var.get_name()];
		var_map[var.get_name()] = p_var;
		Generator::SemanticError(var_redef);
	}
}

void VarTable::AddVar(VarRecord* var) {
	if (syntax_error) return;
	if (var_map.find(var->get_name()) == var_map.end()) {
		var_map[var->get_name()] = var;
	}
	else {
		Generator::SemanticError(var_redef);
		delete var;
	}
	return;
}

VarRecord* VarTable::GetVar(string name, FunRecord&fun) {
	if (syntax_error) return nullptr;
	if (HasName(name,fun)) {
		return var_map[name];
	}
	else {
		Generator::SemanticError(var_undef);
		return nullptr;
	}
}

bool VarTable::HasName(string name, FunRecord&fun) {
	if (syntax_error) return false;
	return (var_map.find(name) != var_map.end() 
		   || fun.HasName(name));
}

void VarTable::DelVar(string name) {
	if (syntax_error)return;
	if (var_map.find(name) != var_map.end()) {
		VarRecord* p = var_map[name];
		delete p;
		var_map.erase(name);
	}
	else {
		return;
	}
}

void VarTable::AddFun(FunRecord& fun) {
	if (syntax_error) return;
	if (fun_map.find(fun.get_name()) == fun_map.end()) {
		FunRecord* p = new FunRecord(fun);
		fun_map[fun.get_name()] = p;
		if (p->get_defined()) {
			fun.FlushArgs();
			Generator::GenerateFunHead(fun);
		}
	}
	else {
		FunRecord* p = fun_map[fun.get_name()];
		if (p->equal(fun)) {
			if (fun.get_defined()) {
				if (p->get_defined()) {
					Generator::SemanticError(fun_redef); 
					return;
				}
				else {
					p->set_defined(true);
					fun.FlushArgs();
					Generator::GenerateFunHead(fun);
				}
			}
			return;
		}
		else {
			// 
			FunRecord* p = new FunRecord(fun);
			delete fun_map[fun.get_name()];
			fun_map[fun.get_name()] = p;
			if (fun.get_defined()) {
				fun.FlushArgs();
				Generator::SemanticError(fun_def_error);
			}
			else {
				Generator::SemanticError(fun_dec_error);
			}
		}
	}

}

void VarTable::AddRealArgs(VarRecord* arg, int& var_number, FunRecord&fun) {
	if (syntax_error) return;
	if (arg->get_type() == rev_string) {
		VarRecord tmp;
		string name = "";
		tmp.Init(rev_string, name);
		arg = Generator::GenerateExp(&tmp, add, arg, var_number, fun);
	}
	parameter.push_back(arg);
}

VarRecord* VarTable::GenerateCall(string fname, int& var_number, FunRecord& fun) {
	if (syntax_error)return nullptr;
	VarRecord* tmp = nullptr;
	if (fun_map.find(fname) != fun_map.end()) {
		FunRecord* p = fun_map[fname];
		if (parameter.size() >= p->arguments->size()) {
			int para_size = parameter.size();
			int args_size = p->arguments->size();
			for (int i = para_size - 1, j = args_size - 1; j >= 0; i--, j--) {
				if (parameter[i]->get_type() != (*(p->arguments))[j]) {
					Generator::SemanticError(real_args_error);
					break;
				}
				else {
					VarRecord* ret = parameter[i];
					if (ret->get_type() == rev_string) {
						Generator::mov(eax, __ebp(ret->get_local_addr()));
					}
					else {
						if (ret->get_local_addr() == 0) {
							Generator::mov(eax, __("@var_" + ret->get_name()));
						}
						else {
							if (ret->get_local_addr() < 0) {
								Generator::mov(eax, __ebp(ret->get_local_addr()));
							}
							else {
								Generator::mov(eax, _ebp(ret->get_local_addr()));
							}
						}
					}
					Generator::push(eax);
				}
			}
			Generator::call(fname);
			Generator::addi(esp, 4 * para_size);
			if (p->get_type() != rev_void) {
				tmp = fun.CreateTmpVar(p->get_type(), false, var_number);
				Generator::mov(__ebp(tmp->get_local_addr()), eax);
				if (p->get_type() == rev_string) {
					VarRecord tmp_str;
					string temp_name = "";
					tmp_str.Init(rev_string, temp_name);
					tmp = Generator::GenerateExp(&tmp_str, add, tmp, var_number, fun);
				}
			}
			while (para_size--)
				parameter.pop_back();
		}
		else {
			Generator::SemanticError(real_args_error);
			return nullptr;
		}
	}
	else {
		Generator::SemanticError(fun_undef);
		return nullptr;
	}
	return tmp;
}

void VarTable::over() {
	if (syntax_error) return;
	unordered_map<string, VarRecord*>::iterator vs, vend;
	Generator::section("section .data");
	vend = var_map.end();
	for (vs = var_map.begin(); vs != vend; vs++) {
		VarRecord* p = vs->second;
		if (p->get_externed()) {
			continue;
		}
		else {
			if (p->get_type() == rev_string) {
				string name = Generator::GenerateName("str", null, p->get_name());
				Generator::other_ass(name + " times 255 db 0\n");
				name = Generator::GenerateName("str", null, p->get_name());
				Generator::other_ass(name + "_len db 0\n");
			}
			else {
				string name = Generator::GenerateName("var", null, p->get_name());
				Generator::other_ass(name + " dd 0\n");
			}
		}

	}

	char str_buffer[255];
	int length = 0;
	for (int i = 0; i < string_table.size(); i++) {
		strcpy(str_buffer, string_table[i]->c_str());
		length = string_table[i]->size();
		Generator::other_ass("@str_" + to_string(i + 1) + " db ");
		bool pass = false;
		for (int j = 0; j < length; j++) {
			if (str_buffer[j] == 10 || str_buffer[j] == 9 || str_buffer[j] == '"') {
				if (!pass) {
					if (j != 0) {
						Generator::other(",");
					}
					Generator::other(to_string(str_buffer[j]));
				}
				else {
					Generator::other("\"," + to_string(str_buffer[j]));
				}
				pass = false;
			}
			else {
				if (!pass) {
					if (j != 0) {
						Generator::other(",");
					}
					Generator::other("\"" + string(1, str_buffer[j]));
					if (j == length - 1) {
						Generator::other("\"");
					}
				}
				else {
					Generator::other(string(1,str_buffer[j]));
					if (j == length - 1) {
						Generator::other("\"");
					}
				}
				pass = true;
			}
		}
		if (length == 0) {
			Generator::other("\"\"");
		}
		Generator::other("\n");
		Generator::other_ass("@str_" + to_string(i + 1) + "_len equ " + to_string(length) + "\n");
	}
}

void VarTable::clear() {
	unordered_map<string, VarRecord*>::iterator vi, vend;
	vend = var_map.end();
	for (vi = var_map.begin(); vi != vend; vi++) {
		delete vi->second;
	}
	var_map.clear();
	unordered_map<string, FunRecord*>::iterator fi, fend;
	fend = fun_map.end();
	for (fi = fun_map.begin(); fi != fend; fi++) {
		delete fi->second;
	}
	fun_map.clear();
	for (int i = 0; i < string_table.size(); i++) {
		delete string_table[i];
	}
	string_table.clear();
	parameter.clear();
	parameter.shrink_to_fit();




}