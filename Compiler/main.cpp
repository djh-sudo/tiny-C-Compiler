#include "Parser.h"
#include "Generator.h"


int main() {
	Parser parser;
	parser.Init("./compiler_test/test1.tc");
	parser.Program();
	parser.end();
	return 0;
}