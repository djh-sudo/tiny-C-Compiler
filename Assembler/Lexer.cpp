#include "Lexer.h"
#include "Generate.h"


Lexer::Lexer() {
	this->line_number = 0;
	this->current_char = ' ';
	this->old_char = ' ';
	this->fin = nullptr;
	this->num = 0;
	this->line_len = 0;
	this->position = 0;
	this->letter = 0;
	this->error_number = 0;
	this->sym = null;
	this->show_flag = false;
	memset(line, 0, MAX_LEN);
	memset(id, 0, ID_LEN + 1);
	memset(str, 0, STRING_LEN + 1);
}

Lexer::~Lexer() {
	Over();
}

bool Lexer::Init(const char* file_path) {
	fin = fopen(file_path, "r");
	if (fin) {
		cout << "file [" << file_path << "] open[r] successfully!" << endl;
		return true;
	}
	else {
		cout << "file [" << file_path << "] open failed! not exist?" << endl;
		return false;
	}
}

bool Lexer::GetChar() {
	if (position >= line_len) {
		position = 0;
		line_len = 0;
		line_number++;
		Generate::line_number++;
		current_char = ' ';
		while (current_char != '\n') {
			if (fscanf(fin, "%c", &current_char) == EOF) {
				line[line_len] = 0;
				break;
			}
			line[line_len] = current_char;
			line_len++;
			if (line_len == MAX_LEN) {
				// error
				break;
			}
		}
	}
	old_char = current_char;
	current_char = line[position];
	position++;
	if (current_char == 0)
		return false;
	else return true;
}

bool Lexer::GetSym() {
	while (current_char == ' ' || current_char == 10 || current_char == 9) {
		GetChar();
	}
	if (current_char >= 'a' && current_char <= 'z' ||
		current_char >= 'A' && current_char <= 'Z' ||
		current_char == '_' || current_char == '@' ||
		current_char == '.') {
		int id_length = 0;
		int real_length = 0;
		bool res_flag = true;
		do {
			real_length++;
			if (id_length <ID_LEN) {
				id[id_length] = current_char;
				id_length++;
			}
			res_flag = GetChar();
		} while (current_char >= 'a' && current_char <= 'z' ||
			current_char >= 'A' && current_char <= 'Z' ||
			current_char == '_' || current_char == '@' ||
			current_char == '.' || current_char >= '0' && current_char <= '9');
		id[id_length] = 0;
		if (real_length > ID_LEN) {
			// error

		}
		CheckReserved();
		if (show_flag) sp(0);
		return res_flag;
	}
	else if (current_char >= '0' && current_char <= '9') {
		sym = number;
		int num_length = 0;
		num = 0;
		int real_length = 0;
		bool res_flag = true;
		do {
			real_length++;
			if (num_length < NUM_LENGTH) {
				num = 10 * num + (current_char - '0');
				num_length++;
			}
			res_flag = GetChar();
		} while (current_char >= '0' && current_char <= '9');
		if (real_length > NUM_LENGTH) {
			// error
		}
		if (show_flag) sp(1);
		return res_flag;
	}
	else {
		int str_length = 0;
		bool res_flag = true;
		int real_length = 0;
		switch (current_char) {
			case '+': {
				sym = addi;
				GET_CHAR
				if (show_flag) sp(3,"+");
				break;
			}
			case '-': {
				sym = subs;
				GET_CHAR
				if (show_flag) sp(3, "-");
				break;
			}
			case ':': {
				sym = colon;
				GET_CHAR
				if (show_flag) sp(3, ":");
				break;
			}
			case ';': {
				sym = null;
				GET_CHAR
				while (current_char != '\n') {
					GET_CHAR
				}
				GET_CHAR
				break;
			}
			case ',': {
				sym = comma;
				GET_CHAR
				if (show_flag) sp(3, ",");
				break;
			}
			case'"': {
				sym = null;
				res_flag = GetChar();
				real_length = 0;
				while (current_char != '"') {
					real_length++;
					if (str_length < STRING_LEN) {
						str[str_length] = current_char;
						str_length++;
					}
					res_flag = GetChar();
					if (!res_flag) {
						// error
						return false;
					}
				}
				str[str_length] = 0;
				if (real_length > STRING_LEN) {
					// error
				}
				sym = strings;
				GET_CHAR
				if (show_flag) sp(2);
				break;
			}
			case '[': {
				sym = lbrac;
				GET_CHAR
				if (show_flag) sp(3, "[");
				break;
			}
			case']': {
				sym = rbrac;
				GET_CHAR
				if (show_flag) sp(3, "]");
				break;
			}
			case 0: {
				sym = null;
				return false;
			}
			default: {
				// error
				sym = excep;
				GET_CHAR
				break;
			}
		}
	}
	return true;
}

void Lexer::CheckReserved() {
	for (int i = 0; i < RESERVED_NUMBER; i++) {
		if (strcmp(id, reserved_table[i]) == 0) {
			sym = reserved_symbol[i];
			return;
		}
	}
	sym = ident;
	return;
}

void Lexer::Over() {
	if (fin) {
		fclose(fin);
		fin = nullptr;
	}
	return;
}

char Lexer::get_current_char() {
	return current_char;
}

char Lexer::get_old_char() {
	return old_char;
}

int Lexer::get_line_number() {
	return line_number;
}

string Lexer::get_line() {
	return line;
}

int Lexer::get_position() {
	return position;
}

int Lexer::get_line_len() {
	return line_len;
}

string Lexer::get_id() {
	return id;
}

symbol Lexer::get_symbol() {
	return sym;
}

int Lexer::get_num() {
	return num;
}

char Lexer::get_letter() {
	return letter;
}

string Lexer::get_str() {
	return str;
}

int Lexer::get_error_number() {
	return error_number;
}

void Lexer::set_flag(bool show_flag) {
	this->show_flag = show_flag;
}

void Lexer::sp(int mode, string s) {
	cout << "line:" << line_number << " pos:" << position << " symbol is ";
	switch (mode) {
		case 0: {
			cout << id << endl;
			break;
		}
		case 1: {
			cout << num << endl;
			break;
		}
		case 2: {
			cout << str << endl;
			break;
		}
		default: {
			cout << s << endl;
			break;
		}
	}
}