#include <iostream>
#include "windows.h"
using namespace std;

#define WHITE   "\033[37m"      /* White */
#define COLOR   "\033[32m"      /* color */

/* logo
	0123456789012345678901234567890123
	######       #####         #####
		##      ###  ##       ###  ##
		##     ##		     ##
		##     ##		     ##
		##     ## 	         ##
		##     ## 	         ##
	##  ##      ###   ##      ###   ##
	 ####        ######        ######
*/
void printc(char c = '#') {
	cout << endl << "Welcome to JCC tools!" << endl;
	cout << "jcc means Just Compiler Compiler, which similar to gcc homophonic." << endl;
	cout << "if you have any question,pls contact with djh113@126.com" << endl;
	cout << "version 1.0 ..." << endl;
	cout << endl << endl;
	cout<< COLOR << endl;
	char position[][33] = { { 0,1,2,3,4,5,12,13,14,15,16,17,27,28,29,30,31},
							{4,5,12,13,14,17,18,26,27,28,31,32},
							{4,5,11,12,25,26,},
							{4,5,11,12,25,26,},
							{4,5,11,12,25,26,},
							{4,5,11,12,25,26,},
	                        {0,1,4,5,12,13,14,18,19,26,27,28,32,33},
	                        {1,2,3,4,13,14,15,16,17,18,27,28,29,30,31,32},
	};
	for (int i = 0; i < 8; i++) {
		int suffix = 10;
		while (suffix--) putchar(' ');
		char count = -1;
		for (int k = 0; k < 33; k++) {
			char element = position[i][k] - count - 1;
			count = position[i][k];
			if (element >= 0) {
				while (element) {
					putchar(' ');
					element--;
				}
				 putchar(c);
			}
			else break;
		}
		putchar('\n');
	}
	cout << WHITE << endl;
}


int main() {
	printc();
	return 0;
}

