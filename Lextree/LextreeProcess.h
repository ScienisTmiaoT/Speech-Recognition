#ifndef LEXTREE_PROCESS

#include <algorithm>
#include <map>
#include "Trie.h"
#include "readFile.h"

#define BEAM (3)

bool readTestFile(string filePath, string inFileName, string outFileName, vector<string>& vec);
bool createTrie(string filePath, string inFileName, Trie& trie);
void beamSearch(map<string, int>& resultSet, Trie& trie, string& input);
void beamSearchUtil(map<string, int>& resultSet, TrieNode* node, string& input, string& tempStr, int pos);
#endif LEXTREE_PROCESS
