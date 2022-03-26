#include <iostream>
#include <string.h>
#include "Parser.h"
#include "Generator.h"


using namespace std;

inline void Usage() {
	xSUCC("%s", "Usage:jcc src_file1 file2 ...\n");
}

int main(int argc,char*argv[]) {
	if (argc < 2) {
		Usage();
	}
	else if (strcmpi(argv[1], "jcc") == 0) {
		for (int i = 2; i < argc; i++) {
			Parser parser;
			parser.Init(argv[i]);
			parser.Program();
			parser.end();
		}
	}
	else {
		xWARN("%s%s%s", "Not recognize command ", argv[1], " ,using jcc \n");
		Usage();
	}

	return 0;
}