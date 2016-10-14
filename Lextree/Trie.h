#ifndef Trie_h
#define Trie_h

#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>
#include <sstream>

using namespace std;

#define MAX_BRANCH_NUM  52   // we have 26 characters. double it so that can store the one that ends here
#define BEAM (3)

/* define the class for the node of the trie tree*/
class TrieNode
{
public:
	char letter;     // one letter of the word that store there
	TrieNode* nextBranch[MAX_BRANCH_NUM];   // the pointers' vectors that point to the next branch
	TrieNode* parentBranch;
	int preNodeCost;
	int curNodeCost;
public:
	TrieNode() : letter(NULL)
	{
		memset(nextBranch, NULL, sizeof(TrieNode*) * MAX_BRANCH_NUM);
		parentBranch = NULL;
		preNodeCost = curNodeCost = INT_MAX / 2;
	}
	bool setPreNodeCost(int val);
	int getPreNodeCost();
	bool setCurNodeCost(int val);
	int getCurNodeCost();
	bool setNodeLetter(char c);
	char getNodeLetter();
	TrieNode* getParent();
	bool isLeaf();     
};


/* define the class for the trie tree */
class Trie {
public:
	Trie();
	~Trie();
	void Insert(string& str);   // insert string str
	bool Search(string& str);
	void PrintALL();            // print out all the node in the trie tree
	TrieNode* getRoot();        //get root node   
	void swapNodeCost(int minCost);
	void swapNodeCostUtil(TrieNode* node, int minCost);
private:
	TrieNode* pRoot;
private:
	void Destory(TrieNode* pRoot);
	bool Print(TrieNode* pRoot, string& str);
};



#endif /* Trie_h */