#include <iostream>
#include "Common.h"
#include "string.h"
#include "Link.h"


using namespace std;

void Usage() {
	xWARN("%s", "link obj1 obj2 ... [-o out_name]\n");
	xWARN("%s", "default output name is a.out\n");
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
		if (argc >= 3) {
			if (i < argc) {
				linker.Excute(argv[i]);
			}
			else {
				string prefix = argv[2];
				int index = prefix.find_last_of("\\");
				if (index == string::npos) {
					index = prefix.find_last_of("/");
				}
				prefix = prefix.substr(0, index) + "\\a.out";
				linker.Excute(prefix.c_str());
			}
		}
	}
	else {
		xPANIC("%s%s%s", "Not recognize command ", argv[1], " ,using link\n");
		Usage();
	}
	return 0;
}
