#pragma once
#include <cstdio>
#include <string.h>
#include "Common.h"


#define GETCHAR if(!Getchar()) return false;

static char symbol_name[][ID_LEN]{
	// �գ���ʶ�����쳣������
	"null", "ident", "excep", "number",
	// �ӣ������ˣ�������ֵ
	"add", "sub", "mult", "divi", "assign",
	// >��>=��<��<=��==��!=
	"gt", "ge", "lt", "le", "equ", "nequ",
	// ����>>�����<<
	"input", "output",
	// �����char,string,;,(,),{,}
	"comma", "chara", "strings", "semicon", "lparen", "rparen", "lbrac", "rbrac",
	// ������
	"min",
	"rev_break", "rev_char", "rev_continue", "rev_else", "rev_extern","rev_for",
	"rev_if", "rev_in", "rev_int", "rev_out",
	"rev_return", "rev_string", "rev_void", "rev_while",
	// end
	"max"
};

static char reserved_table[RESERVED_NUMBER][ID_LEN] = {
	"break","char","continue","else","extern","for","if",
	"in","int","out","return","string","void","while"
};
static enum symbol reserved_symbol[RESERVED_NUMBER] = {
	rev_break,rev_char,rev_continue,rev_else,rev_extern,rev_for,
	rev_if,rev_in,rev_int,rev_out,
	rev_return,rev_string,rev_void,rev_while
};


class Lexer
{
private:
	char current_char;   // current char
	char old_char;       // old char
	int line_number;     // line number
	char line[MAX_LEN];  // �洢��ǰ�е��ַ���
	int position;        // �к�
	int line_length;     // ��ǰ�еĳ���

	enum symbol sym;     // ��������
	char id[ID_LEN + 1]; // ��ʶ��
	char str[STRING_LEN];// �ַ���
	char letter;         // �ַ�
	int digit;           // ����

	FILE* fin;           // �ļ���ʶ
	bool show_flag;      // 
	int error_number;    // error number

public:
	explicit Lexer();
	~Lexer();
	// basic API
	bool Init(const char* file_name);
	void over();
	bool Getchar();
	bool GetSymbol();
	void CheckReserved();
	// other help API
	void sp(int mode, string c = ""); // output token sequence
	void lexical_error(enum error_c code);
	// default get API
	char get_current_char();
	char get_old_char();
	int get_line_number();
	int get_position();
	int get_line_length();
	symbol get_sym();
	string get_id();
	string get_str();
	char get_letter();
	int get_digit();
	bool get_show_flag();
	int get_error_number();
	// default set API 
	void set_show_flag(bool show_flag);
	//
};

