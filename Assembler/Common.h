#pragma once
#define MAX_LEN 80
#define ID_LEN 30
#define STRING_LEN 255
#define RESERVED_NUMBER 46
#define NUM_LENGTH 9

#define al "al"
#define cl "cl"
#define dl "dl"
#define bl "bl"

#define ah "ah"
#define ch "ch"
#define dh "dh"
#define bh "bh"

#define eax "eax"
#define ecx "ecx"
#define edx "edx"
#define ebx "ebx"

#define esp "esp"
#define ebp "ebp"
#define esi "esi"
#define edi "edi"

#define mov "mov"
#define cmp "cmp"
#define sub "sub"
#define add "add"
#define lea "lea"
#define call "call"
#define sys_int "int"
#define imul "imul"
#define idiv "idiv"
#define neg "neg"
#define inc "inc"
#define dec "dec"
#define jmp "jmp"
#define je "je"
#define jg "jg"
#define jl "jl"
#define jge "jge"
#define jle "jle"
#define jne "jne"
#define jna "jna"
#define push "push"
#define pop "pop"
#define ret "ret"
#define section "section"
#define global "global"
#define equ "equ"
#define times "times"
#define db "db"
#define dw "dw"
#define dd "dd"


enum symbol {
	null, ident, excep, number, strings,
	addi, subs, comma, lbrac,
	rbrac, colon,
	rev_al, rev_cl, rev_dl, rev_bl,
	rev_ah, rev_ch, rev_dh, rev_bh,
	rev_eax, rev_ecx, rev_edx, rev_ebx,
	rev_esp, rev_ebp, rev_esi, rev_edi,
	rev_mov, rev_cmp, rev_sub, rev_add, rev_lea,
	rev_call, rev_sys_int, rev_imul, rev_idiv,
	rev_neg, rev_inc, rev_dec, rev_jmp,
	rev_je, rev_jg, rev_jl, rev_jge,
	rev_jle, rev_jne, rev_jna, rev_push,
	rev_pop, rev_ret, rev_section, rev_global,
	rev_equ, rev_times, rev_db, rev_dw, rev_dd
};