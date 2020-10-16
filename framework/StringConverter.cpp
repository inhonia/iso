/* String Converter by @itsthatfox */
#include "string"
#include <iostream>
using namespace std;
void init(string& T) { for (int o = 0; o <= T.length(); o++) { T[o] = toupper(T[o]); } }
int main()
{
	string s, reg;
	getline(cin, s);
	if (s.length() > NULL) { reg = s; }
	init(s);
	cout << "#define  " + s + "                     " + "XorStr(\"\")\n", reg;
	system("pause");
	return NULL;
}