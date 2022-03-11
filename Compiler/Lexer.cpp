#include <iostream>
#include "Lexer.h"
#include "Generator.h"

Lexer::Lexer() {
	this->current_char = ' ';
	this->old_char = ' ';
	this->line_number = 0;
	this->position = 0;
	this->line_length = 0;
	this->letter = ' ';
	this->digit = 0;
	memset(this->line, 0, MAX_LEN);
	memset(this->id, 0, ID_LEN + 1);
	memset(this->str, 0, STRING_LEN);
	this->fin = nullptr;
	this->show_flag = false;
	this->error_number = 0;
}

bool Lexer::Init(const char* file_name) {
	fin = fopen(file_name, "r");
	if (fin != nullptr) {
		cout << "file [" << file_name << "] open successfully!" << endl;
		return true;
	}
	else {
		cout << "file [" << file_name << "] open failed!" << endl;
		return false;
	}
}

bool Lexer::Getchar() {
	if (position >= line_length) {
		// 读入新的一行
		position = 0;
		line_length = 0;
		line_number++;
		Generator::line_number++;
		current_char = ' ';
		while (current_char != '\n') {
			if (fscanf(fin, "%c", &current_char) == EOF) {
				line[line_length] = 0;
				break;
			}
			line[line_length] = current_char;
			line_length++;
			if (line_length == MAX_LEN) {
				lexical_error(line2long);
				break;
			}
		}
	}

	old_char = current_char;
	current_char = line[position];
	position++;
	if (current_char == 0) return false;
	else return true;
}

bool Lexer::GetSymbol() {
	while (current_char == ' ' || current_char == '\n' || current_char == 9) {
		Getchar();
	}
	if (current_char >= 'a' && current_char <= 'z' ||
		current_char >= 'A' && current_char <= 'Z' ||
		current_char == '_') {
		int id_count = 0;    // 标识符有效长度
		int real_length = 0; // 标识符实际长度
		bool result = false;
		do {
			real_length++;
			if (id_count < ID_LEN) {
				id[id_count] = current_char;
				id_count++;
			}
			result = Getchar();
		} while (current_char >= 'a' && current_char <= 'z' ||
			     current_char >= 'A' && current_char <= 'Z' ||
			     current_char >= '0' && current_char <= '9' ||
			     current_char == '_');
		id[id_count] = 0;
		if (real_length > ID_LEN) {
			lexical_error(id2long);
		}
		CheckReserved();
		if (show_flag) sp(0);
		return result;
	}
	else if (current_char >= '0' && current_char <= '9') {
		sym = number;
		int num_count = 0;
		int real_length = 0;
		bool result = false;
		digit = 0;
		do{
			real_length++;
			if (num_count < NUM_LEN) {
				digit = 10 * digit + (current_char - '0');
				num_count++;
			}
			result = Getchar();
		}while (current_char >= '0' && current_char <= '9');
		if (real_length > NUM_LEN) {
			lexical_error(num2long);
		}
		if (show_flag) sp(3);
		return result;
	}
	else {
		int str_count = 0;
		int real_length = 0;
		bool result = false;
		switch (current_char) {
		    case '+':{
		    	sym = add;
				GETCHAR
				if (show_flag) sp(4, "+");
		    	break;
		    }
		    case '-': {
		    	sym = sub;
		    	GETCHAR
				if (show_flag) sp(4, "-");
		    	break;
		    }
			case '*': {
				sym = mult;
				GETCHAR
				if (show_flag) sp(4, "*");
				break;
			}
			case '/': {
				sym = divi;
				GETCHAR
				if (current_char == '/') {
					sym = null;
					while (current_char != '\n') {
						GETCHAR
					}
					GETCHAR
				}
				else if (current_char == '*') {
					sym = null;
					do {
						result = Getchar();
						if (!result && current_char == 0
							&& old_char == ' ') {
							lexical_error(annotation_wrong);
							return false;
						}
						if (current_char == '*') {
							result = Getchar();
							if (!result && current_char == 0
								&& old_char == ' ') {
								lexical_error(annotation_wrong);
								return false;
							}
							if (current_char == '/') break;
						}
					} while (true);
					GETCHAR
				}
				else if (show_flag) sp(4, "/");
				break;
			}
			case '>': {
				sym = gt;
				GETCHAR
				if (current_char == '=') {
					sym = ge;
					if (show_flag) sp(4, ">=");
					GETCHAR
				}
				else if (current_char == '>') {
					sym = input;
					if (show_flag) sp(4, ">>");
					GETCHAR
				}
				else if (show_flag) sp(4, ">");
				break;
			}
			case '<': {
				sym = lt;
				GETCHAR
				if(current_char == '='){
					sym = le;
					if (show_flag) sp(4, "<=");
					GETCHAR
				}
				else if (current_char == '<') {
					sym = output;
					if (show_flag) sp(4, "<<");
					GETCHAR
				}
				else if (show_flag) sp(4, "<");
				break;
			}
			case '=': {
				sym = assign;
				GETCHAR
				if(current_char == '='){
					sym = equ;
					if (show_flag) sp(4, "==");
					GETCHAR
				}
				if (show_flag) sp(4, "=");
				break;
			}
			case '!': {
				sym = null;
				GETCHAR
				if(current_char == '='){
					sym = nequ;
					if (show_flag) sp(4, "!=");
					GETCHAR
				}
				break;
			}
			case ';': {
				sym = semicon;
				GETCHAR
					if (show_flag) sp(4, ";");
				break;
			}
			case ',': {
				sym = comma;
				if (show_flag) sp(4, ",");
				GETCHAR
				break;
			}
			case '\'': {
				sym = null;
				result = Getchar();
				if (!result) {
					lexical_error(char_wrong);
					return false;
				}
				letter = current_char;
				result = Getchar();
				if (!result) {
					lexical_error(char_wrong);
					return false;
				}
				if (current_char == '\'') {
					sym = chara;
					if (show_flag) sp(2);
					GETCHAR
				}
				else {
					lexical_error(char_wrong);
					return false;
				}
				break;
			}
			case '"': {
				sym = null;
				result = Getchar();
				if (!result) {
					lexical_error(string_wrong);
					return false;
				}
				real_length = 0;
				str_count = 0;
				while (current_char != '"') {
					real_length++;
					if (str_count < STRING_LEN) {
						if (current_char == '\\') {
							result = Getchar();
							if (!result) {
								lexical_error(string_wrong);
								return false;
							}
							switch (current_char) {
							case 'n': str[str_count] = 10; break;
							case 't':str[str_count] = 9; break;
							default:str[str_count] = current_char; break;
								lexical_error(warn_escape);
							}
						}
						else if (current_char == '\n') {
							lexical_error(string_wrong);
							return false;
						}
						else {
							str[str_count] = current_char;
						}
						str_count++;
					}
					result = Getchar();
					if (!result) {
						lexical_error(string_wrong);
						return false;
					}
				}
				str[str_count] = 0;
				if (real_length > STRING_LEN) {
					lexical_error(str2long);
				}
				sym = strings;
				if (show_flag) sp(1);
				GETCHAR
				break;
			}
			case '(': {
				sym = lparen;
				if (show_flag) sp(4, "(");
				GETCHAR
				break;
			}
			case ')': {
				sym = rparen;
				if (show_flag) sp(4, ")");
				GETCHAR
				break;
			}
			case '{': {
				sym = lbrac;
				if (show_flag) sp(4, "{");
				GETCHAR
				break;
			}
			case '}': {
				sym = rbrac;
				if (show_flag) sp(4, "}");
				GETCHAR
				break;
			}
			case 0: {
				sym = null;
				return false;
			}
			default: {
				sym = excep;
				lexical_error(expec_char);
				GETCHAR
				break;
			}
		}
	}
	return true;
}

void Lexer::CheckReserved() {
	int i = 0;
	int j = RESERVED_NUMBER - 1;
	int k = 0;
	for (; i <= j;) {
		k = (i + j) / 2;
		if (strcmp(id, reserved_table[k]) < 0) j = k - 1;
		else if (strcmp(id, reserved_table[k]) > 0) i = k + 1;
		else {
			sym = reserved_symbol[k];
			break;
		}
	}
	if (i > j) sym = ident;
	return;
}

void Lexer::over() {
	if (fin) {
		fclose(fin);
		fin = nullptr;
	}
	return;
}

// other help API
void Lexer::sp(int mode,string content) {
	cout << "line:" << line_number << " pos:" << position << "[" << symbol_name[sym] << "] is ";
	switch (mode) {
		case 0: {
			cout << id << endl;
			break;
		}
		case 1: {
			cout << str << endl;
			break;
		}
		case 2: {
			cout << letter << endl;
			break;
		}
		case 3: {
			cout << digit << endl;
			break;
		}
		default: {
			cout << content << endl;
			break;
		}
	}
}

void Lexer::lexical_error(enum error_c code) {
	Generator::error = true;
	if (code == warn_escape) {
		cout << "warning[lexical warning at" << line_number << "] escape failed with \\" << current_char << endl;
		return;
	}
	error_number++;
	cout << "[lexical error at " << line_number << "] ";
	switch (code) {
		case char_wrong:{
			cout << "unrecognized char,miss ending '" << endl; 
			break;
		}
		case string_wrong: {
			cout << "unrecognized string,miss ending \"" << endl;
			break;
		}
		case annotation_wrong: {
			cout << "annotation wrong missing ending * or /" << endl;
			break;
		}
		case line2long: {
			cout << "line over 80 charactors!" << endl;
			break;
		}
		case str2long: {
			cout << "string " << str << " is too long(over 255 byte)" << endl;
			break;
		}
		case num2long: {
			cout << "number " << digit << " is too long(over 999999999)" << endl;
			break;
		}
		case id2long: {
			cout << "ident " << id << " is too long(over 30 byte)" << endl;
			break;
		}
		case expec_char: {
			cout << "Unexpected symbol " << current_char << endl;
			break;
		}
	}
}
// default get API
char Lexer::get_current_char() {
	return current_char;
}

char Lexer::get_old_char() {
	return old_char;
}

int Lexer::get_line_number() {
	return line_number;
}

int Lexer::get_position() {
	return position;
}

int Lexer::get_line_length() {
	return line_length;
}

symbol Lexer::get_sym() {
	return sym;
}

string Lexer::get_id() {
	return id;
}

string Lexer::get_str() {
	return str;
}

char Lexer::get_letter() {
	return letter;
}

int Lexer::get_digit() {
	return digit;
}

bool Lexer::get_show_flag() {
	return show_flag;
}

int Lexer::get_error_number() {
	return error_number;
}
// default set API

void Lexer::set_show_flag(bool show_flag) {
	this->show_flag = show_flag;
}

Lexer::~Lexer() {
	over();
}