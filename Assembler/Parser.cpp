#include "Parser.h"
#include "Generate.h"


void Parser::Init() {
	this->scan_first = true;
	this->rel_var = nullptr;
}

bool Parser::NextToken() {
	if (wait) {
		wait = false;
		return true;
	}
	bool flag = false;
	while (true) {
		flag = lexer.GetSym();
		if (token == null || token == excep) {
			if (!flag) {
				token = null;
				if (scan_first) {
					// table
					continue;
				}
				return false;
			}
		}
		else {
			token = lexer.get_symbol();
			return true;
		}
	}
}

bool Parser::Match(symbol t) {
	return token == t;
}

void Parser::Program() {
	if (!NextToken() || token == null) {
		// table
		return;
	}
	else {
		string sym_name;
		switch(token) {
			case ident:{
				sym_name = lexer.get_id();
				IdentTail(sym_name);
				break;
			}
			case rev_section: {
				NextToken();
				if (Match(ident)) {
					// table
				}
				else {
					// error
				}
				break;
			}
			case rev_global: {
				NextToken();
				if (!Match(ident)) {
					// error
				}
				break;
			}
			default: {
				Instruct();
				break;
			}
		}
	}
}

void Parser::IdentTail(string name) {
	switch (token) {
		case rev_times:{
			NextToken();
			if (Match(number)) {
				BaeseTail(name,lexer.get_num());
			}
			else {
				// error
			}
			break;
		}
		case rev_equ: {
			NextToken();
			if (Match(number)) {
				VarRecord* var = new VarRecord(name, lexer.get_num(),cur_seg);
				// table
			}
			else {
				// error
			}
			break;
		}
		case colon: {
			VarRecord* var = new VarRecord(name, false, cur_seg);
			// table
			break;
		}
		default: {
			BaeseTail(name, 1);
			break;
		}
	}
}

void Parser::BaeseTail(string name, int num) {
	int length = Len();
	Value(name, num, length);
}

int Parser::Len() {
	NextToken();
	switch (token) {
		case rev_db: {
			return 1;
		}
		case rev_dw: {
			return 2;
		}
		case rev_dd: {
			return 4;
		}
		default: {
			// error
		}
	}
}

void Parser::Value(string name, int time, int len) {
	int content[255] = { 0 };
	int content_length = 0;
	Type(content, content_length, len);
	ValTail(content, content_length, len);
	VarRecord* var = new VarRecord(name, time, len, content, content_length, cur_seg);
	// table
}

void Parser::Type(int content[], int& content_len, int len) {
	NextToken();
	switch (token) {
		case number:{
			content[content_len] = lexer.get_num();
			content_len++;
			break;
		}
		case strings: {
			string tmp = lexer.get_str();
			for (int i = 0; i<tmp.size(); i++) {
				content[content_len] = (unsigned char)tmp[i];
				content_len++;
			}
			break;
		}
		case ident: {
			string name = lexer.get_id();
			// table
			break;
		}
		default: {
			// error
			break;
		}
	}
}

void Parser::ValTail(int content[], int& content_len, int len) {
	NextToken();
	if (token == comma) {
		Type(content, content_len, len);
		ValTail(content, content_len, len);
	}
	else {
		this->wait = true;
	}
	return;
}

void Parser::Instruct() {
	instructure.Init();
	int len = 0;
	NextToken();
	if (token >= rev_mov && token <= rev_lea) {
		symbol op = token;
		int des_type = 0;
		int src_type = 0;
		int reg_num = 0;
		Operate(reg_num, des_type, len);
		NextToken();
		if (Match(comma)) {
			Operate(reg_num, src_type, len);
			// gen code
		}
		else {
			// error
		}
	}
	else if (token >= rev_call && token <= rev_pop) {
		symbol op = token;
		int type = 0;
		int reg_num = 0;
		Operate(reg_num, type, len);
		// gen code
	}
	else if (token == rev_ret) {
		symbol op = token;
		// gen code
	}
	else {
		// error
	}
}

void Parser::Operate(int& reg_num, int& type, int& len) {
	string name = "";
	VarRecord* var;
	NextToken();
	switch (token) {
		case number: {
			type = IMMD;
			instructure.set_imm32(lexer.get_num());
			break;
		}
		case ident: {
			type = IMMD;
			name = lexer.get_id();
			// table
			break;
		}
		case lbrac: {
			type = MEMO;
			this->wait = true;
			Memory();
			break;
		}
		case subs: {
			type = IMMD;
			NextToken();
			if (Match(number)) {
				instructure.set_imm32(-lexer.get_num());
			}
			else {
				// error
			}
			break;
		}
		default: {
			type = REGS;
			this->wait = true;
			len = Register();
			if (reg_num != 0) {
				instructure.mod_rm.set_mod(3);
				instructure.mod_rm.set_rm(instructure.mod_rm.get_reg());
				instructure.mod_rm.set_reg(token - rev_al - (1 - len % 4) * 8);
			}
			else {
				instructure.mod_rm.set_reg(token - rev_al - (1 - len % 4) * 8);
			}
			reg_num++;
			break;
		}
	}
}

int Parser::Register() {
	NextToken();
	if (token >= rev_al && token <= rev_bh) {
		return 1;
	}
	else if (token >= rev_eax && token <= rev_edi) {
		return 4;
	}
	else {
		// error
		return 0;
	}
}

void Parser::Memory(){
	NextToken();
	if (Match(lbrac)) {
		Addr();
		NextToken();
		if (!Match(rbrac)) {
			// error
		}
	}
	else {
		// error
	}
}

void Parser::Addr(){
	string name = "";
	VarRecord* var;
	NextToken();
	switch (token) {
		case number: {
			instructure.mod_rm.set_mod(0);
			instructure.mod_rm.set_rm(5);
			instructure.SetDisp(lexer.get_num(), 4);
			break;
		}
		case ident: {
			instructure.mod_rm.set_mod(0);
			instructure.mod_rm.set_rm(5);
			name = lexer.get_id();
			// table
			break;
		}
		default: {
			this->wait = true;
			int type = Register();
			RegAddr(token, type);
			break;
		}
	}
}

void Parser::RegAddr(symbol base_reg, const int type){
	NextToken();
	if (token == addi || token == subs) {
		RegAddrTail(base_reg, type, token);
	}
	else {
		if (base_reg == rev_esp) {
			instructure.mod_rm.set_mod(0);
			instructure.mod_rm.set_rm(4);
			instructure.sib.set_scale(0);
			instructure.sib.set_index(4);
			instructure.sib.set_base(4);
		}
		else if (base_reg == rev_ebp) {
			instructure.mod_rm.set_mod(1);
			instructure.mod_rm.set_rm(5);
			instructure.SetDisp(0, 1);
		}
		else {
			instructure.mod_rm.set_mod(0);
			instructure.mod_rm.set_rm(base_reg - rev_al - (1 - type % 4) * 8);
		}
		this->wait = true;
		return;
	}
}

void Parser::RegAddrTail(symbol base_reg, const int type, symbol sign){
	NextToken();
	if (token == number) {
		int number = lexer.get_num();
		if (sign == subs) {
			number = -number;
		}
		if (number >= -128 && number <= 128) {
			instructure.mod_rm.set_mod(1);
			instructure.SetDisp(number, 1);
		}
		else {
			instructure.mod_rm.set_mod(2);
			instructure.SetDisp(number, 4);
		}
		instructure.mod_rm.set_rm(base_reg - rev_al - (1 - type % 4) * 8);

		if (base_reg == rev_esp) {
			instructure.mod_rm.set_rm(4);
			instructure.sib.set_scale(0);
			instructure.sib.set_index(4);
			instructure.sib.set_base(4);
		}
	}
	else {
		this->wait = true;
		int type_reg = Register();
		instructure.mod_rm.set_mod(0);
		instructure.mod_rm.set_rm(4);
		instructure.sib.set_scale(0);
		instructure.sib.set_index(token - rev_al - (1 - type_reg % 4) * 8);
		instructure.sib.set_base(base_reg - rev_al - (1 - type % 4) * 8);
	}
}