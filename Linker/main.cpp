#include <iostream>
#include "Link.h"


using namespace std;

void Usage() {
	cout << "link obj1 obj2 ... [-o out_name]" << endl;
	cout << "default output name is a.out" << endl;
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		Usage();
	}
	else if (strcmpi(argv[1], "link") == 0) {
		Link linker;
		int i = 2;
		for (i; i < argc; i++) {
			if (strcmpi(argv[i], "-o") == 0) {
				i++;
				break;
			}
			linker.AddElf(argv[i]);
		}
		if (i < argc) {
			linker.Excute(argv[i]);
		}
		else {
			linker.Excute("a.out");
		}
	}
	else {
		cout << "Not recognize command " << argv[1] << " ,using link" << endl;
		Usage();
	}
	return 0;
}