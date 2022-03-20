#pragma once

#include <iostream>
#include <cstdio>
#include <string>
#include "Common.h"
#include "Semantic.h"

class VarRecord;
class SIB;
class ModRM;
class Inst;
using namespace std;


static unsigned char op2code[] = {
	  /*       8 bit       |         32 bit      */
	  /* r,r  r,m rm,r r,im| r,r  r,m rm,r r,im  */
		0x88,0x8a,0x88,0xb0,0x89,0x8b,0x89,0xb8, // mov
		0x38,0x3a,0x38,0x80,0x39,0x3b,0x39,0x81, // cmp
		0x28,0x2a,0x28,0x80,0x29,0x2b,0x29,0x81, // sub
		0x00,0x02,0x00,0x80,0x01,0x03,0x01,0x81, // add
		0x00,0x00,0x00,0x00,0x00,0x8d,0x00,0x00  // lea
};

static unsigned char op1code[] = {
	/*  call  int imul idiv  neg  inc  dec jmp  */
		0xe8,0xcd,0xf7,0xf7,0xf7,0x40,0x48,0xe9,
	/*    je     jg     jl    jge     jle   jne    jna */
		0x0f84,0x0f8f,0x0f8c,0x0f8d,0x0f8e,0x0f85,0x0f86,
	/*  push  pop */
		0x50,0x58
};

static unsigned char op0code[] = {
	/*  ret  */
		0xc3,
};

class Generate{
private:
	static FILE* fout;
	static int total_length;
public:
	static int error_number;
	static int line_number;
public:
	static bool Init(string file_name);
	static void WriteModRM(ModRM modrm, bool scan);
	static void WriteSIB(SIB sib, bool scan);
	static void WriteBytes(int value, int len, bool scan);
	static void WriteBytes(int value, int len);
	static void WriteBytes(const void* buffer, size_t len);
	static bool HandleRelocation(int type, bool scan, VarRecord* rel, string cur_seg);
	static void Generate2Op(symbol op, int des, int src, int len, Inst instructure, bool scan, VarRecord* rel, string cur_seg);
	static void Generate1Op(symbol op, int op_type, int len, Inst instructure, bool scan, VarRecord* relo, string cur_seg);
	static void Generate0Op(symbol op, bool scan);
	static void Over();
	static void Error(error_c code);
};	 
	 
