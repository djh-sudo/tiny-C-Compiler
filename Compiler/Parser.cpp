#include <string>
#include "Parser.h"
#include "Generator.h"


Parser::Parser(){
	this->wait = false;
	this->old_token = null;
	this->token = null;
	this->if_id = 0;
	this->while_id = 0;
	this->syntax_error = 0;
	this->compiler_ok = false;
	this->ident_only = false;
}


void Parser::Init(const char* file_name) {
	bool result = lexer.Init(file_name);
	if (result) {
		cout << "Lexer initial successfully!" << endl;
		result = Generator::GenerateComm();
		if (result) {
			int index = string(file_name).find_last_of(".");
			string output = string(file_name).substr(0, index);
			result = Generator::Init((output + ".s").c_str());
			if (result) {
				cout << "Generator initial successfully!" << endl;
				Generator::section("section .text");
			}
			else {
				cout << "Generator initial failed!" << endl;
				exit(1);
			}
		}
		else {
			cout << "Generator common file failed!" << endl;
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
			cout << "compiler successfully! " << endl;
		}
		else {
			cout << "compiler failed!" << endl;
		}
		cout << "lexical error:" << lexer.get_error_number() << endl;
		cout << "syntax error:" << syntax_error << endl;
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
		     token == rbrac) {
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
	// ��������������
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
		token == rev_continue || token == rev_in || token == rev_out) {
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
			WhileState(var_number, level);
			break;
		}
		case rev_if: {
			IfState(var_number, level, loop_id, addr);
			break;
		}
		case rev_break: {
			NextToken();
			if (token == ident || token == rev_while || token == rev_if ||
				token == rev_return || token == rev_break || token == rev_continue ||
				token == rev_in || token == rev_out || token == rbrac) {
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
				Generator::jmp(WHILE_EXIT(to_string(loop_id)));
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
				token == rev_in || token == rev_out || token == rbrac) {
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
				Generator::jmp(WHILE_LOOP(to_string(loop_id)));
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

void Parser::WhileState(int& var_number, int& level) {
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
	VarRecord* condition = Expr(init);
	Generator::GenerateCondition(condition);
	Generator::je(WHILE_EXIT(to_string(temp_id)));

	NextToken();
	if (!Match(rparen)) {
		if (token == lbrac || token == rev_while || token == rev_if ||
			token == rev_return || token == rev_break || token == rev_continue ||
			token == semicon || token == rev_in || token == rev_out ||
			token == ident || token == rev_void || token == rev_int ||
			token == rev_string || token == rev_char) {
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
	return;
}

void Parser::IfState(int& var_number, int& level, int loop_id, int addr) {
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
	VarRecord* condition = Expr(init);
	Generator::GenerateCondition(condition);
	Generator::je(IF_MIDDLE(to_string(temp_id)));
	NextToken();
	if (!Match(rparen)) {
		if (token == lbrac || token == rev_while || token == rev_if ||
			token == rev_return || token == rev_break || token == rev_continue ||
			token == semicon || token == rev_in || token == rev_out ||
			token == ident || token == rev_void || token == rev_int ||
			token == rev_string || token == rev_char) {
			SyntaxError(rparen_lost);
			this->wait = true;
		}
		else {
			SyntaxError(rparen_wrong);
		}
	}
	// gen code
	Block(init, level, if_id, block_addr);
	Generator::GenerateBlock(block_addr, fun);
	Generator::jmp(IF_END(to_string(temp_id)));
	Generator::label(IF_MIDDLE(to_string(temp_id)));
	Generator::GenerateBlock(block_addr, fun);


	NextToken();
	if (!Match(rev_else)) {
		if (token == lbrac || token == semicon || token == rev_while ||
			token == rev_if || token == rev_return || token == rev_break ||
			token == rev_continue || token == rev_in || token == rev_out ||
			token == rev_void || token == rev_int || token == rev_char ||
			token == rev_string) {
			SyntaxError(else_lost);
		}
		else {
			SyntaxError(else_wrong);
		}
	}

	// gen code

	Block(0, level, loop_id, block_addr);
	Generator::GenerateBlock(block_addr, fun);
	Generator::label(IF_END(to_string(temp_id)));
	return;
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
		// ������
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
		token == rev_int || token == rev_char || token == strings ||
		token == rev_void) {
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
			// todo
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
				token == divi) {
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
	if (token == mult || token == divi) {
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
	cout << "[syntax error at line" << lexer.get_line_number() << "] ";
	switch (error_code)
	{
		case semicon_lost: {
			cout << "semicon(;) lost!" << endl;
			break;
		}
		case commma_lost: {
			cout << "comma(,) lost!" << endl;
			break;
		} 
		case type_lost: {
			cout << "type(var no type) lost!" << endl;
			break;
		} 
		case ident_lost:{
			cout << "ident name lost!" << endl;
			break;
		}
		case semicon_wrong: {
			cout << "semicon wrong!" << endl;
			break;
		} case type_wrong: {
			cout << "type maybe wrong!" << endl;
			break;
		} 
		case statement_wrong: {
			cout << "not an effective statement" << endl;
			break;
		}
		case lparen_wrong: {
			cout << "lparen `(` wrong!" << endl;
			break;
		}
		case para_lost: {
			cout << "parameter lost!" << endl;
			break;
		}
		case lparen_lost: {
			cout << "lparen `(` maybe lost!" << endl;
			break;
		}
		case rparen_lost: {
			cout << "rparen `)` maybe lost!" << endl;
			break;
		}
		case lbrac_lost: {
			cout << "lbarc `{` maybe lost!" << endl;
			break;
		}
		case rbrac_lost: {
			cout << "rbarc `}` maybe lost!" << endl;
			break;
		}
		case input_wrong: {
			cout << "input stream (>>) wrong!" << endl;
			break;
		}
		case non_input : {
			cout << "no var be inputed!" << endl;
			break;
		}
		case output_wrong: {
			cout << "output stream (>>) wrong!" << endl;
			break;
		} 
		case rparen_wrong: {
			cout << "rparen `)` maybe wrong!" << endl;
			break;
		}
		case else_lost: {
			cout << "else (keyword) lost!" << endl;
			break;
		}
		case else_wrong: {
			cout << "else(keyword) maybe wrong!" << endl;
			break;
		}
		case return_wrong: {
			cout << "function return maybe wrong!" << endl;
			break;
		}
		case idtail_lost: {
			cout << "can't resolve the identifier!" << endl;
			break;
		}
		case arg_lost: {
			cout << "args maybe lost!" << endl;
			break;
		}
		case arg_wrong: {
			cout << "can't resolve the args!" << endl;
			break;
		} 
		case arglist_wrong: {
			cout << "no right separator in args!" << endl;
			break;
		}
		case op_lost: {
			cout << " operator maybe lost!" << endl;
			break;
		}
		case op_wrong: {
			cout << "operator maybe wrong!" << endl;
			break;
		} 
		case expr_lost: {
			cout << "expression maybe lost!" << endl;
			break;
		} case expr_wrong: {
			cout << "not an effective expression" << endl;
			break;
		}
		default:
			break;
	}
}

Parser::~Parser() {
	Generator::over();
}

void Parser::end() {
	Generator::section("section .bss");
	Generator::over();
	VarTable::clear();
}