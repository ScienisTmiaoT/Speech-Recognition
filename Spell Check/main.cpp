#include "spellCheck.h"
using namespace std;

int main() {
	//spellCheck();
	printPath("noce", "notice", FILE_PATH, PRINT_TABLE_NAME);
	cout << pureLevenshteinDistance("noce", "notice") << endl;
	return 0;
}