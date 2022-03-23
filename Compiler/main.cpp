#include <iostream>
#include <string.h>
#include "Parser.h"
#include "Generator.h"


using namespace std;

void Usage() {
	cout << "Usage:jcc src_file1 file2 ..." << endl;
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
		cout << "Not recognize command " << argv[1] << " ,using jcc " << endl;
		Usage();
	}

	return 0;
}