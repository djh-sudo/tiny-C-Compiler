#pragma once
#include <iostream>
using namespace std;

#define ID_LEN 30
#define NUM_LEN 9
#define STRING_LEN 255
#define MAX_LEN 80
#define RESERVED_NUMBER 13
#define GLOBAL_STRING -2
// 40个枚举符号
enum symbol {
	// 空，标识符，异常，数字
	null,ident,excep,number,
	// 加，减，乘，除，赋值
	add,sub,mult,divi,assign,
	// >，>=，<，<=，==，!=
	gt,ge,lt,le,equ,nequ,
	// 输入>>，输出<<
	input,output,
	// 界符，char,string,;,(,),{,}
	comma,chara,strings,semicon,lparen,rparen,lbrac,rbrac,
	min,
	// 保留字
	rev_break,rev_char,rev_continue,rev_else,rev_extern,
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

};