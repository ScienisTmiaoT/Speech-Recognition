#include "ContinuousPhone.h"

//Euclidean distance
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

//caculate the minimal value of dis add leaf or leaf's parent
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

	//initial non-emitting count
	vector<int> emitTemp(input_size, 0);
	for(int i = 0; i < MAX_BRANCH_NUM - 1; i++)
	{
		root->nextBranch[i]->nonEmitting = emitTemp;
	}

	//leaf of every template
	vector<double> last(MAX_BRANCH_NUM - 1, INT_MAX / 2);
	//leaf's parent of every template
	vector<double> lastTwo(MAX_BRANCH_NUM - 1, INT_MAX / 2);

	for(int i = 0; i < input_size; i++)
	{
		for(int j = 0; j < MAX_BRANCH_NUM - 1; j++)
		{
			vector<double> temp(SEG_NUM, INT_MAX / 2);
			root->nextBranch[j]->curNodeCost = temp;
			if(i == 0)
			{
				root->nextBranch[j]->preNodeCost[0] = Dis(input[i], root->nextBranch[j]->segTemplate[0]);
				root->nextBranch[j]->preNodeCost[1] = Dis(input[i], root->nextBranch[j]->segTemplate[1]);
			}
			else
			{
				//avoid that in every template, if first state and second state all come from
				//loop back, then the minNode will change twice
				bool addFlag = false;
				for (int k = 0; k < SEG_NUM; k++)
				{
					double var1 = Dis(input[i], root->nextBranch[j]->segTemplate[k]);
					double var2 = root->nextBranch[j]->preNodeCost[k] + var1;
					double var3 = root->nextBranch[j]->preNodeCost[k - 1] + var1;
					double var4 = root->nextBranch[j]->preNodeCost[k - 2] + var1;
					int pos1 = 0;
					int pos2 = 0;
					double var5 = costUtil(last, var1, pos1) + PENALTY;
					double var6 = costUtil(lastTwo, var1, pos2) + PENALTY;
					if (k == 0) {
						if (var5 < INT_MAX / 2 )
							root->nextBranch[j]->curNodeCost[k] = min({ var2, var5, var6 });
						else if (var6 < INT_MAX / 2)
							root->nextBranch[j]->curNodeCost[k] = min({ var2, var6 });
						else
							root->nextBranch[j]->curNodeCost[k] = var2;
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
						if (var5 < INT_MAX / 2)
							root->nextBranch[j]->curNodeCost[k] = min({ var3, var2, var5 });
						else
							root->nextBranch[j]->curNodeCost[k] = min({ var3, var2 });
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
 		if (i > 0)
		{
			trie.swapNodeCost();
		}
	}
	return;
}