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
string segmentPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\Archive\\isolateTem11\\segment.txt";
string variancePath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\Archive\\isolateTem11\\variance.txt";
string transferPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\Archive\\isolateTem11\\transfer.txt";

string wavTemPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\Archive\\isolateTem11\\";
string txtTemPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\Archive\\isolateTem11\\";
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
string segmentTrainPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\hwdata\\model\\segment.txt";
string varianceTrainPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\hwdata\\model\\variance.txt";
string transferTrainPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\hwdata\\model\\transfer.txt";

//test single wav file for project7.2
//string trainWavPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\hwdata\\collection\\train\\";
//string trainTestWavPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\hwdata\\collection\\test\\";

//use single to init k-means
string segmentPathInit = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\hwdata\\init\\segment.txt";
string variancePathInit = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\hwdata\\init\\variance.txt";
string transferPathInit = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\hwdata\\init\\transfer.txt";
string wavTemPathInit = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\hwdata\\init\\";
string txtTemPathInit = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\hwdata\\init\\";

void problem3(vector<vector<vector<double>>> segTemGroup, vector<vector<double>> testInput, vector<vector<vector<double>>> varianceTerm, vector<vector<vector<int>>> countTransfer)
{
	Trie trie;
	TrieNode* root = trie.getRoot();
	for (int i = 0; i < MAX_BRANCH_NUM; i++)
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

void writeSegForTain() {

	vector<vector<string>> dir;
	for(int i = 0; i < TYPE_NUM; i++)
	{
		vector<string> files;
		string format = ".wav";
		GetAllFormatFiles(wavTemPathInit + to_string(i) + "\\", files, format);
		dir.push_back(files);
	}



	vector<vector<vector<double>>> segTemGroup;
	vector<vector<vector<double>>> varianceTerm(TYPE_NUM, vector<vector<double>>(SEG_NUM, vector<double>(DIMENSION)));
	vector<vector<vector<int>>> countTransfer(TYPE_NUM, vector<vector<int>>(SEG_NUM + 1, vector<int>(SEG_NUM)));
	for (int i = 0; i < TYPE_NUM; i++) {
		vector<vector<vector<double>>> temGroup;
		for (int j = 0; j < dir[i].size(); j++) {
			cout << "-----------------------Template " << i << " Instance " << j << "------------------------" << endl;
			string wavpath = wavTemPathInit + to_string(i) + "\\" + dir[i][j];
			//            capture(wavpath);
			vector<vector<double>> temFeature;
			string txtpath = txtTemPathInit + to_string(i) + "\\" + dir[i][j] + "_";
			featureExtractionNew(temFeature, wavpath, txtpath);
			temGroup.push_back(temFeature);

			temFeature.swap(vector<vector<double>>());

			//avoid memory problem
			if (i == (TYPE_NUM - 1) && j == 40)
				break;
		}
		vector<vector<double>> segTem;
		segTem = dtw2hmm(temGroup, varianceTerm[i], countTransfer[i]);
		cout << "You have got the segment template!!!!!!!!!!!!!!!!!!!" << endl;
		segTemGroup.push_back(segTem);

		segTem.swap(vector<vector<double>>());
		temGroup.swap(vector<vector<vector<double>>>());
	}

	cout << segTemGroup.size() << endl;

	ofstream out(segmentPathInit);
	ofstream out2(variancePathInit);
	ofstream out3(transferPathInit);
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
		string wavp = wavpath + files[i];
		string txtp = txtpath + to_string(i);
		vector<vector<double>> testInput;
		featureExtractionNew(testInput, wavp, txtp);
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
					digit = 0;
					break;
				//use digit 10 to represent O
				case 'O':
					digit = 10;
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

//train all wav data for project7.2
void trainAll()
{
	vector<vector<vector<double>>> segTemGroup(TYPE_NUM, vector<vector<double>>(SEG_NUM, vector<double>(DIMENSION)));
	vector<vector<vector<double>>> varianceTerm(TYPE_NUM, vector<vector<double>>(SEG_NUM, vector<double>(DIMENSION)));
	vector<vector<vector<int>>> countTransfer(TYPE_NUM, vector<vector<int>>(SEG_NUM + 1, vector<int>(SEG_NUM)));
	ifstream in(segmentPathInit);
	ifstream in2(variancePathInit);
	ifstream in3(transferPathInit);
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

	for (int i = 0; i < TRAIN_TYPE; i++)
	{
		for (int j = 0; j < TRAIN_NUM; j++)
		{
			//			string wavTestPath = wavTestPathDigits + dir[i] + "\\" + to_string(j) + "\\record.wav";
			string txtTestPath = trainTxtPath + to_string(i) + "result.txt";
			ifstream inTrain(txtTestPath);
			vector<vector<double>> testInput;
			vector<double> singleInput;
			double temp;
			for (int k = 0; !inTrain.eof(); k++)
			{
				if (k == DIMENSION)
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
			inTrain.close();
			testInput.swap(vector<vector<double>>());
			singleInput.swap(vector<double>());
		}
	}

	cout << "load single segment" << endl;

	vector<string> files;
	string format = ".wav";
	GetAllFormatFiles(trainWavPath, files, format);
	vector<vector<int>> digits;
	digits = parseDigit(files);
	//cout << files[3182] << endl;
	//return;
	vector<vector<vector<vector<int>>>> allState;
	vector<vector<vector<vector<int>>>> resultState;
	allState = getAllStateIndex(DIGIT_TYPE, segTemGroup, input, digits, varianceTerm, countTransfer);
	cout << "get all state" << endl;
	vector<vector<vector<double>>> variance(TRAIN_TYPE, vector<vector<double>>(DIGIT_TYPE * SEG_NUM, vector<double>(DIMENSION)));
	vector<vector<vector<int>>> transfer(TRAIN_TYPE, vector<vector<int>>(DIGIT_TYPE * SEG_NUM + 1, vector<int>(DIGIT_TYPE * SEG_NUM)));
	resultState = conDtw2hmm(input, allState, variance, transfer);

	vector<vector<vector<vector<double>>>> variance_G(TRAIN_TYPE, vector<vector<vector<double>>>());
	vector<vector<vector<int>>> transfer_G(TRAIN_TYPE, vector<vector<int>>());
	vector<vector<vector<vector<double>>>> multi_kernel = multiGaussDtw2hmm(input, KERNEL_NUM, resultState, variance_G, transfer_G);

	cout << "get result state" << endl;

	vector<vector<vector<vector<double>>>> varianceSeg(DIGIT_TYPE, vector<vector<vector<double>>>(SEG_NUM, vector<vector<double>>(KERNEL_NUM, vector<double>(DIMENSION))));
	vector<vector<vector<int>>> transferSeg(DIGIT_TYPE, vector<vector<int>>(SEG_NUM + 1, vector<int>(SEG_NUM)));
	vector<vector<vector<vector<double>>>>resultSeg = getTrainFrame_G(resultState, input, digits, varianceSeg, transferSeg, multi_kernel);

	cout << "get result seg" << endl;

	ofstream out(segmentTrainPath);
	ofstream out1(varianceTrainPath);
	ofstream out2(transferTrainPath);
	for (int i = 0; i < DIGIT_TYPE; i++)
	{
		for (int j = 0; j < SEG_NUM; j++)
		{
			for (int p = 0; p < KERNEL_NUM; p++)
			{
				for (int k = 0; k < DIMENSION; k++)
				{
					out << resultSeg[i][j][p][k] << " ";
					out1 << varianceSeg[i][j][p][k] << " ";
				}
				out << endl;
				out1 << endl;
			}
			out << endl;
			out1 << endl;
		}

		for (int j = 0; j < SEG_NUM + 1; j++)
		{
			for (int k = 0; k < SEG_NUM; k++)
			{
				out2 << transferSeg[i][j][k] << " ";
			}
			out2 << endl;
		}

		out << endl;
		out1 << endl;
		out2 << endl;
	}
	out.close();
	out1.close();
	out2.close();
	cout << "store seg" << endl;
}

//run test data in project7.2 and caculate accuracy
void testTrain()
{
	vector<vector<vector<double>>> segTemGroup(TYPE_NUM, vector<vector<double>>(SEG_NUM, vector<double>(DIMENSION)));
	vector<vector<vector<double>>> varianceTerm(TYPE_NUM, vector<vector<double>>(SEG_NUM, vector<double>(DIMENSION)));
	vector<vector<vector<int>>> countTransfer(TYPE_NUM, vector<vector<int>>(SEG_NUM + 1, vector<int>(SEG_NUM)));
	ifstream in(segmentTrainPath);
	ifstream in2(varianceTrainPath);
	ifstream in3(transferTrainPath);
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

	Trie trie;
	TrieNode* root = trie.getRoot();
	for (int i = 0; i < MAX_BRANCH_NUM; i++)
	{
		root->nextBranch[i]->segTemplate = segTemGroup[i];
	}

	vector<vector<vector<vector<double>>>> input(TEST_TYPE, vector<vector<vector<double>>>(TRAIN_NUM, vector<vector<double>>()));

	for (int i = 0; i < TEST_TYPE; i++)
	{
		for (int j = 0; j < TRAIN_NUM; j++)
		{
			string txtTestPath = trainTestTxtPath + to_string(i) + "result.txt";
			ifstream inTrain(txtTestPath);
			vector<vector<double>> testInput;
			vector<double> singleInput;
			double temp;
			for (int k = 0; !inTrain.eof(); k++)
			{
				if (k == DIMENSION)
				{
					k = 0;
					testInput.push_back(singleInput);
					singleInput.clear();
				}
				inTrain >> temp;
				singleInput.push_back(temp);
			}
			input[i][j] = testInput;
			inTrain.close();
			testInput.swap(vector<vector<double>>());
			singleInput.swap(vector<double>());
		}
	}

	cout << "load single segment" << endl;

	vector<string> files;
	string format = ".wav";
	GetAllFormatFiles(trainTestWavPath, files, format);
	vector<vector<int>> digits;
	digits = parseDigit(files);

	long correctWordByState = 0;
	long correctWordRandom = 0;
	long correctSen = 0;
	long wholeWord = 0;
	long singleWord = 0;
	long stateSingle = 0;
	long randomSingle = 0;
	for (int i = 0; i < TEST_TYPE; i++)
	{
		for (int j = 0; j < TRAIN_NUM; j++)
		{
			stack<int> resultDigitByState;
			resultDigitByState = DigitRecognitionGussian(digits[i].size(), input[i][j], segTemGroup, varianceTerm, countTransfer);

			stack<int> resultDigitRandom;
			resultDigitRandom = RestrictPhoneGuassian(trie, input[i][j], varianceTerm, countTransfer);

			//int count = 0;
			//bool senFlag = true;
			vector<int> resultByState;
			vector<int> resultRandom;

			while (!resultDigitByState.empty())
			{
				resultByState.push_back(resultDigitByState.top());
				resultDigitByState.pop();
			}

			while (!resultDigitRandom.empty())
			{
				resultRandom.push_back(resultDigitRandom.top());
				resultDigitRandom.pop();
			}

			//calculate single accuracy
			if (digits[i].size() == 1)
				singleWord++;
			if(digits[i].size() == 1 && resultByState.size() == 1)
			{
				if (digits[i][0] == resultByState[0])
					stateSingle++;
			}
			if(digits[i].size() == 1 && resultRandom.size() == 1)
			{
				if (digits[i][0] == resultRandom[0])
					randomSingle++;
			}

			cout << "correct digits: " << endl;
			for (int p = 0; p < digits[i].size(); p++)
				cout << digits[i][p] << " ";
			cout << endl;

			cout << "recognize digits by state: " << endl;
			for (int p = 0; p < resultByState.size(); p++)
				cout << resultByState[p] << " ";
			cout << endl;

			cout << "recognize digits by random: " << endl;
			for (int p = 0; p < resultRandom.size(); p++)
				cout << resultRandom[p] << " ";
			cout << endl << endl;

			unsigned int errorByState = pureLevenshteinDistance(resultByState, digits[i]);
			unsigned int errorRandom = pureLevenshteinDistance(resultRandom, digits[i]);
			wholeWord += digits[i].size();
			correctWordByState += (digits[i].size() - errorByState) > 0 ? (digits[i].size() - errorByState) : (errorByState - digits[i].size());
			correctWordRandom += (digits[i].size() - errorRandom) > 0 ? (digits[i].size() - errorRandom) : (errorRandom - digits[i].size());
			/*
			while(!resultDigit.empty())
			{
				int di = resultDigit.top();
				if(di == digits[i][count])
				{
					correctWord++;
				}
				if(di != digits[i][count])
				{
					senFlag = false;
				}
				count++;
				resultDigit.pop();
			}
			wholeWord += count;
			if(senFlag)
			{
				correctSen++;
			}
			*/
		}
	}
	cout << "word accuracy by state machine: " << (double)correctWordByState / wholeWord << endl;
	cout << "word accuracy by random: " << (double)correctWordRandom / wholeWord << endl;
	cout << "single word accuracy by state machine: " << (double)stateSingle / singleWord << endl;
	cout << "single word accuracy by random: " << (double)randomSingle / singleWord << endl;
	//cout << "sentence accuracy: " << (double)correctSen / TEST_TYPE << endl;
	return;
}

//train all wav data for project7.2
void trainAllOther()
{
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

	vector<string> files;
	string format = ".wav";
	GetAllFormatFiles(trainWavPath, files, format);
	vector<vector<int>> digits;
	digits = parseDigit(files);

	vector<vector<vector<vector<double>>>> input(TRAIN_TYPE, vector<vector<vector<double>>>(TRAIN_NUM, vector<vector<double>>()));

	for (int i = 0; i < TRAIN_TYPE; i++)
	{
		for (int j = 0; j < TRAIN_NUM; j++)
		{
			//			string wavTestPath = wavTestPathDigits + dir[i] + "\\" + to_string(j) + "\\record.wav";
			string txtTestPath = trainWavPath + files[i].substr(0, files[i].size() - 4);
			ifstream inTrain(txtTestPath);
			vector<vector<double>> testInput;
			vector<double> singleInput;
			double temp;
			for (int k = 0; !inTrain.eof(); k++)
			{
				if (k == DIMENSION)
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
			inTrain.close();
			testInput.swap(vector<vector<double>>());
			singleInput.swap(vector<double>());
		}
	}

	cout << "load single segment" << endl;


	//cout << files[3182] << endl;
	//return;
	vector<vector<vector<vector<int>>>> allState;
	vector<vector<vector<vector<int>>>> resultState;
	allState = getAllStateIndex(DIGIT_TYPE, segTemGroup, input, digits, varianceTerm, countTransfer);
	cout << "get all state" << endl;
	vector<vector<vector<double>>> variance(TRAIN_TYPE, vector<vector<double>>(DIGIT_TYPE * SEG_NUM, vector<double>(DIMENSION)));
	vector<vector<vector<int>>> transfer(TRAIN_TYPE, vector<vector<int>>(DIGIT_TYPE * SEG_NUM + 1, vector<int>(DIGIT_TYPE * SEG_NUM)));
	resultState = conDtw2hmm(input, allState, variance, transfer);

	cout << "get result state" << endl;

	vector<vector<vector<double>>> varianceSeg(DIGIT_TYPE, vector<vector<double>>(SEG_NUM, vector<double>(DIMENSION)));
	vector<vector<vector<int>>> transferSeg(DIGIT_TYPE, vector<vector<int>>(SEG_NUM + 1, vector<int>(SEG_NUM)));
	vector<vector<vector<double>>>resultSeg = getTrainFrame(resultState, input, digits, varianceSeg, transferSeg);

	cout << "get result seg" << endl;

	ofstream out(segmentTrainPath);
	ofstream out1(varianceTrainPath);
	ofstream out2(transferTrainPath);
	for (int i = 0; i < DIGIT_TYPE; i++)
	{
		for (int j = 0; j < SEG_NUM; j++)
		{
			for (int k = 0; k < DIMENSION; k++)
			{
				out << resultSeg[i][j][k] << " ";
				out1 << varianceSeg[i][j][k] << " ";
			}
			out << endl;
			out1 << endl;
		}

		for (int j = 0; j < SEG_NUM + 1; j++)
		{
			for (int k = 0; k < SEG_NUM; k++)
			{
				out2 << transferSeg[i][j][k] << " ";
			}
			out2 << endl;
		}

		out << endl;
		out1 << endl;
		out2 << endl;
	}
	out.close();
	out1.close();
	out2.close();
	cout << "store seg" << endl;
}

//run test data in project7.2 and caculate accuracy
void testTrainOther()
{
	vector<vector<vector<double>>> segTemGroup(TYPE_NUM, vector<vector<double>>(SEG_NUM, vector<double>(DIMENSION)));
	vector<vector<vector<double>>> varianceTerm(TYPE_NUM, vector<vector<double>>(SEG_NUM, vector<double>(DIMENSION)));
	vector<vector<vector<int>>> countTransfer(TYPE_NUM, vector<vector<int>>(SEG_NUM + 1, vector<int>(SEG_NUM)));
	ifstream in(segmentTrainPath);
	ifstream in2(varianceTrainPath);
	ifstream in3(transferTrainPath);
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

	Trie trie;
	TrieNode* root = trie.getRoot();
	for (int i = 0; i < MAX_BRANCH_NUM; i++)
	{
		root->nextBranch[i]->segTemplate = segTemGroup[i];
	}

	vector<vector<vector<vector<double>>>> input(TEST_TYPE, vector<vector<vector<double>>>(TRAIN_NUM, vector<vector<double>>()));

	vector<string> files;
	string format = ".wav";
	GetAllFormatFiles(trainTestWavPath, files, format);
	vector<vector<int>> digits;
	digits = parseDigit(files);

	for (int i = 0; i < TEST_TYPE; i++)
	{
		for (int j = 0; j < TRAIN_NUM; j++)
		{
			string txtTestPath = trainTestWavPath + files[i].substr(0, files[i].size() - 4);
			ifstream inTrain(txtTestPath);
			vector<vector<double>> testInput;
			vector<double> singleInput;
			double temp;
			for (int k = 0; !inTrain.eof(); k++)
			{
				if (k == DIMENSION)
				{
					k = 0;
					testInput.push_back(singleInput);
					singleInput.clear();
				}
				inTrain >> temp;
				singleInput.push_back(temp);
			}
			input[i][j] = testInput;
			inTrain.close();
			testInput.swap(vector<vector<double>>());
			singleInput.swap(vector<double>());
		}
	}

	cout << "load single segment" << endl;

	long correctWordByState = 0;
	long correctWordRandom = 0;
	long correctSen = 0;
	long wholeWord = 0;
	long singleWord = 0;
	long stateSingle = 0;
	long randomSingle = 0;
	for (int i = 0; i < TEST_TYPE; i++)
	{
		for (int j = 0; j < TRAIN_NUM; j++)
		{
			stack<int> resultDigitByState;
			resultDigitByState = DigitRecognitionGussian(digits[i].size(), input[i][j], segTemGroup, varianceTerm, countTransfer);

			stack<int> resultDigitRandom;
			resultDigitRandom = RestrictPhoneGuassian(trie, input[i][j], varianceTerm, countTransfer);

			//int count = 0;
			//bool senFlag = true;
			vector<int> resultByState;
			vector<int> resultRandom;

			while (!resultDigitByState.empty())
			{
				resultByState.push_back(resultDigitByState.top());
				resultDigitByState.pop();
			}

			while (!resultDigitRandom.empty())
			{
				resultRandom.push_back(resultDigitRandom.top());
				resultDigitRandom.pop();
			}

			//calculate single accuracy
			if (digits[i].size() == 1 && resultByState.size() == 1 && resultRandom.size() == 1)
			{
				singleWord++;
				if (digits[i][0] == resultByState[0])
					stateSingle++;
				if (digits[i][0] == resultRandom[0])
					randomSingle++;
			}

			cout << "correct digits: " << endl;
			for (int p = 0; p < digits[i].size(); p++)
				cout << digits[i][p] << " ";
			cout << endl;

			cout << "recognize digits by state: " << endl;
			for (int p = 0; p < resultByState.size(); p++)
				cout << resultByState[p] << " ";
			cout << endl;

			cout << "recognize digits by random: " << endl;
			for (int p = 0; p < resultRandom.size(); p++)
				cout << resultRandom[p] << " ";
			cout << endl << endl;

			unsigned int errorByState = pureLevenshteinDistance(resultByState, digits[i]);
			unsigned int errorRandom = pureLevenshteinDistance(resultRandom, digits[i]);
			wholeWord += digits[i].size();
			correctWordByState += (digits[i].size() - errorByState) > 0 ? (digits[i].size() - errorByState) : (errorByState - digits[i].size());
			correctWordRandom += (digits[i].size() - errorRandom) > 0 ? (digits[i].size() - errorRandom) : (errorRandom - digits[i].size());
			/*
			while(!resultDigit.empty())
			{
			int di = resultDigit.top();
			if(di == digits[i][count])
			{
			correctWord++;
			}
			if(di != digits[i][count])
			{
			senFlag = false;
			}
			count++;
			resultDigit.pop();
			}
			wholeWord += count;
			if(senFlag)
			{
			correctSen++;
			}
			*/
		}
	}
	cout << "word accuracy by state machine: " << (double)correctWordByState / wholeWord << endl;
	cout << "word accuracy by random: " << (double)correctWordRandom / wholeWord << endl;
	cout << "single word accuracy by state machine: " << (double)stateSingle / singleWord << endl;
	cout << "single word accuracy by random: " << (double)randomSingle / singleWord << endl;
	//cout << "sentence accuracy: " << (double)correctSen / TEST_TYPE << endl;
	return;
}

int main()
{
	trainAll();
//	trainAllOther();
//	testTrainOther();
//	testTrain();
//	writeSegForTain();
	return 0;
	vector<string> files;
	string format = ".wav";
	GetAllFormatFiles(trainTestWavPath, files, format);
//	cout << files[1128];
	testReadDir(files, trainTestWavPath, trainTestTxtPath);
	//testSingleWav(370, files);
//	writeSeg();
	return 0;
	vector<int> a = { 1, 2, 3, 4 };
	vector<int> b = { 2, 3, 4, 5, 6 };
	long x = pureLevenshteinDistance(a, b);
	long y = absBeamLevenshteinDistance(a, b);
	long z = beamLevenshteinDistance(a, b);
	cout << "pure: " << x << " abs: " << y << " beam: " << z << endl;
	return 0;
}