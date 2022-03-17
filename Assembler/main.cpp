#include "Lexer.h"

int main() {
	Lexer lexer;
	bool res = lexer.Init("./ass_test/common.s");
	if (res) {
		lexer.set_flag(true);
		while (lexer.GetSym());
		lexer.over();
	}
	return 0;
}