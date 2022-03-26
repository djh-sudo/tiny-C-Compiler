#include <iostream>
#include "string.h"
#include "Parser.h"
#include "Generate.h"

using namespace std;

void Usage() {
	xWARN("%s", "Usage: ass file1 file2 ...\n");
}
int main(int argc, char* argv[]) {
	if (argc < 2) {
		Usage();
	}
	else if (strcmpi(argv[1], "ass") == 0) {
		for (int i = 2; i < argc; i++) {
			const char* path = argv[i];
			Generate::Init(string(path) + ".t");
			Elf_File::Init();
			Parser parser;
			parser.Init(path);
			parser.Program();
			parser.Over();
		}
	}
	else {
		xPANIC("%s%s%s", "not recognize ", argv[1], ", using ass\n");
		Usage();

	}
	return 0;
}