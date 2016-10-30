#ifndef LEXTREE_PROCESS
#include <iostream>
#include <map>
#include <algorithm>
#include "Trie.h"
#include "readFile.h"


bool readTestFile(string filePath, string inFileName, string outFileName, vector<string>& vec);
bool createTrie(string filePath, string inFileName, Trie& trie);
void beamSearch(map<string, int>& resultSet, Trie& trie, string& input);
void beamSearchUtil(map<string, int>& resultSet, TrieNode* node, string& input, string& tempStr, int pos, int& minCost);
#endif LEXTREE_PROCESS
