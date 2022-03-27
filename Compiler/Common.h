#pragma once
#include <Windows.h>
#include <cstdio>
#include <iostream>
using namespace std;

#define ID_LEN 30
#define NUM_LEN 9
#define STRING_LEN 255
#define MAX_LEN 80
#define RESERVED_NUMBER 17
#define GLOBAL_STRING -2
#define DYNAMIC_STRING -1

#define al "al"
#define bl "bl"
#define cl "cl"
#define dl "dl"
#define eax "eax"
#define ebx "ebx"
#define ecx "ecx"
#define edx "edx"
#define edi "edi"
#define esi "esi"
#define esp "esp"
#define ebp "ebp"

#define ESP "@s_esp"
#define EBP "@s_ebp"
#define STACK "@s_stack"
#define BASE "@s_base"
#define STR2LONG "@str2long"
#define PROC_BUF "@procBuf"
#define STR2LONG_DATA_LEN "@str2long_data_len"
#define STR2LONG_DATA "@str2long_data"
#define BUFFER "@buffer"
#define BUFFER_LEN "@buffer_len"
#define CAL_BUF_LEN "@cal_buf_len"
#define CAL_BUF_LEN_EXIT "@cal_buf_len_exit"
#define _START "_start"

#define __(X)             string("[") + X + "]"
#define _(X,Y)            string("[") + X + "+" + Y + "]"
#define _ebp(X)           string("[") + ebp + "+" + to_string(X) + "]" 
#define __ebp(X)          string("[") + ebp + to_string(X) + "]"
#define WHILE_LOOP(X)     string("@while_") + X + "_loop"
#define WHILE_EXIT(X)     string("@while_") + X + "_exit"
#define IF_MIDDLE(X)      string("@if_") + X + "_middle"
#define IF_END(X)         string("@if_") + X + "_end"
#define FOR_START(X)      string("@for_") + X + "_start"
#define FOR_LOOP(X)       string("@for_") + X + "_loop"
#define FOR_EXIT(X)       string("@for_") + X + "_exit"
#define FOR_ITER(X)       string("@for_") + X + "_iter"
#define FOR_BLOCK(X)      string("@for_") + X + "_blcok"

#define CASE(X,Y)         string("@case_") + X + "_" + Y  + "_lab"
#define SWITCH_END(X)     string("@switch_") + X + "_end"
#define DEFAULT(X)        string("@default_") + X + "_lab"
#define DEFAULT_END(X)    string("@default_") + X + "_end"
#define CASE_JMP(X,Y)     string("@case_") + X + "_" + Y + "_jmp"
#define CASE_TABLE(X)     string("@switch_") + X + "_table"
#define CASE_TABLE_END(X) string("@switch_tab_") + X + "_end"

//
// aux routines for colorful printf. And you should not pay more attention to them
//
#define xINFO(_X_,...)            color_printf(FG_WHITE,_X_,__VA_ARGS__)

#define xPANIC(_X_,...)           color_printf(FG_RED,_X_,__VA_ARGS__)

#define xSUCC(_X_,...)            color_printf(FG_GREEN,_X_,__VA_ARGS__)

#define xWARN(_X_,...)            color_printf(FG_YELLOW,_X_,__VA_ARGS__)


#define FG_BLACK               0x00
#define FG_BLUE                0x01
#define FG_GREEN               0x02
#define FG_BLUE_2              0x03
#define FG_RED                 0x04
#define FG_PURPLE              0x05
#define FG_YELLOW              0x06
#define FG_WHITE               0x0F

inline void setColor(int color, const bool bIntensity = true) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color | (bIntensity ? FOREGROUND_INTENSITY : 0));
}

inline void resetColor() {
	setColor(FG_WHITE);
}

inline int color_printf(const int color, const char* pszFmt, ...) {
	if (color != -1) {
		setColor(color);
	}
	va_list  marker;
	va_start(marker, pszFmt);
	auto ri = vprintf(pszFmt, marker);
	va_end(marker);
	resetColor();
	return ri;
}

// 41
enum symbol {
	null,ident,excep,number,

	add,sub,mult,divi,modi,assign,
	// >£¬>=£¬<£¬<=£¬==£¬!=
	gt,ge,lt,le,equ,nequ,l_and,l_or,
	// input >>£¬output <<
	input,output,
	// ½ç·û£¬char,string,;,(,),{,}
	comma,chara,strings,semicon,lparen,rparen,lbrac,rbrac, colon,
	min,
	// reserved words
	rev_break,rev_case,rev_char,rev_continue,rev_default,rev_else,rev_extern,rev_for,
	rev_if,rev_in,rev_int,rev_out,
	rev_return,rev_string,rev_switch,rev_void,rev_while,
	// end
	max
};


// 
enum error_c {
	// lexer error
	char_wrong,string_wrong, annotation_wrong,line2long,
	str2long,num2long,id2long,expec_char,warn_escape,
	// grammar error
	semicon_lost,commma_lost,type_lost,ident_lost,
	semicon_wrong,type_wrong, statement_wrong,lparen_wrong,
	para_lost, lparen_lost,rparen_lost,lbrac_lost,rbrac_lost,
	input_wrong, non_input,output_wrong, rparen_wrong, else_lost,
	else_wrong, return_wrong, idtail_lost, arg_lost, arg_wrong, arglist_wrong,
	op_lost, op_wrong, expr_lost, expr_wrong,

	// semantic error
	void_non_calc,str_non_add, void_non_assi, type_assi, void_non_in, 
	var_redef, var_undef, fun_redef, fun_def_error, fun_undef, fun_dec_error,
	real_args_error, para_redef, local_redef,
	break_non_in_while, continue_non_in_while,
	str_non_cond, void_non_cond, ret_type_error, null_pointer,
	continue_non_switch, case_lab_error, case_lab_dup, colon_lost,
	switch_error, error_operator,


};