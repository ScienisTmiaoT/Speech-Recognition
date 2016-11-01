#include "ContinuousPhone.h"

double Dis(vector<double>& matrix1, vector<double>& matrix2) {
	double distance = 0;
	unsigned int num1 = (unsigned int)matrix1.size();
	unsigned int num2 = (unsigned int)matrix2.size();

	if (num1 == num2) {
		for (unsigned int i = 0; i < num1; i++) {
			distance += pow((matrix1[i] - matrix2[i]), 2);
		}
		distance = sqrt(distance);
	}
	else {
		cout << "------------------ERROR------------------" << endl;
		cout << "THE DIMENSION OF EACH VECTOR IS NOT SAME!" << endl;
	}

	return distance;
}

double costUtil(vector<double>& vec, double c, int& pos)
{
	int length = vec.size();
	double cost = INT_MAX / 2;
	for(int i = 0; i < length; i++)
	{
		if(cost > vec[i] + c)
		{
			cost = vec[i] + c;
			pos = i;
		}
	}
	return cost;
}

void RestrictPhone(Trie& trie, vector<vector<double>>& input, vector<int>& minNode)
{
	int input_size = input.size();
	TrieNode* root = trie.getRoot();

	vector<int> emitTemp(input_size, 0);
	for(int i = 0; i < MAX_BRANCH_NUM - 1; i++)
	{
		root->nextBranch[i]->nonEmitting = emitTemp;
	}

	vector<double> last(0, MAX_BRANCH_NUM - 1);
	vector<double> lastTwo(0, MAX_BRANCH_NUM - 1);
	for(int i = 1; i < input_size; i++)
	{
		for(int j = 0; j < MAX_BRANCH_NUM - 1; j++)
		{
			vector<double> temp(SEG_NUM, INT_MAX / 2);
			root->nextBranch[j]->curNodeCost = temp;
			if(i == 0)
			{
				for(int k = 0; k < SEG_NUM; k++)
				{
					root->nextBranch[j]->preNodeCost[k] = Dis(input[i], root->nextBranch[j]->segTemplate[k]);
				}
				last[j] = root->nextBranch[j]->preNodeCost[SEG_NUM - 1];
				lastTwo[j] = root->nextBranch[j]->preNodeCost[SEG_NUM - 2];
			}
			else
			{
				for (int k = 0; k < SEG_NUM; k++)
				{
					double var1 = Dis(input[i], root->nextBranch[j]->segTemplate[k]);
					double var2 = root->nextBranch[j]->preNodeCost[k] + var1;
					double var3 = root->nextBranch[j]->preNodeCost[k - 1] + var1;
					double var4 = root->nextBranch[j]->preNodeCost[k - 2] + var1;
					int pos1 = 0;
					int pos2 = 0;
					double var5 = costUtil(last, var1, pos1);
					double var6 = costUtil(lastTwo, var1, pos2);
					bool addFlag = false;
					if (k == 0) {
						root->nextBranch[j]->curNodeCost[k] = min({var2, var5, var6 });
						if (root->nextBranch[j]->curNodeCost[k] == var5)
						{
							root->nextBranch[j]->nonEmitting[k]++;
							minNode[i] = pos1;
							addFlag = true;
						}
						else if (root->nextBranch[j]->curNodeCost[k] == var6)
						{
							root->nextBranch[j]->nonEmitting[k]++;
							minNode[i] = pos2;
							addFlag = true;
						}
					}
					else if (k == 1) {
						root->nextBranch[j]->curNodeCost[k] = min({ var3, var2, var5 });
						if (root->nextBranch[j]->curNodeCost[k] == var5 && addFlag == false)
						{
							root->nextBranch[j]->nonEmitting[k]++;
							minNode[i] = pos1;
						}
					}
					else
						root->nextBranch[j]->curNodeCost[k] = min({ var4, var3, var2 });
				}
				last[j] = root->nextBranch[j]->curNodeCost[SEG_NUM - 1];
				lastTwo[j] = root->nextBranch[j]->curNodeCost[SEG_NUM - 2];
			}
		}
		trie.swapNodeCost();
	}

}