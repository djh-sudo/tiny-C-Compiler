#include <string>
#include <algorithm>
#include <vector>
#include "Parser.h"
#include "Generator.h"


Parser::Parser(){
	this->wait = false;
	this->old_token = null;
	this->token = null;
	this->if_id = 0;
	this->while_id = 0;
	this->for_id = 0;
	this->switch_id = 0;
	this->syntax_error = 0;
	this->compiler_ok = false;
	this->ident_only = false;
	this->has_default = false;
	this->break_level.clear();
	this->case_number.clear();
}


void Parser::Init(const char* file_name) {
	bool result = lexer.Init(file_name);
	if (result) {
		this->file_name = file_name;
		xSUCC("%s", "\nLexer initial successfully ...\n");
		string output = file_name;
		int index = output.find_last_of("/");
		if (index == string::npos) {
			index = output.find_last_of("\\");
		}
		output = output.substr(0,index) + "/common.s";
		result = Generator::GenerateComm(output);
		if (result) {
			int index = string(file_name).find_last_of(".");
			string output = string(file_name).substr(0, index);
			result = Generator::Init((output + ".s").c_str());
			if (result) {
				xSUCC("%s", "Generator initial successfully ...\n");
				Generator::section("section .text");
			}
			else {
				xPANIC("%s", "Generator initial failed!\n");
				exit(1);
			}
		}
		else {
			xPANIC("%s", "Generator common file failed!\n");
			exit(1);
		}
	}
	else {
		exit(1);
	}
}

bool Parser::NextToken() {
	if (wait) {
		wait = !wait;
		return true;
	}
	bool result = false;
	while (true) {
		result = lexer.GetSymbol();
		if (lexer.get_sym() == null || lexer.get_sym() == excep) {
			if (!result) {
				old_token = token;
				token = null;
				return false;
			}
		}
		else {
			old_token = token;
			token = lexer.get_sym();
			return true;
		}
	}
}

bool Parser::Match(symbol s) {
	return s == token;
}

void Parser::Program() {
	if (!NextToken()) {
		compiler_ok = true;
		if (syntax_error == 0 && lexer.get_error_number() == 0
			&& !Generator::error) {
			VarTable::over();
			xSUCC("%s%s%s", "compiler ", file_name.c_str(), " successfully!\n");
		}
		else {
			xPANIC("%s%s%s", "compiler ", file_name.c_str(), " failed!\n");
		}
		xINFO("%s%d%s","lexical error:" , lexer.get_error_number(),"\n");
		xINFO("%s%d%s", "syntax error:", syntax_error,"\n");
	}
	else {
		Dec();
		Program();
	}
}

void Parser::Dec() {
	// empty declare
	if (token == semicon) return;
	// extern variable declare
	else if (token == rev_extern) {
		NextToken();
		symbol dec_type = Type();
		string dec_name = "";
		NextToken();
		if (!Match(ident)) {
			SyntaxError(ident_lost);
			this->wait = true;
		}
		else {
			dec_name = lexer.get_id();
			var.Init(dec_type, dec_name);
			var.set_externed(true);
			if (dec_type == rev_string) {
				var.set_value(GLOBAL_STRING);
			}
			VarTable::AddVar(var);
		}
		NextToken();
		if (!Match(semicon)) {
			if (token == rev_extern || token == rev_void ||
				token == rev_int || token == rev_char ||
				token == rev_string) {
				SyntaxError(semicon_lost);
				this->wait = true;
			}
			else {
				SyntaxError(semicon_wrong);
			}
		}

	}
	else {
		symbol dec_type = Type();
		NextToken();
		string dec_name = "";
		if (!Match(ident)) {
			SyntaxError(ident_lost);
			this->wait = true;
		}
		else {
			dec_name = lexer.get_id();
		}
		DecTail(dec_type, dec_name);
	}
}

symbol Parser::Type() {
	switch (token)
	{
		case rev_int:return rev_int;
		case rev_char:return rev_char;
		case rev_void: return rev_void;
		case rev_string:return rev_string;
		case ident: {
			SyntaxError(type_lost);
			this->wait = true;
			break;
		}
		default: {
			SyntaxError(type_wrong);
			break;
		}
	}
	return null;
}

void Parser::DecTail(symbol type, string name){
	NextToken();
	switch (token) {
		case semicon:{
			var.Init(type, name);
			if (type == rev_string) {
				var.set_value(GLOBAL_STRING);
			}
			// table add global var
			VarTable::AddVar(var);
			break;
		}
		case lparen: {
			fun.Init(type, name);
			Parameter();
			FunTail(type, name);
			break;
		}
		default: {
			var.Init(type, name);
			if (type == rev_string) {
				var.set_value(GLOBAL_STRING);
			}
			// table add global var
			VarTable::AddVar(var);
			VarList(type);
			break;
		}
	}
}

void Parser::FunTail(symbol type, string name){
	static int level = 0;
	NextToken();
	// fun declare
	if (token == semicon) {
		// table add fun
		VarTable::AddFun(fun);
		return;
	}
	// fun
	else if (token == lbrac) {
		fun.set_defined(true);
		// table add
		VarTable::AddFun(fun);
		this->wait = true;
		Block(0, level, 0, 0);
		level = 0;
		fun.PopLocalVars(-1);
		// gen funtail
		Generator::GenerateFunTail(fun);
		return;
	}
	else if (token == ident || token == rev_if ||
		     token == rev_while || token == rev_return ||
		     token == rev_break || token == rev_continue ||
		     token == rev_in || token == rev_out ||
		     token == rbrac|| token == rev_switch) {
		SyntaxError(lbrac_lost);
		this->wait = true;
		Block(0, level, 0, 0);
		level = 0;
		return;
	}
	else if (token == rev_void || token == rev_int ||
		     token == rev_string || token == rev_char) {
		SyntaxError(semicon_lost);
		this->wait = true;
		return;
	}
	else {
		SyntaxError(semicon_wrong);
		return;
	}
}

void Parser::VarList(symbol type) {
	if (token == comma) {
		NextToken();
		if (!Match(ident)) {
			this->wait = true;
			SyntaxError(ident_lost);
		}
		else {
			string dec_name = lexer.get_id();
			var.Init(type, dec_name);
			if (type == rev_string) {
				var.set_value(GLOBAL_STRING);
			}
			// table add global var
			VarTable::AddVar(var);
		}
		NextToken();
		VarList(type);
	}
	else if (token == semicon) {
		// recursive exit
		return;
	}
	else if (token == ident) {
		SyntaxError(commma_lost);
		NextToken();
		VarList(type);
	}
	else if (token == rev_int || token == rev_char ||
		    token == rev_string || token == rev_void) {
		SyntaxError(semicon_lost);
		this->wait = true;
	}
	else {
		SyntaxError(semicon_wrong);
	}
}

void Parser::Parameter() {
	NextToken();
	switch (token) {
		case rparen:{
			break;
		}
		default: {
			symbol type = Type();
			string name = "";
			NextToken();
			if (!Match(ident)) {
				SyntaxError(para_lost);
				this->wait = true;
			}
			else {
				name = lexer.get_id();
				bool has = VarTable::HasName(name, fun);
				if (!has) {
					var.Init(type, name);
					fun.AddArgs(var);
				}
				else {
					Generator::SemanticError(para_redef, name);
				}
			}
			ParaList();
			break;
		}
	}
}

void Parser::ParaList() {
	NextToken();
	if (token == comma) {
		NextToken();
		symbol type = Type();
		string name = "";
		NextToken();
		if (!Match(ident)) {
			SyntaxError(para_lost);
			this->wait = true;
		}
		else {
			name = lexer.get_id();
			bool has = VarTable::HasName(name, fun);
			if (!has) {
				var.Init(type, name);
				fun.AddArgs(var);
			}
			else {
				Generator::SemanticError(para_redef, name);
			}
		}
		ParaList();
	}
	else if (token == lbrac || token == semicon) {
		SyntaxError(rparen_lost);
		this->wait = true;
		return;
	}
	else if (token == rparen) {
		return;
	}
	else if (token == rev_int || token == rev_void ||
		     token == rev_char || token == rev_string) {
			SyntaxError(commma_lost);
			NextToken();
			if (!Match(ident)) {
				SyntaxError(para_lost);
				this->wait = true;
			}
			ParaList();
		 }
}

void Parser::Block(int init_number, int& level, int loop_id, int addr) {
	NextToken();
	if (!Match(lbrac)) {
		SyntaxError(lbrac_lost);
		this->wait = true;
	}
	// 复合语句变量数量
	int var_number = init_number;
	level++;
	SubProgram(var_number, level, loop_id, addr);
	level--;
	fun.PopLocalVars(var_number);
}

void Parser::SubProgram(int& var_number, int& level, int loop_id, int addr) {
	NextToken();
	if (token == semicon || token == rev_while || token == rev_if ||
		token == rev_return || token == ident || token == rev_break ||
		token == rev_continue || token == rev_in || token == rev_out||
		token == rev_for || token == rev_switch) {
		Statement(var_number, level, loop_id, addr);
		SubProgram(var_number, level, loop_id, addr);
	}
	else if (token == rev_void || token == rev_int ||
		token == rev_char || token == rev_string) {
		LocalDec(var_number, level);
		SubProgram(var_number, level, loop_id, addr);
	}
	else if (token == rbrac) {
		// recursive exit
		return;
	}
	else if (token == null) {
		SyntaxError(rbrac_lost);
		return;
	}
	else {
		SyntaxError(statement_wrong);
		return;
	}
}

void Parser::LocalDec(int& var_number, int& level) {
	symbol type = Type();
	string name = "";
	NextToken();
	if (!Match(ident)) {
		SyntaxError(ident_lost);
		this->wait = true;
	}
	else {
		name = lexer.get_id();
		// table add local var
		bool has = VarTable::HasName(name, fun);
		if (!has) {
			var.Init(type, name);
			fun.PushLocalVar(var);
			var_number++;
		}
		else {
			Generator::SemanticError(local_redef, name);
		}
	}
	NextToken();
	LocalDecTail(var_number, type, level);
}

void Parser::LocalDecTail(int& var_number, symbol type, int& level) {
	if (token == comma) {
		NextToken();
		if (!Match(ident)) {
			SyntaxError(ident_lost);
			this->wait = true;
		}
		else {
			string name = lexer.get_id();
			// table add local var
			bool has = VarTable::HasName(name, fun);
			if (!has) {
				var.Init(type, name);
				fun.PushLocalVar(var);
				var_number++;
			}
			else {
				Generator::SemanticError(local_redef);
			}
		}
		NextToken();
		LocalDecTail(var_number,type,level);
	}
	else if (token == semicon) {
		return;
	}
	else if (token == assign) {
		// TODO.
		string name = lexer.get_id();
		this->wait = true;
		IdentTail(name, var_number);
		NextToken();
		LocalDecTail(var_number, type, level);
	}
	else if (token == ident) {
		SyntaxError(commma_lost);
		NextToken();
		LocalDecTail(var_number, type, level);
	}
	else if (token == rev_int || token == rev_void ||
		     token == rev_char || token == rev_string) {
		SyntaxError(semicon_lost);
		this->wait = true;
		return;
	}
	else if (token == lparen) {
		SyntaxError(rbrac_lost);
		return;
	}
	else {
		SyntaxError(semicon_wrong);
		this->wait = true;
		return;
	}
}

void Parser::Statement(int& var_number, int& level, int loop_id, int addr) {
	string name = "";
	switch (token) {
		case semicon:{
			break;
		}
		case rev_while: {
			this->break_level.push_back(0);
			WhileState(level);
			break_level.pop_back();
			break;
		}
		case rev_for: {
			this->break_level.push_back(1);
			ForState(level);
			this->break_level.pop_back();
			break;
		}
		case rev_if: {
			IfState(level, loop_id, addr);
			break;
		}
		case rev_break: {
			NextToken();
			if (token == ident || token == rev_while || token == rev_if ||
				token == rev_return || token == rev_break || token == rev_continue ||
				token == rev_in || token == rev_out || token == rbrac
				|| token == rev_for || token == rev_switch) {
				this->wait = true;
				SyntaxError(semicon_lost);
			}
			else if (token != semicon) {
				this->wait = true;
				SyntaxError(semicon_lost);
			}
			// gen code
			if (loop_id != 0) {
				Generator::GenerateBlock(addr, fun);
				if (break_level.back() == 0) {
					Generator::jmp(WHILE_EXIT(to_string(loop_id)));
				}
				else if(break_level.back() == 1){
					Generator::jmp(FOR_EXIT(to_string(loop_id)));
				}
				else if(break_level.back() == 2){
					Generator::jmp(CASE_TABLE_END(to_string(loop_id)));
				}
			}
			else {
				Generator::SemanticError(break_non_in_while);
			}
			break;
		}
		case rev_continue: {
			NextToken();
			if (token == ident || token == rev_while || token == rev_if ||
				token == rev_return || token == rev_break || token == rev_continue ||
				token == rev_in || token == rev_out || token == rbrac||
				token == rev_for|| token == rev_switch) {
				SyntaxError(semicon_lost);
				this->wait = true;
			}
			else if (token != semicon) {
				this->wait = true;
				SyntaxError(semicon_wrong);
			}
			// gen code
			if (loop_id != 0) {
				Generator::GenerateBlock(addr, fun);
				if (break_level.back() == 0) {
					Generator::jmp(WHILE_LOOP(to_string(loop_id)));
				}
				else if(break_level.back() == 1){
					Generator::jmp(FOR_ITER(to_string(loop_id)));
				}
				else {
					 SyntaxError(continue_non_switch);
				}
			}
			else {
				Generator::SemanticError(continue_non_in_while);
			}
			break;
		}
		case rev_return: {
			ReturnState(var_number, level);
			break;
		}
		case rev_in: {
			NextToken();
			if (!Match(input)) {
				SyntaxError(input_wrong);
			}
			NextToken();
			if (!Match(ident)) {
				SyntaxError(non_input);
			}
			else {
				name = lexer.get_id();
				// gen code
				Generator::GenerateInput(VarTable::GetVar(name, fun), var_number, fun);
			}
			NextToken();
			if (!Match(semicon)) {
				SyntaxError(semicon_lost);
				this->wait = true;
			}
			break;
		}
		case rev_out: {
			NextToken();
			if (!Match(output)) {
				SyntaxError(output_wrong);
			}
			// gen code
			Generator::GenerateOutput(Expr(var_number), var_number, fun);
			NextToken();
			if (!Match(semicon)) {
				SyntaxError(semicon_lost);
				this->wait = true;
			}
			break;
		}
		case rev_switch: {
			this->break_level.push_back(2);
			SwitchState(level);
			this->break_level.pop_back();
			break;
		}
		case ident: {
			name = lexer.get_id();
			IdentTail(name, var_number);
			NextToken();
			if (!Match(semicon)) {
				SyntaxError(semicon_lost);
				this->wait = true;
			}
			break;
		}
		default: {
			SyntaxError(statement_wrong);
			break;
		}
	}
}

void Parser::WhileState(int& level) {
	while_id++;
	int temp_id = while_id;
	NextToken();
	if (!Match(lparen)) {
		if (token == ident || token == number || token == lbrac||
			token == chara || token == strings || token == rparen) {
			SyntaxError(lparen_lost);
			this->wait = true;
		}
		else {
			SyntaxError(lparen_wrong);
		}
	}
	// gen condition
	Generator::label(WHILE_LOOP(to_string(temp_id)));
	int block_addr = Generator::GenerateBlock(-1, fun);
	int init = 0;
	VarRecord* condition = OrExpr(init);
	Generator::GenerateCondition(condition);
	Generator::je(WHILE_EXIT(to_string(temp_id)));

	NextToken();
	if (!Match(rparen)) {
		if (token == lbrac || token == rev_while || token == rev_if ||
			token == rev_return || token == rev_break || token == rev_continue ||
			token == semicon || token == rev_in || token == rev_out ||
			token == ident || token == rev_void || token == rev_int ||
			token == rev_string || token == rev_char || token == rev_for ||
			token == rev_switch) {
			SyntaxError(rparen_lost);
			this->wait = true;
		}
		else {
			SyntaxError(rparen_wrong);
		}
	}
	// gen code
	Block(init, level, while_id, block_addr);
	Generator::GenerateBlock(block_addr, fun);
	Generator::jmp(WHILE_LOOP(to_string(temp_id)));
	Generator::label(WHILE_EXIT(to_string(temp_id)));
	Generator::GenerateBlock(block_addr, fun);//
	return;
}

void Parser::IfState(int& level, int loop_id, int addr) {
	if_id++;
	int temp_id = if_id;
	NextToken();
	if (!Match(lparen)) {
		if (token == ident || token == number || token == chara ||
			token == strings || token == lbrac || token == rparen) {
			SyntaxError(lparen_lost);
			this->wait = true;
		}
		else {
			SyntaxError(lparen_wrong);
		}
	}
	// gen code
	int block_addr = Generator::GenerateBlock(-1,fun);
	int init = 0;
	VarRecord* condition = OrExpr(init);
	Generator::GenerateCondition(condition);
	Generator::je(IF_MIDDLE(to_string(temp_id)));
	NextToken();
	if (!Match(rparen)) {
		if (token == lbrac || token == rev_while || token == rev_if ||
			token == rev_return || token == rev_break || token == rev_continue ||
			token == semicon || token == rev_in || token == rev_out ||
			token == ident || token == rev_void || token == rev_int ||
			token == rev_string || token == rev_char || token == rev_for) {
			SyntaxError(rparen_lost);
			this->wait = true;
		}
		else {
			SyntaxError(rparen_wrong);
		}
	}
	// gen code
	Block(init, level, loop_id, block_addr);
	Generator::GenerateBlock(block_addr, fun);
	Generator::jmp(IF_END(to_string(temp_id)));
	Generator::label(IF_MIDDLE(to_string(temp_id)));
	Generator::GenerateBlock(block_addr, fun);


	NextToken();
	if (Match(rev_else)) {
		// gen code
		Block(0, level, loop_id, addr);
		Generator::GenerateBlock(block_addr, fun);
	}
	else {
		this->wait = true;
	}
	Generator::label(IF_END(to_string(temp_id)));
	Generator::GenerateBlock(block_addr, fun);//
	return;
}

void Parser::ForState(int& level) {
	for_id++;
	NextToken();
	if (!Match(lparen)) {
		if (token == ident || token == number || token == lbrac ||
			token == chara || token == strings || token == rparen) {
			SyntaxError(lparen_lost);
			this->wait = true;
		}
		else {
			SyntaxError(lparen_wrong);
		}
	}
	//
	int init_number = 0;
	ForInit(init_number, level);
	return;
}

void Parser::ForInit(int& init_number, int& level) {
	NextToken();
	
	Generator::label(FOR_START(to_string(for_id)));
	int block_addr = Generator::GenerateBlock(-1, fun);
	if (token == rev_int || token == rev_char || token == rev_string) {
		LocalDec(init_number, level);
	}
	else if (token == ident) {
		this->wait = true;
		OneExpr(init_number);
		NextToken();
	}
	if (!Match(semicon)) {
		SyntaxError(semicon_lost);
		this->wait = true;
	}
	ForCondition(init_number, level,block_addr);
}

void Parser::ForCondition(int& init_number, int& level,int addr_end) {
	NextToken();
	int temp_id = for_id;
	// code gen
	Generator::label(FOR_LOOP(to_string(temp_id)));
	int block_addr = Generator::GenerateBlock(-1, fun);

	if (token != semicon) {
		this->wait = true;
		VarRecord* condition = OrExpr(init_number);
		Generator::GenerateCondition(condition);
		Generator::je(FOR_EXIT(to_string(temp_id)));
		NextToken();
	}
	if (!Match(semicon)) {
		SyntaxError(semicon_lost);
		this->wait = true;
	}
	ForEnd(init_number, level, temp_id, block_addr,addr_end);
}

void Parser::ForEnd(int& init_number, int& level, int loop_id, int addr, int addr_end) {
	NextToken();
	if (token != rparen) {
		this->wait = true;
		Generator::for_flag = loop_id;
		OneExpr(init_number);
		NextToken();
		// Statement(var_number, level, loop_id, addr);
	}
	else {
		Generator::jmp(FOR_BLOCK(to_string(loop_id)));
		Generator::label(FOR_ITER(to_string(loop_id)));
	}
	Generator::GenerateBlock(addr, fun);
	Generator::jmp(FOR_LOOP(to_string(loop_id)));
	if (!Match(rparen)) {
		SyntaxError(rparen_lost);
		this->wait = true;
	}
	
	Generator::label(FOR_BLOCK(to_string(loop_id)));
	Block(init_number, level, for_id, addr);
	// 

	Generator::jmp(FOR_ITER(to_string(loop_id)));
	Generator::label(FOR_EXIT(to_string(loop_id)));
	Generator::GenerateBlock(addr_end, fun);//
	return;
}

void Parser::SwitchState(int& level) {
	switch_id++;
	NextToken();
	if (!Match(lparen)) {
		if (token == ident || token == number || token == lbrac ||
			token == chara || token == strings || token == rparen) {
			SyntaxError(lparen_lost);
			this->wait = true;
		}
		else {
			SyntaxError(lparen_wrong);
		}
	}
	int block_addr = Generator::GenerateBlock(-1, fun);
	int init = 0;
	VarRecord* var = Expr(init);
	Generator::jmp(CASE_TABLE(to_string(switch_id)));
	NextToken();
	if (!Match(rparen)) {
		if (token == ident || token == number || token == lbrac ||
			token == chara || token == strings || token == rparen) {
			SyntaxError(rparen_lost);
			this->wait = true;
		}
		else {
			SyntaxError(rparen_wrong);
		}
	}
	NextToken();
	if (!Match(lbrac)) {
		SyntaxError(rparen_lost);
	}
	CaseState(level, init, block_addr, var);
}

void Parser::CaseState(int& level, int& init_number, int addr, VarRecord* var) {
	NextToken();
	if (Match(rev_case)) {
		NextToken();
		if (!Match(number) && !Match(chara)) {
			SyntaxError(case_lab_error);
		}
		else {
			int case_lab = 0;
			if (token == number) {
				Generator::label(CASE(to_string(switch_id), to_string(lexer.get_digit())));
				case_lab = lexer.get_digit();
			}
			else if (token == chara) {
				Generator::label(CASE(to_string(switch_id), to_string(int(lexer.get_digit()))));
				case_lab = lexer.get_letter();
			}
			vector<int>::iterator it = find(case_number.begin(), case_number.end(), case_lab);
			if (it != case_number.end()) {
				SyntaxError(case_lab_dup);
			}
			else {
				case_number.push_back(case_lab);
			}

		}
		NextToken();
		if (!Match(colon)) {
			SyntaxError(colon_lost);
			this->wait = true;
		}
		int init = 0;
		int block_addr = Generator::GenerateBlock(-1, fun);

		Block(init, level, switch_id, block_addr);
		Generator::GenerateBlock(block_addr, fun);
		CaseState(level, init_number, addr, var);
	}
	else if(Match(rev_default)){
		Generator::label(DEFAULT(to_string(switch_id)));
		NextToken();
		if (!Match(colon)) {
			SyntaxError(colon_lost);
			this->wait = true;
		}
		this->has_default = true;
		int init = 0;
		int block_addr = Generator::GenerateBlock(-1, fun);
		
		Block(init, level, switch_id, block_addr);
		
		Generator::GenerateBlock(block_addr, fun);
		Generator::jmp(SWITCH_END(to_string(switch_id)));

		CaseState(level, init_number, addr, var);
	}
	else if (Match(rbrac)) {
		Generator::label(SWITCH_END(to_string(switch_id)));
		Generator::GenerateBlock(addr, fun);
		Generator::jmp(CASE_TABLE_END(to_string(switch_id)));
		Generator::label(CASE_TABLE(to_string(switch_id)));
		CaseHandle(var);
		Generator::label(CASE_TABLE_END(to_string(switch_id)));
		this->has_default = false;
	}
	else {
		SyntaxError(switch_error);
	}
}

void Parser::CaseHandle(VarRecord* var) {
	if (case_number.size() <= 4) {
		Generator::LoadVarAddrToReg(var);
		for (int i = 0; i < case_number.size(); i++) {
			Generator::cmp(eax, case_number[i]);
			Generator::je(CASE(to_string(switch_id), to_string(case_number[i])));
		}
		if (has_default) {
			Generator::jmp(DEFAULT(to_string(switch_id)));
		}
		else {
			Generator::jmp(SWITCH_END(to_string(switch_id)));
		}
	}
	else {
		sort(case_number.begin(), case_number.end());
		int min_number = case_number[0];
		int max_number = case_number.back();

		Generator::LoadVarAddrToReg(var);
		Generator::cmp(eax, max_number);
		Generator::jg(DEFAULT_END(to_string(switch_id)));
		Generator::cmp(eax, min_number);
		Generator::jl(DEFAULT_END(to_string(switch_id)));
		int start = 0;
		int end = case_number.size() - 1;
		Generator::GenerateCaseTable(start, end, switch_id, case_number);
		Generator::label(DEFAULT_END(to_string(switch_id)));
		if (has_default) {
			Generator::jmp(DEFAULT(to_string(switch_id)));
		}
		else {
			Generator::jmp(SWITCH_END(to_string(switch_id)));
		}
	}
}

void Parser::ReturnState(int& var_number, int& level) {
	ReturnTail(var_number, level);
	NextToken();
	if (!Match(semicon)) {
		if (token == rbrac) {
			SyntaxError(semicon_lost);
			this->wait = true;
		}
		else {
			SyntaxError(semicon_wrong);
		}
	}
}
	
void Parser::ReturnTail(int& var_number, int& level) {
	if (level == 1)
		fun.set_has_return(true);
	NextToken();
	if (token == ident || token == number || token == chara ||
		token == strings || token == lparen) {
		this->wait = true;
		VarRecord* ret = Expr(var_number);
		// semantic check
		if (ret != nullptr && (ret->get_type() != fun.get_type())) {
			// semantic error
			Generator::SemanticError(ret_type_error);
		}
		// gen code
		Generator::GenerateReturn(ret, var_number, fun);
		return;
	}
	else if (token == semicon) {
		this->wait = true;
		if (fun.get_type() != rev_void) {
			// semantic error
			Generator::SemanticError(ret_type_error, "void");
		}
		// gen code
		Generator::GenerateReturn(nullptr, var_number, fun);
		return;
	}
	else if (token == rbrac) {
		this->wait = true;
		return;
	}
	else {
		SyntaxError(return_wrong);
		return;
	}
}

VarRecord* Parser::IdentTail(string name, int& var_number) {
	NextToken();
	if (token == assign) {
		VarRecord* src = Expr(var_number);
		VarRecord* des = VarTable::GetVar(name, fun);
		// gen code
		if (des == nullptr) {
			Generator::SemanticError(null_pointer, name);
		}
		return Generator::GenerateAssign(des, src, var_number, fun);
	}
	else if (token == lparen) {
		RealArgs(name, var_number);
		// gen code
		VarRecord* ret = VarTable::GenerateCall(name, var_number, fun);
		NextToken();
		if (!Match(rparen)) {
			SyntaxError(rparen_lost);
			this->wait = true;
		}
		return ret;
	}
	// only ident
	if (ident_only) {
		ident_only = false;
		this->wait = true;
		return VarTable::GetVar(name, fun);
	}
	else {
		SyntaxError(idtail_lost);
		this->wait = true;
	}
	return nullptr;
}

void Parser::RealArgs(string name, int& var_number) {
	NextToken();
	if (token == ident || token == number || token == chara ||
		token == strings || token == lparen) {
		this->wait = true;
		// table add real args
		VarTable::AddRealArgs(Expr(var_number), var_number, fun);
		RealArgsList(var_number);
	}
	else if (token == rparen || token == semicon) {
		this->wait = true;
		return;
	}
	else if (token == comma) {
		SyntaxError(arg_lost);
		this->wait = true;
		RealArgsList(var_number);
	}
	else {
		SyntaxError(arg_wrong);
	}
}

void Parser::RealArgsList(int& var_number) {
	NextToken();
	if (token == comma) {
		NextToken();
		if (token == ident || token == number || token == chara ||
			token == strings || token == lparen) {
			this->wait = true;
			// table add real args
			VarTable::AddRealArgs(Expr(var_number), var_number, fun);
			RealArgsList(var_number);
		}
		else if (token == comma)
		{
			SyntaxError(arg_lost);
			this->wait = true;
			RealArgsList(var_number);
		}
		else if (token == semicon || token == rparen) {
			SyntaxError(arg_lost);
			this->wait = true;
		}
		else {
			SyntaxError(arg_wrong);
		}
	}
	else if (token == semicon || token == rparen) {
		this->wait = true;
		return;
	}
	else if (token == ident || token == number || token == chara ||
		     token == strings || token == lparen) {
		SyntaxError(commma_lost);
		this->wait = true;
		// todo
		Expr(var_number);
		RealArgsList(var_number);
	}
	else {
		SyntaxError(arglist_wrong);
		return;
	}
}

VarRecord* Parser::OrExpr(int& var_number) {
	VarRecord* factor1 = Expr(var_number);
	VarRecord* factor2 = OrExprTail(factor1, var_number);
	if (factor2 == nullptr)
		return factor1;
	else
		return factor2;
}

VarRecord* Parser::OrExprTail(VarRecord* factor, int& var_number) {
	NextToken();
	if (token == l_and || token == l_or) {
		symbol op = token;
		VarRecord* factor2 = Expr(var_number);
		return Generator::GenerateExp(factor, op, factor2, var_number, fun);
	}
	else {
		// todo
		this->wait = true;
		return nullptr;
	}
}

VarRecord* Parser::Expr(int& var_number) {
	VarRecord* factor1 = OneExpr(var_number);
	VarRecord* factor2 = ExprTail(factor1, var_number);
	if (factor2 == nullptr)
		return factor1;
	else
		return factor2;
}

VarRecord* Parser::ExprTail(VarRecord* factor, int& var_number) {
	NextToken();
	if (token == gt || token == ge || token == lt ||
		token == le || token == equ || token == nequ) {
		symbol cmp = token; // 
		VarRecord* factor2 = Expr(var_number);
		// gen code 
		
		return Generator::GenerateExp(factor, cmp, factor2, var_number, fun);
	}
	else if (token == ident || token == number || token == chara ||
	 	     token == strings || token == lparen) {
		SyntaxError(op_lost);
		this->wait = true;
		Expr(var_number);
	}
	else if (token == semicon || token == comma || token == rparen ||
		token == rbrac || token == rev_return || token == rev_break ||
		token == rev_continue || token == rev_in || token == rev_out ||
		token == rev_int || token == rev_char || token == rev_string ||
		token == rev_void|| token ==l_and || token == l_or) {
		this->wait = true;
		return nullptr;
	}
	else {
		NextToken();
		if (token == semicon || token == comma || token == rparen) {
			SyntaxError(op_wrong);
			this->wait = true;
			return nullptr;
		}
		else if (token == ident || token == number || token == chara ||
			token == strings || token == lparen) {
			SyntaxError(op_wrong);
			this->wait = true;
			Expr(var_number);
		}
		else {
			SyntaxError(op_wrong);
			return nullptr;
		}
	}
	return nullptr;
}

VarRecord* Parser::OneExpr(int& var_number) {
	VarRecord* factor1 = Item(var_number);
	VarRecord* factor2 = ItemTail(factor1, var_number);
	if (factor2 == nullptr)
		return factor1;
	else
		return factor2;
}

VarRecord* Parser::Item(int& var_number) {
	VarRecord* factor1 = Factor(var_number);
	VarRecord* factor2 = FactorTail(factor1, var_number);
	if (factor2 == nullptr)
		return factor1;
	else
		return factor2;
}

VarRecord* Parser::ItemTail(VarRecord* factor, int& var_number) {
	NextToken();
	if (token == add || token == sub) {
		symbol op = token;
		// gen code
		VarRecord* f2 = OneExpr(var_number);
		return Generator::GenerateExp(factor, op, f2, var_number, fun);
		
	}
	else if (token == ident || token == number || token == chara ||
		token == strings || token == lparen) {
		SyntaxError(op_lost);
		this->wait = true;
		OneExpr(var_number);
	}
	else {
		this->wait = true;
		return nullptr;
	}
	return nullptr;
}

VarRecord* Parser::Factor(int& var_number) {
	NextToken();
	VarRecord* temp_p = nullptr;
	string name = "";
	switch (token) {
		case ident: {
			ident_only = true;
			name = lexer.get_id();
			temp_p = IdentTail(name, var_number);
			break;
		}
		case number: {
			// fun
			temp_p = fun.CreateTmpVar(rev_int, true, var_number, "", lexer.get_digit());
			break;
		}
		case chara: {
			// fun
			temp_p = fun.CreateTmpVar(rev_char, true, var_number, "", lexer.get_letter());
			break;
		}
		case lparen: {
			temp_p = Expr(var_number);
			NextToken();
			if (!Match(rparen)) {
				SyntaxError(rparen_lost);
				this->wait = true;
			}
			break;
		}
		case strings: {
			// fun
			temp_p = fun.CreateTmpVar(rev_string, true, var_number, lexer.get_str(), -1);
			break;
		}
		default: {
			if (token == rparen || token == semicon || token == comma ||
				token == gt || token == ge || token == lt ||
				token == le || token == equ || token == nequ ||
				token == add || token == sub || token == mult ||
				token == divi || token == modi) {
				SyntaxError(expr_lost);
				this->wait = true;
			}
			else {
				SyntaxError(expr_wrong);
			}
			break;
		}
	}
	return temp_p;
}

VarRecord* Parser::FactorTail(VarRecord* factor, int& var_number) {
	NextToken();
	if (token == mult || token == divi || token == modi) {
		symbol op = token;
		VarRecord* factor2 = Item(var_number);
		// gen code
		return Generator::GenerateExp(factor, op, factor2, var_number, fun);
	}
	else if (token == ident || token == number || token == chara ||
		token == strings || token == lparen) {
		SyntaxError(op_lost);
		this->wait = true;
		Item(var_number);
	}
	else {
		this->wait = true;
		return nullptr;
	}
	return nullptr;
}

void Parser::SyntaxError(error_c error_code) {
	if (lexer.get_error_number() != 0) return;
	VarTable::syntax_error = true;
	syntax_error++;
	xPANIC("%s%d%s", "[syntax error at line ", lexer.get_line_number(), "] ");
	switch (error_code)
	{
		case semicon_lost: {
			xWARN("%s","semicon(;) lost!\n");
			break;
		}
		case commma_lost: {
			xWARN("%s", "comma(,) lost!\n");
			break;
		} 
		case type_lost: {
			xWARN("%s", "type(var no type) lost!\n");
			break;
		} 
		case ident_lost:{
			xWARN("%s", "ident name lost!\n");
			break;
		}
		case semicon_wrong: {
			xWARN("%s", "semicon wrong!\n");
			break;
		} 
		case type_wrong: {
			xWARN("%s", "type maybe wrong!\n");
			break;
		} 
		case statement_wrong: {
			xWARN("%s", "not an effective statement\n");
			break;
		}
		case lparen_wrong: {
			xWARN("%s", "lparen `(` wrong!\n");
			break;
		}
		case para_lost: {
			xWARN("%s", "parameter lost!\n");
			break;
		}
		case lparen_lost: {
			xWARN("%s", "lparen `(` maybe lost!\n");
			break;
		}
		case rparen_lost: {
			xWARN("%s", "rparen `)` maybe lost!\n");
			break;
		}
		case lbrac_lost: {
			xWARN("%s", "lbarc `{` maybe lost!\n");
			break;
		}
		case rbrac_lost: {
			xWARN("%s", "rbarc `}` maybe lost!\n");
			break;
		}
		case input_wrong: {
			xWARN("%s", "input stream (>>) wrong!\n");
			break;
		}
		case non_input : {
			xWARN("%s", "no var be inputed!\n");
			break;
		}
		case output_wrong: {
			xWARN("%s", "output stream (>>) wrong!\n");
			break;
		} 
		case rparen_wrong: {
			xWARN("%s", "rparen `)` maybe wrong!\n");
			break;
		}
		case else_lost: {
			xWARN("%s", "else (keyword) lost!\n");
			break;
		}
		case else_wrong: {
			xWARN("%s", "else(keyword) maybe wrong!\n");
			break;
		}
		case return_wrong: {
			xWARN("%s", "function return maybe wrong!\n");
			break;
		}
		case idtail_lost: {
			xWARN("%s", "can't resolve the identifier!\n");
			break;
		}
		case arg_lost: {
			xWARN("%s", "args maybe lost!\n");
			break;
		}
		case arg_wrong: {
			xWARN("%s", "can't resolve the args!\n");
			break;
		} 
		case arglist_wrong: {
			xWARN("%s", "no right separator in args!\n");
			break;
		}
		case op_lost: {
			xWARN("%s", " operator maybe lost!\n");
			break;
		}
		case op_wrong: {
			xWARN("%s", "operator maybe wrong!\n");
			break;
		} 
		case expr_lost: {
			xWARN("%s", "expression maybe lost!\n");
			break;
		} case expr_wrong: {
			xWARN("%s", "not an effective expression\n");
			break;
		}
		case continue_non_switch: {
			xWARN("%s", "continue can't in switch-case expression!\n");
			break;
		}
		case case_lab_error: {
			xWARN("%s", "case-lab only be constant number!\n");
			break;
		}
		case case_lab_dup: {
			xWARN("%s", "case-lab duplicated!\n");
			break;
		}
		case colon_lost: {
			xWARN("%s", "colon(:) maybe lost!\n");
			break;
		}
		case switch_error: {
			xWARN("%s", "not an effective case-block in switch statement!\n");
			break;
		}
		default:
			break;
	}
}

Parser::~Parser() {
	this->break_level.clear();
	this->case_number.clear();
	Generator::over();
}

void Parser::end() {
	Generator::section("section .bss");
	Generator::over();
	VarTable::clear();
}