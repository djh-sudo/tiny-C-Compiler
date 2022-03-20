#pragma once
#include <string>
#include <iostream>
#include "Common.h"
#define GET_CHAR if(!GetChar()) return false;

using namespace std;


static enum symbol reserved_symbol[RESERVED_NUMBER]{
	rev_al, rev_cl, rev_dl, rev_bl,
	rev_ah, rev_ch, rev_dh, rev_bh,
	rev_eax, rev_ecx, rev_edx, rev_ebx,
	rev_esp, rev_ebp, rev_esi, rev_edi,
	rev_mov, rev_cmp, rev_sub, rev_add,
	rev_lea,rev_call, rev_sys_int, rev_imul,
	rev_idiv,rev_neg, rev_inc, rev_dec, rev_jmp,
	rev_je, rev_jg, rev_jl, rev_jge,
	rev_jle, rev_jne, rev_jna, rev_push,
	rev_pop, rev_ret, rev_section, rev_global,
	rev_equ, rev_times, rev_db, rev_dw,rev_dd
};

static char reserved_table[RESERVED_NUMBER][ID_LEN] = {
	al,cl,dl,bl,
	ah,ch,dh,bh,
	eax,ecx,edx,ebx,
	esp	,ebp,esi,edi,
	mov,cmp,sub,add,
	lea,call,sys_int,imul,
	idiv,neg,inc,dec,jmp,
	je ,jg, jl ,jge,
	jle,jne,jna,_push,
	_pop,_ret,_section,
	global,equ,times,
	db,dw,dd
};

class Lexer {
	
private:			 
	char current_char;
	char old_char;
	int line_number;
	char line[MAX_LEN];
	int position;
	int line_len;

	enum symbol sym;
	char id[ID_LEN + 1];
	int num;
	char str[STRING_LEN + 1];
	char letter;
	int error_number;
	FILE* fin;
	bool show_flag;

public:
	Lexer();
	~Lexer();
	bool Init(const char* file_name);
	void Over();
	bool GetChar();
	bool GetSym();
	void CheckReserved();
	// error handle
	void sp(int mode, string s = "");
	// void LexicalError(enum error_c);
	// set API
	char get_current_char();
	char get_old_char();
	int get_line_number();
	string get_line();
	int get_position();
	int get_line_len();
	string get_id();
	symbol get_symbol();
	int get_num();
	char get_letter();
	string get_str();
	int get_error_number();
	// set API
	void set_flag(bool show_flag);
};

