#include "LextreeProcess.h"


bool readTestFile(string filePath, string inFileName, string outFileName, vector<string>& vec)
{
	convertFile(filePath, inFileName, outFileName);
	readFile(filePath, outFileName, vec);
	return true;
}

bool createTrie(string filePath, string inFileName, Trie& trie)
{
	string str;
	ifstream in(filePath + inFileName);
	while (in >> str)
	{
		trie.Insert(str);
	}
	return true;
}

void beamSearch(map<string, int>& resultSet, Trie& trie, string& input)
{
	input.insert(0, "*");
	const int input_len = input.size();
	TrieNode* node = trie.getRoot();
	string str = "";     //used to save the template in a map
	for (int i = 0; i < input_len; i++)
	{
		if (i > 1)
			trie.swapNodeCost();     //swap the preNodeCost and curNodeCost when move to next input character
		beamSearchUtil(resultSet, node, input, str, i);
	}
}

void beamSearchUtil(map<string, int>& resultSet, TrieNode* node, string& input, string& tempStr, int pos)
{
	if (node == NULL)
		return;

	if (pos == 0)
	{
		if (node->getParent() == NULL)
			node->preNodeCost = 0;
		else
			node->preNodeCost = node->parentBranch->preNodeCost + 1;
	}
	else
	{
		if (node->getParent() == NULL)
			node->curNodeCost = node->preNodeCost + 1;
		else
			node->curNodeCost = min({ node->preNodeCost + 1, node->parentBranch->curNodeCost + 1,
				node->parentBranch->preNodeCost + (input[pos] == node->getNodeLetter() ? 0 : 1) });
		if (pos == input.length() - 1)
			tempStr += node->getNodeLetter();
	}

	if (node->isLeaf() && pos == input.length() - 1)
	{
		//resultSet[tempStr] = node->curNodeCost;
		resultSet.insert(pair<string, int>(tempStr, node->curNodeCost));
	}

	for (int i = 0; i < MAX_BRANCH_NUM; i++) {
		
		string tempStr2 = tempStr;
		beamSearchUtil(resultSet, node->nextBranch[i], input, tempStr2, pos);
	}
	
}