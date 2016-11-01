#include <iostream>
#include "ContinuousPhone.h"
#include "readwave.h"

using namespace std;

string wavTestPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\record.wav";
string txtTestPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\";


string wavTemPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\test 4.0\\template";
string txtTemPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\test 4.0\\template";
string wavInputPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\test 4.0\\input";
string txtInputPath = "C:\\Users\\Administrator\\Desktop\\Current\\Continuous Speech\\test 4.0\\input";



// test segmental k-mean
int main(){
	vector<vector<vector<double>>> segTemGroup;
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

	vector<vector<double>> testInput;
	featureExtraction(testInput, wavTestPath, txtTestPath);
	Trie trie;
	TrieNode* root = trie.getRoot();
	for(int i = 0; i < MAX_BRANCH_NUM - 1; i++)
	{
		root->nextBranch[i]->segTemplate = segTemGroup[i];
	}

	int length = testInput.size();
	vector<int> minNode(INT_MAX / 2, length);
	RestrictPhone(trie, testInput, minNode);
	for (int i = 0; i < length; i++)
		cout << minNode[i] << " ";
	cout << endl;
	return 0;
}












/*
int main()
{
	short* dataWave;    // store the original wave data
	int numSample;
	int sampleRate;
	string wavpath = "record.wav";
	const char *wavFile = wavpath.c_str();
	// read in the wave data
	dataWave = ReadWavFile(wavFile, &numSample, &sampleRate);
	cout << "numSample " << numSample << endl;
	int start = pruneFrameFromStart(dataWave, numSample);
	int end = pruneFrameFromEnd(dataWave, numSample);
	dataWave += start;
	numSample = end - start;
	cout << "numSample " << numSample << endl;
    char writePath[] = "record_prune.wav";
	WriteWave(writePath, dataWave, numSample, sampleRate);
	return 0;
}

*/



/*
int main(int argc, const char * argv[]) {
    
    
    getResult();      // get the result of dtw
    getSynDTWResult(); // get the Syn DTW
    getSegTem();        // get the result of segmental k-mean
    testSegTem();       // test segmental k-mean

}
*/

//void getTem1(vector<vector<vector<double>>>& temGroup){
//    string wavTemPath = "/Users/hty/desktop/testingData/test 4.0/template";
//    string txtTemPath = "/Users/hty/desktop/testingData/test 4.0/template";
//    for (unsigned int i = 0; i < TYPE_NUM; i++) {
//        //    for (unsigned int i = 0; i < 1; i++) {
//        for(unsigned int j = 0; j < TEM_NUM; j ++){
//            cout << "-----------------------Template " << i << " Instance " << j <<"------------------------" << endl;
//            string wavpath = wavTemPath +  to_string(i) + "/" + to_string(j)+ "/record.wav";
//            
//            //            capture(wavpath);
//            
//            //            cout << "Enter 0 to go on, otherwise to record again"<< endl;
//            //            int out = getchar();
//            //            while (out != 48) {
//            //                cout << "Enter 0 to go on, otherwise to record again"<< endl;
//            //                out = getchar();
//            //                capture(wavpath);
//            //            }
//            
//            
//            vector<vector<double>> temFeature;
//            string txtpath = txtTemPath + to_string(i) +  "/" + to_string(j) +  "/";
//            featureExtraction(temFeature, wavpath, txtpath);
//            temGroup.push_back(temFeature);
//        }
//    }
//}
//
//
//void syTest(){
//    string wavInputPath = "/Users/hty/desktop/testingData/test 4.0/input";
//    string txtInputPath = "/Users/hty/desktop/testingData/test 4.0/input";
//    vector<vector<vector<double>>> temGroup;
//    getTem1(temGroup);
//    
//    vector<vector<double>> input;
//    string wavPath = wavInputPath + to_string(5) + "/" + to_string(9) + "/record.wav";
//    string txtPath = txtInputPath + to_string(5) + "/" + to_string(9) + "/";
//    featureExtraction(input, wavPath, txtPath);
//    
//    ofstream out("/Users/hty/desktop/path.txt");
//    printDTWPath(input, temGroup[6], out);
//}

    
//    cout << "-------------------------TEMPLATE--------------------------" << endl;
////    getchar();
//    
//    string wavFilePath1 = "/Users/hty/desktop/testingData/template/record.wav";
////    capture(wavFilePath1);           // capture data and write it to a wav file
//    
//    
//    cout << "Press any key to get the tem feature." << endl;
//    getchar();
//    
//    vector<vector<double>> temFeature;
//    string txtFilePath1 = "/Users/hty/desktop/testingData/template/";
//    featureExtraction(temFeature, wavFilePath1, txtFilePath1);   // write txt file to the disk in order to watch
//    
//    
//
//    cout << "-------------------------INPUT1--------------------------" << endl;
//    cout << "Press any key to record the input" << endl;
////    getchar();
//    
//    string wavFilePath2 = "/Users/hty/desktop/testingData/input1/record.wav";
////    capture(wavFilePath2);           // capture data and write it to a wav file
//    
//    
////    cout << "Press any key to get the input feature." << endl;
////    getchar();
//    
//    vector<vector<double>> inputFeature;
//    string txtFilePath2 = "/Users/hty/desktop/testingData/input1/";
//    featureExtraction(inputFeature, wavFilePath2, txtFilePath2);   // write txt file to the disk in order to watch
//    
//
//    
//    cout << "Press any key to record the input" << endl;
//    getchar();
//    
//    
//
//    //------------------------------------INPUT 2--------------------------
//    cout << "-------------------------INPUT2--------------------------" << endl;
//    string wavFilePath3 = "/Users/hty/desktop/testingData/input2/record.wav";
////    capture(wavFilePath3);           // capture data and write it to a wav file
//    
//    
////    cout << "Press any key to get the input feature." << endl;
////    getchar();
//    
//    vector<vector<double>> inputFeature2;
//    string txtFilePath3 = "/Users/hty/desktop/testingData/input2/";
//    featureExtraction(inputFeature2, wavFilePath3, txtFilePath3);   // write txt file to the disk in order to watch
//    
//    
//    
//    
//    
//    
//    cout << endl;
//    double cost = dtw(inputFeature, temFeature);
//    cout << "The first cost value = " << cost << endl;
//    
//    cout << endl;
//    double cost2 = dtw(inputFeature2, temFeature);
//    cout << "The second cost value = " << cost2 << endl;
//    cout << " " << endl;


