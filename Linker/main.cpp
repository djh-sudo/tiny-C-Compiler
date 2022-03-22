#include "Link.h"


int main() {
	Link linker;
	linker.AddElf("./linker_test/test.s.o");
	linker.AddElf("./linker_test/common.s.o");
	
	linker.Excute("./linker_test/a.out");
	return 0;
}