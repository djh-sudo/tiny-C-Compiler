#include "Lexer.h"
#include "Generator.h"


int main() {
	Generator::GenerateComm();
	Lexer lexer;
	bool result = lexer.init("./compiler_test/test7.tc");
	if (result) {
		lexer.set_show_flag(true);
		while (lexer.GetSymbol());
		cout << "lexical error:" << lexer.get_error_number();
		lexer.over();
	}
	return 0;
}