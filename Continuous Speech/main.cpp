#include <iostream>
#include <fstream>
#include "ContinuousPhone.h"
#include "readwave.h"
#include "ReadDir.h"
#include "featureExtractionNew.h"

using namespace std;

string wavTestPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\record.wav";
string txtTestPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\";
//used to save segment result
string segmentPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\Archive\\isolateTem\\segment.txt";
string variancePath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\Archive\\isolateTem\\variance.txt";
string transferPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\Archive\\isolateTem\\transfer.txt";

string wavTemPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\Archive\\isolateTem\\";
string txtTemPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\Archive\\isolateTem\\";
string wavInputPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\test 4.0\\input";
string txtInputPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\test 4.0\\input";

//used to record train data project 7.1
string wavTestPathDigits = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\Archive\\requiredTem\\";
string txtTestPathDigits = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\Archive\\requiredTem\\";
string segTestPathDigits = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\Archive\\requiredTem\\segment.txt";

//used to record train data project 7.2
string trainWavPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\hwdata\\train\\";
string trainTestWavPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\hwdata\\test\\";
string trainTxtPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\hwdata\\train_txt\\";
string trainTestTxtPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\hwdata\\test_txt\\";

void problem3(vector<vector<vector<double>>> segTemGroup, vector<vector<double>> testInput, vector<vector<vector<double>>> varianceTerm, vector<vector<vector<int>>> countTransfer)
{
	Trie trie;
	TrieNode* root = trie.getRoot();
	for (int i = 0; i < MAX_BRANCH_NUM - 1; i++)
	{
		root->nextBranch[i]->segTemplate = segTemGroup[i];
	}
	RestrictPhone(trie, testInput, varianceTerm, countTransfer);
	cout << endl;
}

void writeSeg() {
	vector<vector<vector<double>>> segTemGroup;
	vector<vector<vector<double>>> varianceTerm(TYPE_NUM, vector<vector<double>>(SEG_NUM, vector<double>(DIMENSION)));
	vector<vector<vector<int>>> countTransfer(TYPE_NUM, vector<vector<int>>(SEG_NUM + 1, vector<int>(SEG_NUM)));
	for (int i = 0; i < TYPE_NUM; i++) {
		vector<vector<vector<double>>> temGroup;
		for (int j = 0; j < TEM_NUM; j++) {
			cout << "-----------------------Template " << i << " Instance " << j << "------------------------" << endl;
			string wavpath = wavTemPath + to_string(i) + "\\" + to_string(j) + "\\record.wav";
			//            capture(wavpath);
			vector<vector<double>> temFeature;
			string txtpath = txtTemPath + to_string(i) + "\\" + to_string(j) + "\\";
			featureExtractionOld(temFeature, wavpath, txtpath);
			temGroup.push_back(temFeature);
		}
		vector<vector<double>> segTem;
		segTem = dtw2hmm(temGroup, varianceTerm[i], countTransfer[i]);
		cout << "You have got the segment template!!!!!!!!!!!!!!!!!!!" << endl;
		segTemGroup.push_back(segTem);
	}


	ofstream out(segmentPath);
	ofstream out2(variancePath);
	ofstream out3(transferPath);
	for (int i = 0; i < TYPE_NUM; i++)
	{
		cout << "template " << i << endl;

		for (int j = 0; j < SEG_NUM + 1; j++) {
			for (int k = 0; k < SEG_NUM; k++) {
				out3 << countTransfer[i][j][k] << " ";
			}
			out3 << endl;
		}

		for (int j = 0; j < SEG_NUM; j++)
		{
			cout << "state " << j << endl;
			for (int k = 0; k < DIMENSION; k++)
			{
				out << segTemGroup[i][j][k] << " ";
				out2 << varianceTerm[i][j][k] << " ";
			}
			out << endl;
			out2 << endl;
		}
		out << endl;
		out2 << endl;
		out3 << endl;
	}
	out.close();
	out2.close();
	out3.close();
}

void trainDigits() {
	vector<vector<vector<double>>> segTemGroup(TYPE_NUM, vector<vector<double>>(SEG_NUM, vector<double>(DIMENSION)));
	vector<vector<vector<double>>> varianceTerm(TYPE_NUM, vector<vector<double>>(SEG_NUM, vector<double>(DIMENSION)));
	vector<vector<vector<int>>> countTransfer(TYPE_NUM, vector<vector<int>>(SEG_NUM + 1, vector<int>(SEG_NUM)));
	ifstream in(segmentPath);
	ifstream in2(variancePath);
	ifstream in3(transferPath);
	for (int i = 0; i < TYPE_NUM; i++)
	{
		for (int j = 0; j < SEG_NUM; j++)
		{
			for (int k = 0; k < DIMENSION; k++)
			{
				in >> segTemGroup[i][j][k];
				in2 >> varianceTerm[i][j][k];
			}
		}
		for (int j = 0; j < SEG_NUM + 1; j++) {
			for (int k = 0; k < SEG_NUM; k++) {
				in3 >> countTransfer[i][j][k];
			}
		}
	}
	in.close();
	in2.close();
	in3.close();

	vector<vector<vector<vector<double>>>> input(TRAIN_TYPE, vector<vector<vector<double>>>(TRAIN_NUM, vector<vector<double>>()));
	

	string dir[] = { "0123456789", "0987654321", "1234567890", "1357902468", "8642097531", "9876543210" };
	for(int i = 0; i < TRAIN_TYPE; i++)
	{
		for(int j = 0; j < TRAIN_NUM; j++)
		{
//			string wavTestPath = wavTestPathDigits + dir[i] + "\\" + to_string(j) + "\\record.wav";
			string txtTestPath = txtTestPathDigits + dir[i] + "\\" + to_string(j) + "\\result.txt";
			ifstream inTrain(txtTestPath);
			vector<vector<double>> testInput;
			vector<double> singleInput;
			double temp;
			for(int k = 0; !inTrain.eof(); k++)
			{
				if(k == DIMENSION)
				{
					k = 0;
					testInput.push_back(singleInput);
					singleInput.clear();
				}
				inTrain >> temp;
				singleInput.push_back(temp);
			}
//			featureExtractionTwo(testInput, wavTestPath, txtTestPath);
			input[i][j] = testInput;
		}
	}
	vector<vector<vector<vector<int>>>> allState = getAllStateIndex(DIGIT_NUM10, segTemGroup, input, varianceTerm, countTransfer);
	vector<vector<vector<double>>> variance(TRAIN_TYPE, vector<vector<double>>(DIGIT_NUM * SEG_NUM, vector<double>(DIMENSION)));
	vector<vector<vector<int>>> transfer(TRAIN_TYPE, vector<vector<int>>(DIGIT_NUM * SEG_NUM + 1, vector<int>(DIGIT_NUM * SEG_NUM)));
	vector<vector<vector<vector<int>>>> resultState = conDtw2hmm(input, allState, variance, transfer);
	vector<vector<vector<double>>>resultSeg = getSegFrame(resultState, input);

	ofstream out(segTestPathDigits);
	for(int i = 0; i < DIGIT_NUM; i++)
	{
		for(int j = 0; j < SEG_NUM; j++)
		{
			for (int k = 0; k < DIMENSION; k++)
			{
				out << resultSeg[i][j][k] << " ";
			}
			out << endl;
		}
		out << endl;
	}
	out.close();
	cout << endl;
}

void readSeg() {
	vector<vector<vector<double>>> segTemGroup(DIGIT_NUM, vector<vector<double>>(SEG_NUM, vector<double>(DIMENSION)));
	//    vector<vector<vector<double>>> varianceTerm(TYPE_NUM, vector<vector<double>>(SEG_NUM, vector<double>(DIMENSION)));
	//    vector<vector<vector<int>>> countTransfer(TYPE_NUM, vector<vector<int>>(SEG_NUM + 1, vector<int>(SEG_NUM)));

	ifstream in(segTestPathDigits);
	//    ifstream in2(variancePath);
	//    ifstream in3(transferPath);
	for (int i = 0; i < TYPE_NUM; i++)
	{
		for (int j = 0; j < SEG_NUM; j++)
		{
			for (int k = 0; k < DIMENSION; k++)
			{
				in >> segTemGroup[i][j][k];
				//                in2 >> varianceTerm[i][j][k];
			}
		}
		//        for (int j = 0; j < SEG_NUM + 1; j++) {
		//            for (int k = 0; k < SEG_NUM; k++) {
		//                in3 >> countTransfer[i][j][k];
		//            }
		//        }
	}
	in.close();
	//    in2.close();
	//    in3.close();

	vector<vector<double>> testInput;
	//    featureExtractionTwo(testInput, wavTestPath, txtTestPath);
	featureExtractionTwoOld(testInput, wavTestPath, txtTestPath);

	vector<vector<vector<double>>> varianceTerm;
	vector<vector<vector<int>>> countTransfer;

	cout << "problem3 result : ";
	problem3(segTemGroup, testInput, varianceTerm, countTransfer);

}

// test segmental k-mean
void part6() {
	vector<vector<vector<double>>> segTemGroup(TYPE_NUM, vector<vector<double>>(SEG_NUM, vector<double>(DIMENSION)));
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


	ifstream in(segmentPath);
	for (int i = 0; i < TYPE_NUM; i++)
	{
		for (int j = 0; j < SEG_NUM; j++)
		{
			for (int k = 0; k < DIMENSION; k++)
			{
				in >> segTemGroup[i][j][k];
			}
		}
	}
	in.close();

	/*
	for (int i = 0; i < TYPE_NUM; i++)
	{
	cout << "template " << i << endl;
	for (int j = 0; j < SEG_NUM; j++)
	{
	cout << "state " << j << endl;
	for (int k = 0; k < DIMENSION; k++)
	{
	cout << segTemGroup[i][j][k] << " ";
	}
	cout << endl;
	}
	cout << endl;
	}
	*/

	/*
	vector<vector<double>> testInput;
	featureExtractionTwo(testInput, wavTestPath, txtTestPath);
	Trie trie;
	TrieNode* root = trie.getRoot();
	for (int i = 0; i < MAX_BRANCH_NUM - 1; i++)
	{
	root->nextBranch[i]->segTemplate = segTemGroup[i];
	}
	RestrictPhone(trie, testInput);
	cout << endl;
	*/
}

// first arg: path
// second arg: file name vector
// store in Allfiles.txt
int testReadDir(vector<string>& files, string wavpath, string txtpath)
{
	//string result = trainTxtPath + "testWavName.txt";
	//const char * distAll = result.c_str();

	//read all file, include child file
	//GetAllFiles(filePath, files);

	int vec_len = files.size();
	for(int i = 0; i < vec_len; i++)
	{
		wavpath += files[i];
		txtpath += to_string(i);
		vector<vector<double>> testInput;
		featureExtractionNew(testInput, wavpath, txtpath);
		cout << "file index : " << i << " file name: " << files[i] << endl;
	} 
	

	//string wavpath = trainWavPath + files[50];
	//string txtpath = trainTxtPath;
	//vector<vector<double>> testInput;
	//featureExtractionNew(testInput, wavpath, txtpath);
	/*
	//store path + filename
	ofstream ofn(distAll);
	int size = files.size();
	//ofn << size << endl;
	for (int i = 0;i<size;i++)
	{
		ofn << files[i] << endl;
		//cout << files[i] << endl;
	}
	ofn.close();
	*/
	return 0;
}

int testSingleWav(int index, vector<string>& files)
{
	//string result = trainTxtPath + "testWavName.txt";
	//const char * distAll = result.c_str();
	string wavpath = trainWavPath + files[index];
	string txtpath = trainTxtPath;
	vector<vector<double>> testInput;
	featureExtractionNew(testInput, wavpath, txtpath);
	return 0;
}

//parse digit from file name
vector<vector<int>> parseDigit(vector<string>& files)
{
	int vec_len = files.size();
	vector<vector<int>> result(vec_len, vector<int>());
	for(int i = 0; i < vec_len; i++)
	{
		int index = 0;
		int end = files[i].length() - 5;
		for(int j = 0; j < end; j++)
		{
			if(files[i][j] == '_')
			{
				index = j + 1;
				break;
			}
		}
		for(int j = index; j < end; j++)
		{
			int digit = 0;
			char c = files[i][j];
			switch(c)
			{
				case 'Z':
				case 'O':
					digit = 0;
					break;
				default:
					digit = c - '0';
					break;
			}
			result[i].push_back(digit);
		}
	}
	return result;
}
int main()
{
	vector<string> files;
	string format = ".wav";
	GetAllFormatFiles(trainWavPath, files, format);

	vector<vector<int>> digits;
	digits = parseDigit(files);
	cout << digits.size() << endl;
	for(int i = 0; i < digits.size(); i++)
	{
		for (int j = 0; j < digits[i].size(); j++)
			cout << digits[i][j] << " ";
		cout << endl;
	}
//	testReadDir(files, trainWavPath, trainTxtPath);
	//testSingleWav(370, files);
	return 0;
}