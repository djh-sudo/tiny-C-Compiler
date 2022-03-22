#include <iostream>
#include "Parser.h"
#include "Generate.h"

using namespace std;

void Usage() {
	cout << "Usage: ass file1 file2 ..." << endl;
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
		cout << "not recognize " << argv[1] << ", using ass" << endl;
		Usage();

	}
	return 0;
}