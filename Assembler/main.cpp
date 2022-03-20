#include "Parser.h"
#include "Generate.h"


int main() {
	Generate::Init("./ass_test/common.t");
	Elf_File::Init();
	Parser parser;
	parser.Init("./ass_test/common.s");
	parser.Program();
	parser.Over();
	return 0;
}