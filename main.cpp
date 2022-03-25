#include "windows.h"
#include "string.h"
#include <direct.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>


using namespace std;


/* logo
	0123456789012345678901234567890123
	######       #####         #####
		##      ###  ##       ###  ##
		##     ##		     ##
		##     ##		     ##
		##     ## 	         ##
		##     ## 	         ##
	##  ##      ###   ##      ###   ##
	 ####        ######        ######
*/

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

void Usage() {
	color_printf(FG_PURPLE,"%s", "jcc file1 file2 ...\n");
	xINFO("%s", "   [-h] help show detail information\n");
	xINFO("%s", "   [-v] show verision and developer info\n");
	xINFO("%s", "   [-c] compile the c code to s code\n");
	xINFO("%s", "   [-s] assmble the s code to obj code(elf-x86)\n");
	xINFO("%s", "   [-l][ out_name] link the obj file to one elf file\n");
	xINFO("%s", "   [-o][ out_name] compile the c code to elf file\n");
}


void Printc(char c = '#') {
	xINFO("%s", "\nWelcome to JCC tools!\n");
	xINFO("%s", "jcc means Just Compiler Compiler, which similar to gcc homophonic.\n");
	xINFO("%s", "if you have any question,pls contact with djh113@126.com\n");
	xINFO("%s", "version 1.0 ...\n\n\n");
	char position[][33] = { { 0,1,2,3,4,5,12,13,14,15,16,17,27,28,29,30,31},
							{4,5,12,13,14,17,18,26,27,28,31,32},
							{4,5,11,12,25,26,},
							{4,5,11,12,25,26,},
							{4,5,11,12,25,26,},
							{4,5,11,12,25,26,},
							{0,1,4,5,12,13,14,18,19,26,27,28,32,33},
							{1,2,3,4,13,14,15,16,17,18,27,28,29,30,31,32},
	};
	for (int i = 0; i < 8; i++) {
		int suffix = 10;
		while (suffix--) putchar(' ');
		char count = -1;
		for (int k = 0; k < 33; k++) {
			char element = position[i][k] - count - 1;
			count = position[i][k];
			if (element >= 0) {
				while (element) {
					putchar(' ');
					element--;
				}
				xSUCC("%c", c);
			}
			else break;
		}
		putchar('\n');
	}
	resetColor();
	putchar('\n');
	Usage();
}


void Compile(string pwd,vector<string>&files) {
	string cmd = "";
	for (int i = 0; i < files.size(); i++) {
		cmd += files[i];
		cmd += " ";
	}
	cmd = "jcc " + cmd;
	cmd = pwd + "\\Compiler\\compile.exe " + cmd;
	system(cmd.c_str());
}

void Assemble(string pwd, vector<string>& files) {
	string cmd = "";
	for (int i = 0; i < files.size(); i++) {
		cmd += files[i];
		cmd += " ";
	}
	cmd = "ass " + cmd;
	cmd = pwd + "\\Assembler\\assembly.exe " + cmd;
	system(cmd.c_str());
}

void Linker(string pwd,vector<string>&files,string output_path="") {
	string cmd = "";
	for (int i = 0; i < files.size(); i++) {
		cmd += files[i];
		cmd += " ";
	}
	cmd = "link " + cmd;
	cmd = pwd + "\\Linker\\linker.exe " + cmd;
	if (output_path != "") {
		cmd = cmd + " -o " + output_path;
	}
	system(cmd.c_str());
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		Printc();
	}
	else if (strcmpi(argv[1], "jcc") == 0) {
		int i = 2;
		vector<string>file_names;
		file_names.clear();
		// getting current work dir
		string pwd = getcwd(nullptr, 0);
		string output_path = "";
		char option = 0;
		for (i; i < argc; i++) {
			if (argv[i][0] == '-') {
				option = strlen(argv[i]) <= 1 ? 0 : argv[i][1];
				i++;
				break;
			}
			else {
				file_names.push_back(argv[i]);
			}
		}
		// check valid
		if (option == 0) {
			xPANIC("%s", "missing parameter! -?\n");
			return 0;
		}
		else if (file_names.size() == 0 && option != 'h' && option != 'v') {
			xWARN("%s", "no input file!\n");
		}
		else if (i < argc) {
			if (i == argc - 1) {
				output_path = argv[i];
			}
			else {
				xWARN("%s", "more parameter given!\n");
			}
		}
		// parameter parser
		switch (option) {
		case 'h': {
			Usage();
			break;
		}
		case 'v': {
			Printc();
			break;
		}
		case 'c': {
			Compile(pwd, file_names);
			break;
		}
		case 's': {
			Assemble(pwd, file_names);
			break;
		}
		case 'l': {
			Linker(pwd, file_names, output_path);
			break;
		}
		case 'o': {
			Compile(pwd, file_names);
			for (int i = 0; i < file_names.size(); i++) {
				int index = file_names[i].find_last_of('.');
				file_names[i] = file_names[i].substr(0, index) + ".s";
			}
			int index = file_names[0].find_last_of('\\');
			if (index == string::npos) {
				index = file_names[0].find_last_of('/');
			}
			string common_file = file_names[0].substr(0, index) + "\\common.s";
			file_names.push_back(common_file);
			Assemble(pwd, file_names);
			for (int i = 0; i < file_names.size(); i++) {
				file_names[i] = file_names[i] + ".o";
			}
			Linker(pwd, file_names, output_path);
			break;
		}
		}
	}
	else {
		xPANIC("Not recognize command %s ,using jcc\n", argv[1]);
		Printc();
	}
	return 0;
}

