#pragma once
#include <iostream>
using namespace std;

#define ID_LEN 30
#define NUM_LEN 9
#define STRING_LEN 255
#define MAX_LEN 80
#define RESERVED_NUMBER 13
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

#define ESP "@_esp"
#define EBP "@_ebp"
#define STACK "@stack"
#define BASE "@base"
#define STR2LONG "@str2long"
#define PROC_BUF "@proc_buf"
#define STR2LONG_DATA_LEN "@str2long_data_len"
#define STR2LONG_DATA "@dtr2long_data"
#define BUFFER "@buffer"
#define BUFFER_LEN "@buffer_len"
#define CAL_BUF_LEN "@cal_buf_len"
#define CAL_BUF_LEN_EXIT "@cal_buf_len_exit"
#define _START "_start"

#define __(X) string("[") + X + "]"
#define _(X,Y) string("[") + X + "+" + Y + "]"
#define _ebp(X) string("[") + ebp + "+" + to_string(X) + "]" 
#define __ebp(X) string("[") + ebp + to_string(X) + "]"

// 40��ö�ٷ���
enum symbol {
	// �գ���ʶ�����쳣������
	null,ident,excep,number,
	// �ӣ������ˣ�������ֵ
	add,sub,mult,divi,assign,
	// >��>=��<��<=��==��!=
	gt,ge,lt,le,equ,nequ,
	// ����>>�����<<
	input,output,
	// �����char,string,;,(,),{,}
	comma,chara,strings,semicon,lparen,rparen,lbrac,rbrac,
	min,
	// ������
	rev_break,rev_char,rev_continue,rev_else,rev_extern,
	rev_if,rev_in,rev_int,rev_out,
	rev_return,rev_string,rev_void,rev_while,
	// end
	max
};


// 
enum error_c {
	// �ʷ�����
	char_wrong,string_wrong, annotation_wrong,line2long,
	str2long,num2long,id2long,expec_char,warn_escape,
	// �﷨����
	semicon_lost,commma_lost,type_lost,ident_lost,
	semicon_wrong,type_wrong, statement_wrong,lparen_wrong,
	para_lost, lparen_lost,rparen_lost,lbrac_lost,rbrac_lost,
	input_wrong, non_input,output_wrong, rparen_wrong, else_lost,
	else_wrong, return_wrong, idtail_lost, arg_lost, arg_wrong, arglist_wrong,
	op_lost, op_wrong, expr_lost, expr_wrong,

	//�������
	void_non_calc,str_non_add, void_non_assi, type_assi, void_non_in,

};