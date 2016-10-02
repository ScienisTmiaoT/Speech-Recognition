#ifndef _DTW_H_
#define _DTW_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <iomanip>

using namespace std;

#define DTW_BEAM (100)
#define TABLE_GAP (10)

double dtw(vector<vector<double>>& inputAduio, vector<vector<double>>& temAduio);
vector<double> dtwSynchronous(vector<vector<double>>& inputAduio, vector<vector<vector<double>>>& temsAudio);
double beamDTW(vector<vector<double>>& inputAudio, vector<vector<double>>& temAudio);
double printDTWPath(vector<vector<double>>& s1, vector<vector<double>>& s2, ofstream& out);

#endif