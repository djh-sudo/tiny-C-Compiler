#include "Lexer.h"

int main() {
	Lexer lexer;
	bool result = lexer.init("./compiler_test/test4.tc");
	lexer.set_show_flag(true);
	while (lexer.GetSymbol());
	cout << "lexical error:" << lexer.get_error_number();
	lexer.over();
	return 0;
}