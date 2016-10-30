#include <iostream>
#include <fstream>
#include <string>
#include "LextreeProcess.h"

using namespace std;

int main(int argc, const char * argv[]) {

	string filePath = "";

	/*
	string inFileName = "typos.txt";
	string outFileName = "pureTypos.txt";
	string dictFileName = "dict_1.txt";

	string input = "abbreviate";

	vector<string> typos;
	Trie trie;
	map<string, int> resultSet;

	//readTestFile(filePath, inFileName, outFileName, typos);
	createTrie(filePath, dictFileName, trie);
	beamSearch(resultSet, trie, input);

	map<string, int>::const_iterator iter = resultSet.begin();
	for (; iter != resultSet.end(); iter++)
		cout << iter->first << " " << iter->second << endl;
*/

	string inFileName = "typos.txt";
	string outFileName = "pureTypos.txt";
	string outFileName2 = "correct.txt";
	string dictFileName = "dict_1.txt";

	string input = "abbreviate";

	vector<string> typos;
	Trie trie;

	ofstream out(filePath + outFileName2);

	readFile(filePath, inFileName, typos);
	//    readTestFile(filePath, inFileName, outFileName, typos);
	createTrie(filePath, dictFileName, trie);

	for (int i = 0; i < typos.size(); i++) {
		map<string, int> resultSet;
		int minCost = INT_MAX / 2;
		string bestString;
		beamSearch(resultSet, trie, typos[i]);
		map<string, int>::const_iterator iter = resultSet.begin();
		for (; iter != resultSet.end(); iter++) {
			if (iter->second < minCost) {
				minCost = iter->second;
				bestString = iter->first;
				bestString = bestString.erase(0, 1);
			}
		}
		//        cout << typos[i] << "    " << bestString << endl;
		out << bestString + ' ';
		cout << bestString << " ";
	}
	out.close();

	return 0;
}