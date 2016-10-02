#include "dtw.h"

/**
 *  compute the Euclidean distance of two 39-dimension vector
 *
 *  @param matrix1                  the first vector
 *  @param matrix2    the second vector
 *
 *  @return the double distance
 */
double dis(vector<double>& matrix1, vector<double>& matrix2){
    double distance = 0;
    unsigned int num1 = (unsigned int)matrix1.size();
    unsigned int num2 = (unsigned int)matrix2.size();
    
    if (num1 == num2) {
        for (unsigned int i = 0; i < num1; i++) {
            distance += pow((matrix1[i] - matrix2[i]), 2);
        }
        distance = sqrt(distance);
    }
    else{
        cout << "------------------ERROR------------------"<< endl;
        cout << "THE DIMENSION OF EACH VECTOR IS NOT SAME!"<< endl;
    }
    
    return distance;
}


/**
 *  use dtw to compute the cost
 *
 *  @param inputAduio input audio feature, seperate into different frames and each frame with a 39-D features
 *  @param temAduio   pre-input template audio feature, the same as input
 *
 *  @return the minimun cost from the best path.
 */
double dtw(vector<vector<double>>& inputAduio, vector<vector<double>>& temAduio){
    
    unsigned int inputFrameNum = (unsigned int)inputAduio.size();
    unsigned int temFrameNum = (unsigned int)temAduio.size();
    
    vector<double> col(temFrameNum), prevCol(temFrameNum), colMax(temFrameNum);
    
    for (unsigned int i = 0; i < temFrameNum; i++) {
        colMax[i] = DBL_MAX / 2;
    }
    
    prevCol = colMax;
    
    prevCol[0] = dis(inputAduio[0], temAduio[0]);
    prevCol[1] = dis(inputAduio[0], temAduio[1]);
    
    for (unsigned int i = 1; i < inputFrameNum; i++) {
        
        unsigned int temp = min({2 * (i + 1), temFrameNum});
        col = colMax;
        
        for (unsigned int j = 0; j < temp; j++) {
            
            if (j == 0) {
                col[j] = prevCol[j] + dis(inputAduio[i], temAduio[j]);
            }
            
            else if (j == 1){
                col[j] = min({prevCol[j - 1], prevCol[j]}) + dis(inputAduio[i], temAduio[j]);
            }
            else
                col[j] = min({prevCol[j - 2], prevCol[j - 1], prevCol[j]}) +  dis(inputAduio[i], temAduio[j]);
        }
        
        col.swap(prevCol);
    }
    
    return prevCol[temFrameNum - 1];
}

double beamDTW(vector<vector<double>>& inputAudio, vector<vector<double>>& temAudio)
{
	unsigned int inputLen = (unsigned int)inputAudio.size();
	unsigned int temLen = (unsigned int)temAudio.size();
	bool** flagArray;
	flagArray = new bool*[temLen];
	for (int i = 0; i < temLen; i++)
	{
		flagArray[i] = new bool[inputLen];
		memset(flagArray[i], true, inputLen * sizeof(bool));
	}
	vector<double> col(temLen), preCol(temLen), colMax(temLen);
	for (unsigned int i = 0; i < temLen; i++)
		colMax[i] = DBL_MAX / 2;
	preCol = colMax;
	preCol[0] = dis(inputAudio[0], temAudio[0]);
	preCol[1] = dis(inputAudio[0], temAudio[1]);
	for (unsigned int i = 1; i < inputLen; i++)
	{
		double beamTemp = DBL_MAX / 2;
		col = colMax;
		unsigned int tempLen = min(2 * i + 2, temLen);
		for (unsigned int j = 0; j < tempLen; j++)
		{
			if (j == 0 && flagArray[j][i])
				col[j] = preCol[j] + dis(inputAudio[i], temAudio[j]);
			else if(j == 1 && flagArray[j][i])
				col[j] = min(preCol[j], preCol[j - 1]) + dis(inputAudio[i], temAudio[j]);
			else if(j > 1 && flagArray[j][i])
				col[j] = min({ preCol[j], preCol[j - 1], preCol[j - 2] }) + dis(inputAudio[i], temAudio[j]);
			if (beamTemp > col[j])
				beamTemp = col[j];
		}
		for (unsigned int j = 0; j < tempLen; j++)
		{
			if(col[j] - beamTemp > DTW_BEAM)
			{
				unsigned int k = i;
				unsigned int p = j;
				while(k < inputLen && p < temLen)
				{
					flagArray[p][k] = false;
					k++;
					p += 2;
				}
			}
		}
		col.swap(preCol);
	}
	delete[] flagArray;
	return preCol[temLen - 1];
}

//print the path of dtw
double printDTWPath(vector<vector<double>>& s1, vector<vector<double>>& s2, ofstream& out) {
	const std::size_t len1 = s1.size(), len2 = s2.size();
	vector<vector <double> > printArray(len1, vector<double>(len2, 0));
	bool** flagArray;
	flagArray = new bool*[len1];
	for (int i = 0; i < len1; i++) {
		flagArray[i] = new bool[len2];
		memset(flagArray[i], false, len2 * sizeof(bool));
	}
	vector<double> col(len2), prevCol(len2), colMax(len2);
	for (unsigned int i = 0; i < len2; i++) {
		colMax[i] = DBL_MAX / 2;
	}
	prevCol = colMax;
	prevCol[0] = dis(s1[0], s2[0]);
	prevCol[1] = dis(s1[0], s2[1]);
	printArray[0] = prevCol;
	for (unsigned int i = 1; i < len1; i++) {

		unsigned int temp = min({ 2 * (i + 1), len2 });
		col = colMax;
		for (unsigned int j = 0; j < temp; j++) {
			if (j == 0) {
				col[j] = prevCol[j] + dis(s1[i], s2[j]);
			}
			else if (j == 1) {
				col[j] = min({ prevCol[j - 1], prevCol[j] }) + dis(s1[i], s2[j]);
			}
			else
				col[j] = min({ prevCol[j - 2], prevCol[j - 1], prevCol[j] }) + dis(s1[i], s2[j]);
		}
		printArray[i] = col;
		col.swap(prevCol);
	}
	for (int i = len1 - 1; i >= 0; i--)
	{
		for (int j = len2 - 1; j >= 0; j--)
		{
			if (i != 0 && j != 0) {
				if (i == len1 - 1 && j == len2 - 1)
				{
					flagArray[i][j] = true;
					double temp = min({ printArray[i - 1][j - 2], printArray[i - 1][j - 1], printArray[i - 1][j] });
					if (printArray[i - 1][j] == temp)
					{
						flagArray[i - 1][j] = true;
						break;
					}
					if (printArray[i - 1][j - 1] == temp)
					{
						flagArray[i - 1][j - 1] = true;
						break;
					}
					if (printArray[i - 1][j - 2] == temp)
					{
						flagArray[i - 1][j - 2] = true;
						break;
					}
				}
				else if(j == 1)
				{
					if (flagArray[i][j]) {
						double temp = min({ printArray[i - 1][j - 2], printArray[i - 1][j - 1], printArray[i - 1][j] });
						if (printArray[i - 1][j] == temp)
						{
							flagArray[i - 1][j] = true;
							break;
						}
						if (printArray[i - 1][j - 1] == temp)
						{
							flagArray[i - 1][j - 1] = true;
							break;
						}
					}
				}
				else
				{
					if(flagArray[i][j])
					{
						double temp = min({ printArray[i - 1][j - 2], printArray[i - 1][j - 1], printArray[i - 1][j] });
						if (printArray[i - 1][j] == temp)
						{
							flagArray[i - 1][j] = true;
							break;
						}
						if (printArray[i - 1][j - 1] == temp)
						{
							flagArray[i - 1][j - 1] = true;
							break;
						}
						if (printArray[i - 1][j - 2] == temp)
						{
							flagArray[i - 1][j - 2] = true;
							break;
						}
					}
				}
			}
			else if(i != 0 && j == 0)
			{
				if(flagArray[i][j])
				{
					flagArray[i - 1][j] = true;
				}
			}
			else if(i == 0 && j != 0)
			{
				if (flagArray[i][j])
				{
					flagArray[i][j - 1] = true;
				}
			}
		}
	
	}
	if (!out.is_open())
		cout << "fail to open file in printDTWPath" << endl;
	for (int i = 0; i <= len2; i++)
	{
		for (int j = len1; j >= 0; j--)
		{
			if (i == len2)
			{
				if (j == len1) {
					cout << setw(TABLE_GAP) << " ";
					out << setw(TABLE_GAP) << " ";
				}
				else {
					cout << setw(TABLE_GAP) << len1 - j;
					out << setw(TABLE_GAP) << len1 - j;
				}
			}
			else if(j == len1)
			{
				cout << setw(TABLE_GAP) << len2 - i;
				out << setw(TABLE_GAP) << len2 - i;
			}
			else
			{
				if (printArray[len1 - 1 - j][len2 - 1 - i] >= DBL_MAX / 2) {
					cout << setw(TABLE_GAP) << "INF";
					out << setw(TABLE_GAP) << "INF";
				}
				else
				{
					//set flag
					cout.setf(ios::fixed);
					//set to one digit
					cout << setw(TABLE_GAP) << setprecision(1) << printArray[len1 - 1 - j][len2 - 1 - i];
					//restore original flag
					cout.unsetf(ios::fixed);
					//restore original digits
					cout << setprecision(6);

					out.setf(ios::fixed);
					out << setw(TABLE_GAP) << setprecision(1) << printArray[len1 - 1 - j][len2 - 1 - i];
					out.unsetf(ios::fixed);
					out << setprecision(6);
				}
				if (flagArray[len1 - 1 - j][len2 - 1 - i]) {
					cout << "*";
					out << "*";
				}
			}
		}
		cout << endl;
		out << endl;
	}
	return prevCol[len2 - 1];
}



/**
 *  do the time-synchronomous DTW
 *
 *  @param inputAduio : input audio feature, seperate into different frames and each frame with a 39-D features
 *  @param tempsAudio : several template audio features, seperate into different templates, then into different frames with 39-D features.
 *
 *  @return a vector with a size of 2, vector(0) stores the index of the best fit template(need to be transformed into unsigned int when using), vector(1) store the minimum cost from the best template's best path
 */
vector<double> dtwSynchronous(vector<vector<double>>& inputAduio, vector<vector<vector<double>>>& temsAudio){
    
    vector<double> result(2);
    
    unsigned int inputFrameNum = (unsigned int)inputAduio.size();
    unsigned int temNum = (unsigned int)temsAudio.size();
    
    unsigned int temFrameNum;
    
    vector<vector<double>> col, prevCol;
    
    // get the largest template frames and initialize the columns
    for (unsigned int i = 0; i < temNum; i++) {
        temFrameNum = (unsigned int) temsAudio[i].size();
        vector<double> vecMax(temFrameNum);
        for (unsigned int j = 0; j < temFrameNum; j++) {
            vecMax[j] = UINT_MAX / 2;
        }
        prevCol.push_back(vecMax);
        col.push_back(vecMax);
    }
    
    for (unsigned int i = 0; i < temNum; i++) {
        prevCol[i][0] = dis(inputAduio[0], temsAudio[i][0]);
        prevCol[i][1] = dis(inputAduio[0], temsAudio[i][1]);
    }
    
    for (unsigned int i = 1; i < inputFrameNum; i++) {
        for (unsigned int j = 0; j < temNum; j++) {
            temFrameNum = (unsigned int) temsAudio[j].size();
            unsigned int tem = min({2 * (i + 1), temFrameNum});
            for (unsigned int k = 0; k < tem; k++) {
                if (k == 0) {
                    col[j][k] = prevCol[j][k] + dis(inputAduio[i], temsAudio[j][k]);
                }
                
                else if (k == 1){
                    col[j][k] = min({prevCol[j][k - 1], prevCol[j][k]}) + dis(inputAduio[i], temsAudio[j][k]);
                }
                else
                    col[j][k] = min({prevCol[j][k - 2], prevCol[j][k - 1], prevCol[j][k]}) +  dis(inputAduio[i], temsAudio[j][k]);
            }
            col[j].swap(prevCol[j]);
        }
    }
    
    double bestIndex = 0;
    double bestValue = UINT_MAX / 2;
    
    for (unsigned int i = 0; i < temNum; i++) {
        temFrameNum =(unsigned int) temsAudio[i].size();
        if (prevCol[i][temFrameNum - 1] < bestValue ) {
            bestIndex = i;
            bestValue = prevCol[i][temFrameNum - 1] ;
        }
    }
    
    result[0] = bestIndex;
    result[1] = bestValue;
    
    return result;
}






