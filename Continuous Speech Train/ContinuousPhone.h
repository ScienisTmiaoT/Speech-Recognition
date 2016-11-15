#ifndef ContinuousPhone
#define ContinuousPhone

#include <math.h>
#include <stack>
#include "Trie.h"
#include "dtw.h"
#include "featureExtractionOld.h"
#include "seg_K_mean.h"

#define PENALTY  (60)
#define TYPE_NUM    (11)
#define TEM_NUM     (5)
#define INPUT_NUM   (10)
#define DIGIT_NUM10  (10)
#define DIGIT_NUM7 (7)
#define DIGIT_NUM4 (4)

//one template ten digits
#define DIGIT_NUM (10)
//six kinds of train data
#define TRAIN_TYPE (8440)
//five same train data
#define TRAIN_NUM (1)
//eleven digits in trainning
#define DIGIT_TYPE (11)
//test data number
#define TEST_TYPE (1000)
//used to caculate accuracy
#define BEAM (1)
#define ABSOLUTE_BEAM (2)

double Dis(vector<double>& matrix1, vector<double>& matrix2);
double costUtil(vector<double>& vec, double c, int& pos);
stack<int> RestrictPhone(Trie& trie, vector<vector<double>>& input, vector<vector<vector<double>>>& varianceTerm, vector<vector<vector<int>>>& countTransfer);

stack<int> backTrace(vector<vector<double>>& input, vector<vector<vector<int>>>& backTable, vector<double>& last);

vector<vector<vector<double>>> getContinuousSeg(int digit_num, vector<vector<vector<double>>>& segTemGroup, vector<vector<double>>& input, vector<vector<vector<double>>>& varianceTerm, vector<vector<vector<int>>>& countTransfer);

vector<vector<int>> getStateIndex(int digit_num, vector<vector<vector<double>>>& segTemGroup, vector<vector<double>>& input, vector<vector<vector<double>>>& varianceTerm, vector<vector<vector<int>>>& countTransfer);

vector<vector<vector<vector<int>>>> getAllStateIndex(int digit_num, vector<vector<vector<double>>>& segTemGroup, vector<vector<vector<vector<double>>>>& input, vector<vector<int>>& digits, vector<vector<vector<double>>>& varianceTerm, vector<vector<vector<int>>>& countTransfer);

vector<vector<vector<double>>> getSegFrame(vector<vector<vector<vector<int>>>>& allState, vector<vector<vector<vector<double>>>>& input);

vector<vector<vector<double>>> getTrainFrame(vector<vector<vector<vector<int>>>>& allState, vector<vector<vector<vector<double>>>>& input, vector<vector<int>> digits, vector<vector<vector<double>>>& varianceSeg, vector<vector<vector<int>>>& transferSeg);

stack<int> DigitRecognition(int digit_num, vector<vector<double>>& input, vector<vector<vector<double>>>& segTemGroup, vector<vector<vector<double>>>& varianceTerm, vector<vector<vector<int>>>& countTransfer);

stack<int> getDigit(int digit_num, vector<vector<double>>& input, vector<vector<vector<double>>>& segTemGroup, vector<vector<vector<double>>>& varianceTerm, vector<vector<vector<int>>>& countTransfer);

stack<int> DigitRecognitionGussian(int digit_num, vector<vector<double>>& input, vector<vector<vector<double>>>& segTemGroup, vector<vector<vector<double>>> &varianceTerm, vector<vector<vector<int>>>& countTransfer);

stack<int> DigitGussianForTrain(int digit_num, vector<vector<double>>& input, vector<vector<vector<double>>>& segTemGroup, vector<vector<vector<double>>> &varianceTerm, vector<vector<vector<int>>>& countTransfer);

stack<int> getRandomDigit(Trie& trie, vector<vector<double>>& input, vector<vector<vector<double>>>& varianceTerm, vector<vector<vector<int>>>& countTransfer);

stack<int> RestrictPhoneGuassian(Trie& trie, vector<vector<double>>& input, vector<vector<vector<double>>>& varianceTerm, vector<vector<vector<int>>>& countTransfer);

stack<int> backTraceRandom(vector<vector<double>>& input, vector<vector<vector<int>>>& backTable, vector<double>& last);

unsigned int beamLevenshteinDistance(const vector<int>& s1, const vector<int>& s2);

unsigned int pureLevenshteinDistance(const vector<int>& s1, const vector<int>& s2);

unsigned int absBeamLevenshteinDistance(const vector<int>& s1, const vector<int>& s2);

#endif

