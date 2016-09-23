#include "spellCheck.h"

vector<string> story;
vector<string> dict;
vector<string> storyCorrect;
//edit distance of input and dict word
unsigned int distances[STORY_SIZE];
//mapping of dict word id to input word id
int distanceId[STORY_SIZE];

void convertFile(string filePath, string inFileName, string outFileName) {
	ofstream out(filePath + outFileName);
	ifstream in(filePath + inFileName);
	if (!in.is_open()) {
		cout << "fail to open file in convertFile" << endl;
		exit(1);
	}
	char buffer[BUFFER_SIZE] = { '\0' };
	in.read(buffer, BUFFER_SIZE);
	in.close();
	int i = 0;
	while (buffer[i] != '\0') {
		if (buffer[i] >= 65 && buffer[i] <= 90) {
			buffer[i] += 32;
		}
		if (buffer[i] == ',' || buffer[i] == '\"' || buffer[i] == '.' ||
			buffer[i] == '!' || buffer[i] == '?' || buffer[i] == ';' ||
			buffer[i] == ':' || buffer[i] == ' ') {
			buffer[i] = '\n';
		}
		i++;
	}
	out << buffer;
	out.close();
}

void readFile(string filePath, string inFileName, vector<string>& vec) {
	ifstream in(filePath + inFileName);
	if (!in.is_open())
		cout << "fail to open story in readFile" << endl;
	while (!in.eof()) {
		char c[100];
		in.getline(c, 100);
		if (c[0] != '\n' && c[0] != ' ' && c[0] != '\0') {
			string str(c);
			vec.push_back(str);
		}
	}
	in.close();
}

//brute force
unsigned int pureLevenshteinDistance(const std::string& s1, const std::string& s2) {
	const std::size_t len1 = s1.size(), len2 = s2.size();
	std::vector<unsigned int> col(len2 + 1), prevCol(len2 + 1);
	for (unsigned int i = 0; i < prevCol.size(); i++)
		prevCol[i] = i;
	for (unsigned int i = 0; i < len1; i++) {
		col[0] = i + 1;
		for (unsigned int j = 0; j < len2; j++) {
			col[j + 1] = std::min({ prevCol[1 + j] + 1, col[j] + 1, prevCol[j] + (s1[i] == s2[j] ? 0 : 1) });
		}
		col.swap(prevCol);
	}
	return prevCol[len2];
}

//levenshtein distance with flag array
unsigned int flagLevenshteinDistance(const std::string& s1, const std::string& s2) {
	const std::size_t len1 = s1.size(), len2 = s2.size();
	//dynamic array to record the state of input and dict for pruning


	bool** flagArray;
	flagArray = new bool*[len2 + 1];
	for (int i = 0; i < len2 + 1; i++) {
		flagArray[i] = new bool[len1 + 1];
		memset(flagArray[i], true, (len1 + 1) * sizeof(bool));
	}


	std::vector<unsigned int> col(len2 + 1), prevCol(len2 + 1);
	for (unsigned int i = 0; i < prevCol.size(); i++)
		prevCol[i] = i;
	for (unsigned int i = 0; i < len1; i++) {
		col[0] = i + 1;
		for (unsigned int j = 0; j < len2; j++) {
			if (flagArray[j][i]) {
				col[j + 1] = std::min({ prevCol[1 + j] + 1, col[j] + 1, prevCol[j] + (s1[i] == s2[j] ? 0 : 1)
				});
				//pruning

				if (col[j + 1] > ABSOLUTE_BEAM) {
					if (j >= i) {
						for (int p = j; p < len2; p++) {
							for (int k = i; k < len1; k++) {
								if ((p - j) >(k - i))
									flagArray[p][k] = false;
							}
						}
					}
					else {
						for (int p = j; p < len2; p++) {
							for (int k = i; k < len1; k++) {
								if ((p - j) < (k - i))
									flagArray[p][k] = false;
							}
						}
					}
				}

			}
		}
		col.swap(prevCol);
	}
	//release dynamic array
	delete[] flagArray;
	return prevCol[len2];
}

//levenshtein distance with beam
unsigned int beamLevenshteinDistance(const std::string& s1, const std::string& s2) {
	const size_t len_input = s1.size(), len_tem = s2.size();
	vector<unsigned int > col(len_tem + 1), prevCol(len_tem + 1);

	unsigned int temp_up = s2.size() - 1;
	unsigned int temp_down = 0;

	// initialize the first column
	for (unsigned int i = 0; i < len_tem; i++)
	{
		prevCol[i] = i;
		if (prevCol[i] > ABSOLUTE_BEAM) {
			temp_up = i;
			break;
		}
	}

	for (unsigned int i = 0; i < len_input; i++)
	{
		if (temp_down == 0) {
			col[0] = i + 1;
			if (col[0] > ABSOLUTE_BEAM) {
				temp_down = 1;
			}
		}
		else
			temp_down += 1;

		temp_up += 1;

		unsigned int temp_value = temp_up >= len_tem ? len_tem : temp_up;

		// the loop should only go to the middle part and forget about the upper and down part
		for (unsigned int j = temp_down; j < temp_value; j++) {
			col[j + 1] = min({ prevCol[1 + j] + 1, col[j] + 1, prevCol[j] + (s1[i] == s2[j] ? 0 : 1) });
			if (col[j + 1] > ABSOLUTE_BEAM) {
				if (i > j) {
					temp_down = j;
				}
				else {
					temp_up = j;
					break;
				}
			}
		}

		col.swap(prevCol);
	}
	return prevCol[len_tem];
}

//levenshtein distance with recursion
int recursionLevenshteinDistance(char *s, int len_s, char *t, int len_t) {
	int cost;

	// base case: empty strings 
	if (len_s == 0) return len_t;
	if (len_t == 0) return len_s;

	// test if last characters of the strings match 
	if (s[len_s - 1] == t[len_t - 1])
		cost = 0;
	else
		cost = 1;

	// return minimum of delete char from s, delete char from t, and delete char from both 
	return min({ recursionLevenshteinDistance(s, len_s - 1, t, len_t) + 1,
		recursionLevenshteinDistance(s, len_s, t, len_t - 1) + 1,
		recursionLevenshteinDistance(s, len_s - 1, t, len_t - 1) + cost });
}

void spellCheck() {
	//convertFile(FILE_PATH, IN_STORY_NAME, OUT_STORY_NAME);
	//convertFile(FILE_PATH, IN_STORY_CORRECT_NAME, OUT_STORY_CORRECT_NAME);
	readFile(FILE_PATH, OUT_STORY_NAME, story);
	readFile(FILE_PATH, OUT_STORY_CORRECT_NAME, storyCorrect);
	readFile(FILE_PATH, IN_DICT_NAME, dict);

	for (int i = 0; i < STORY_SIZE; i++) {
		//initialized to the biggest value of unsigned int 
		distances[i] = UINT_MAX;
		for (int j = 0; j < DICT_SIZE; j++) {
			string s = story.at(i);
			string d = dict.at(j);
			//if (abs((int)(s.length() - d.length())) <= 3) {
			unsigned int dis = pureLevenshteinDistance(s, d);
			if (distances[i] > dis) {
				distances[i] = dis;
				distanceId[i] = j;
			}
			//}
		}
	}
	ofstream storyResult(OUT_STORY_RESULT_NAME);
	ofstream storyResultWithCorrect(OUT_STORY_RESULT_WITH_CORRECT_NAME);
	if (!storyResult.is_open()) {
		cout << "fail to open storyResult" << endl;
		exit(1);
	}
	if (!storyResultWithCorrect.is_open()) {
		cout << "fail to open storyResultWithCorrect" << endl;
		exit(1);
	}
	//correct word after edit distance
	int correctCount = 0;
	for (int i = 0; i < STORY_SIZE; i++) {
		storyResult << story.at(i) << " "
			<< dict.at(distanceId[i]) << " "
			<< distances[i] << "\n";
		//edit distance of alignment word and correct word
		storyResultWithCorrect << dict.at(distanceId[i]) << " "
			<< storyCorrect.at(i) << " "
			<< pureLevenshteinDistance(dict.at(distanceId[i]), storyCorrect.at(i)) << "\n";
		//calculate number of correct word after edit distance
		if (dict.at(distanceId[i]) == storyCorrect.at(i))
			correctCount++;
	}
	cout << "correct rate : " << (double)correctCount / STORY_SIZE << endl;
	storyResult.close();
}