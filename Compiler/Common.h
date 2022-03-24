#pragma once
#include <iostream>
using namespace std;

#define ID_LEN 30
#define NUM_LEN 9
#define STRING_LEN 255
#define MAX_LEN 80
#define RESERVED_NUMBER 14
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

#define __(X) string("[") + X + "]"
#define _(X,Y) string("[") + X + "+" + Y + "]"
#define _ebp(X) string("[") + ebp + "+" + to_string(X) + "]" 
#define __ebp(X) string("[") + ebp + to_string(X) + "]"
#define WHILE_LOOP(X) string("@while_") + X + "_loop"
#define WHILE_EXIT(X) string("@while_") + X + "_exit"
#define IF_MIDDLE(X) string("@if_") + X + "_middle"
#define IF_END(X) string("@if_") + X + "_end"
#define FOR_LOOP(X) string("@for_") + X + "_loop"
#define FOR_EXIT(X) string("@for_") + X + "_exit"
#define FOR_ITER(X) string("@for_") + X + "_iter"
#define FOR_BLOCK(X) string("@for_") + X + "_blcok"
// 41个枚举符号
enum symbol {
	// 空，标识符，异常，数字
	null,ident,excep,number,
	// 加，减，乘，除，赋值
	add,sub,mult,divi,modi,assign,
	// >，>=，<，<=，==，!=
	gt,ge,lt,le,equ,nequ,
	// 输入>>，输出<<
	input,output,
	// 界符，char,string,;,(,),{,}
	comma,chara,strings,semicon,lparen,rparen,lbrac,rbrac,
	min,
	// 保留字
	rev_break,rev_char,rev_continue,rev_else,rev_extern,rev_for,
	rev_if,rev_in,rev_int,rev_out,
	rev_return,rev_string,rev_void,rev_while,
	// end
	max
};


// 
enum error_c {
	// 词法错误
	char_wrong,string_wrong, annotation_wrong,line2long,
	str2long,num2long,id2long,expec_char,warn_escape,
	// 语法错误
	semicon_lost,commma_lost,type_lost,ident_lost,
	semicon_wrong,type_wrong, statement_wrong,lparen_wrong,
	para_lost, lparen_lost,rparen_lost,lbrac_lost,rbrac_lost,
	input_wrong, non_input,output_wrong, rparen_wrong, else_lost,
	else_wrong, return_wrong, idtail_lost, arg_lost, arg_wrong, arglist_wrong,
	op_lost, op_wrong, expr_lost, expr_wrong,

	//语义错误
	void_non_calc,str_non_add, void_non_assi, type_assi, void_non_in, 
	var_redef, var_undef, fun_redef, fun_def_error, fun_undef, fun_dec_error,
	real_args_error, para_redef, local_redef,
	break_non_in_while, continue_non_in_while,
	str_non_cond, void_non_cond, ret_type_error, null_pointer,


};