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

//caculate the minimal value of Euclidean distance add leaf or leaf's parent, and record
//the position to know which digit is the result
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

//return the position in input to decide which frame is the end frame of one digit
stack<int> RestrictPhone(Trie& trie, vector<vector<double>>& input, vector<vector<vector<double>>>& varianceTerm, vector<vector<vector<int>>>& countTransfer)
{
	int input_size = (int)input.size();
	TrieNode* root = trie.getRoot();

	//back table is used to record every frame comes from which state in which template
	//vector<int> contains the state num and whether have loop back
	//backTable[][][0] refer the state number, backTable[][][1] refer whether loop back
	vector<vector<vector<int>>> backTable(SEG_NUM * TYPE_NUM, vector<vector<int>>(input_size, vector<int>(2)));

	//leaf of every template
	vector<double> last(MAX_BRANCH_NUM - 1, INT_MAX / 2);
	//leaf's parent of every template
	vector<double> lastTwo(MAX_BRANCH_NUM - 1, INT_MAX / 2);

	for (int i = 0; i < input_size; i++)
	{
		for (int j = 0; j < MAX_BRANCH_NUM - 1; j++)
		{
			vector<double> temp(SEG_NUM, INT_MAX / 2);
			root->nextBranch[j]->curNodeCost = temp;
			if (i == 0)
			{
				root->nextBranch[j]->preNodeCost[0] = nodeCost(input[i], root->nextBranch[j]->segTemplate[0], varianceTerm[j][0]) + edgeCost(0, countTransfer[j][0]);
				root->nextBranch[j]->preNodeCost[1] = nodeCost(input[i], root->nextBranch[j]->segTemplate[1], varianceTerm[j][1]) + edgeCost(1, countTransfer[j][0]);
			}
			else
			{
				for (int k = 0; k < SEG_NUM; k++)
				{
					double var1 = nodeCost(input[i], root->nextBranch[j]->segTemplate[k], varianceTerm[j][k]);
					double var2 = root->nextBranch[j]->preNodeCost[k] + var1 + edgeCost(k, countTransfer[j][k + 1]);
					//                    double var3 = root->nextBranch[j]->preNodeCost[k - 1] + var1 + edgeCost(k, countTransfer[j][k]);
					//                    double var4 = root->nextBranch[j]->preNodeCost[k - 2] + var1 + edgeCost(k, countTransfer[j][k - 1]);
					int pos1 = 0;
					int pos2 = 0;
					double var5 = costUtil(last, var1, pos1) + PENALTY;
					double var6 = costUtil(lastTwo, var1, pos2) + PENALTY;
					if (k == 0) {
						var5 += edgeCost(k, countTransfer[j][0]);
						var6 += edgeCost(k, countTransfer[j][0]);
						if (var5 < INT_MAX / 2)
							root->nextBranch[j]->curNodeCost[k] = min({ var2, var5 , var6 });
						else if (var6 < INT_MAX / 2)
							root->nextBranch[j]->curNodeCost[k] = min({ var2, var6 });
						else
							root->nextBranch[j]->curNodeCost[k] = var2;
						if (root->nextBranch[j]->curNodeCost[k] == var5)
						{
							backTable[SEG_NUM * j + k][i][0] = SEG_NUM * pos1 + (SEG_NUM - 1);
							backTable[SEG_NUM * j + k][i][1] = 1;
						}
						else if (root->nextBranch[j]->curNodeCost[k] == var6)
						{
							backTable[SEG_NUM * j + k][i][0] = SEG_NUM * pos2 + (SEG_NUM - 2);
							backTable[SEG_NUM * j + k][i][1] = 1;
						}
						else if (root->nextBranch[j]->curNodeCost[k] == var2)
						{
							backTable[SEG_NUM * j + k][i][0] = SEG_NUM * j + k;
							//backTable[SEG_NUM * j + k][i][1] = backTable[SEG_NUM * j + k][i - 1][1];
						}
					}
					else if (k == 1) {
						var5 += edgeCost(k, countTransfer[j][0]);
						double var3 = root->nextBranch[j]->preNodeCost[k - 1] + var1 + edgeCost(k, countTransfer[j][k]);
						if (var5 < INT_MAX / 2)
							root->nextBranch[j]->curNodeCost[k] = min({ var3, var2, var5 });
						else
							root->nextBranch[j]->curNodeCost[k] = min({ var3, var2 });
						if (root->nextBranch[j]->curNodeCost[k] == var5)
						{
							backTable[SEG_NUM * j + k][i][0] = SEG_NUM * pos1 + (SEG_NUM - 1);
							backTable[SEG_NUM * j + k][i][1] = 1;
						}
						else if (root->nextBranch[j]->curNodeCost[k] == var2)
						{
							backTable[SEG_NUM * j + k][i][0] = SEG_NUM * j + k;
							//backTable[SEG_NUM * j + k][i][1] = backTable[SEG_NUM * j + k][i - 1][1];
						}
						else if (root->nextBranch[j]->curNodeCost[k] == var3)
						{
							backTable[SEG_NUM * j + k][i][0] = SEG_NUM * j + (k - 1);
							//backTable[SEG_NUM * j + k][i][1] = backTable[SEG_NUM * j + (k - 1)][i - 1][1];
						}
					}
					else
					{
						double var3 = root->nextBranch[j]->preNodeCost[k - 1] + var1 + edgeCost(k, countTransfer[j][k]);
						double var4 = root->nextBranch[j]->preNodeCost[k - 2] + var1 + edgeCost(k, countTransfer[j][k - 1]);
						root->nextBranch[j]->curNodeCost[k] = min({ var4, var3, var2 });
						if (root->nextBranch[j]->curNodeCost[k] == var4)
						{
							backTable[SEG_NUM * j + k][i][0] = SEG_NUM * j + (k - 2);
							//backTable[SEG_NUM * j + k][i][1] = backTable[SEG_NUM * j + (k - 2)][i - 1][1];
						}
						else if (root->nextBranch[j]->curNodeCost[k] == var2)
						{
							backTable[SEG_NUM * j + k][i][0] = SEG_NUM * j + k;
							//backTable[SEG_NUM * j + k][i][1] = backTable[SEG_NUM * j + k][i - 1][1];
						}
						else if (root->nextBranch[j]->curNodeCost[k] == var3)
						{
							backTable[SEG_NUM * j + k][i][0] = SEG_NUM * j + (k - 1);
							//backTable[SEG_NUM * j + k][i][1] = backTable[SEG_NUM * j + (k - 1)][i - 1][1];
						}
					}
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

	stack<int> resultPos;
	resultPos = backTrace(input, backTable, last);
	return resultPos;
}

//return the position in input to decide which frame is the end frame of one digit
stack<int> backTrace(vector<vector<double>>& input, vector<vector<vector<int>>>& backTable, vector<double>& last)
{
	int input_size = input.size();
	//get the minimal state at last frame
	double minLast = INT_MAX / 2;
	int posLast = 0;
	for (int i = 0; i < MAX_BRANCH_NUM - 1; i++)
	{
		if (minLast > last[i])
		{
			minLast = last[i];
			posLast = i * SEG_NUM + (SEG_NUM - 1);
		}
	}

	//result position of input
	stack<int> resultPos;
	//back tracing
	stack<int> resultPhone;
	resultPhone.push(posLast / SEG_NUM);
	int tracePos = posLast;
	for (int i = input_size - 1; i >= 0; i--)\
	{
		int curPos = backTable[tracePos][i][0];
		int loopBack = backTable[tracePos][i][1];
		if (loopBack == 1)
		{
			resultPhone.push(curPos / SEG_NUM);
			resultPos.push(i);
		}
		tracePos = curPos;
	}
	while (!resultPhone.empty())
	{
		cout << resultPhone.top() << " ";
		resultPhone.pop();
	}
	return resultPos;
}

//cut digit frame from continuous speech, then do k-mean to get seg template
void getContinuousSeg(Trie& trie, vector<vector<double>>& input, vector<vector<vector<double>>>& varianceTerm, vector<vector<vector<int>>>& countTransfer)
{
	stack<int> resultPos;
	resultPos = RestrictPhone(trie, input, varianceTerm, countTransfer);
	vector<vector<vector<double>>> inputSeg;
	int count = 0;
	int curX = 0;
	int preX = 0;
	while(!resultPos.empty())
	{
		curX = resultPos.top();
		int segLength = curX - preX;
		if (preX == 0)
		{
			for (int i = 0; i <= segLength; i++)
			{
				inputSeg[count][i] = input[i + preX];
			}
		}
		else
		{
			for (int i = 0; i < segLength; i++)
			{
				inputSeg[count][i] = input[(i + 1) + preX];
			}
		}
		preX = curX;
		count++;
		resultPos.pop();
	}
	int input_length = input.size();
	//get the last seg
	for(int i = 0; i < (input_length - 1 - preX); i++)
	{
		inputSeg[count][i] = input[(i + 1) + preX];
	}

	//vector<vector<vector<double>>> segTemGroup;

	/*
	for (int i = 0; i < TYPE_NUM; i++) {
	vector<vector<vector<double>>> temGroup;
	for (int j = 0; j < TEM_NUM; j++) {
	cout << "-----------------------Template " << i << " Instance " << j << "------------------------" << endl;
	string wavpath = wavTemPath + to_string(i) + "\\" + to_string(j) + "\\record.wav";
	//            capture(wavpath);
	vector<vector<double>> temFeature;
	string txtpath = txtTemPath + to_string(i) + "\\" + to_string(j) + "\\";
	featureExtraction(temFeature, wavpath, txtpath);
	temGroup.push_back(temFeature);
	}
	vector<vector<double>> segTem;
	segTem = dtw2hmm(temGroup);
	cout << "You have got the segment template!!!!!!!!!!!!!!!!!!!" << endl;
	segTemGroup.push_back(segTem);
	}

	ofstream out(segmentPath);
	for(int i = 0; i < TYPE_NUM; i++)
	{
	cout << "template " << i << endl;
	for(int j = 0; j < SEG_NUM; j++)
	{
	cout << "state " << j << endl;
	for(int k = 0; k < DIMENSION; k++)
	{
	out << segTemGroup[i][j][k] << " ";
	}
	out << endl;
	}
	out << endl;
	}
	*/
}