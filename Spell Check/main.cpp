#include "spellCheck.h"
#include <Windows.h>
#include <iostream>
using namespace std;


void print(string s1, string s2, ofstream& out) {
	const std::size_t len1 = s1.size(), len2 = s2.size();
	//store pathArray
	int** pathArray;
	//flag point
	int** flagArray;
	pathArray = new int*[len2 + 1];
	flagArray = new int*[len2 + 1];
	for (int i = 0; i < len2 + 1; i++) {
		pathArray[i] = new int[len1 + 1];
		memset(pathArray[i], 0, (len1 + 1) * sizeof(int));
		flagArray[i] = new int[len1 + 1];
		memset(flagArray[i], 0, (len1 + 1) * sizeof(int));
	}

	printPath(s1, s2, pathArray, flagArray);

	HANDLE hOut;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	char *c1 = new char[len1 + 1];
	strcpy(c1, s1.c_str());
	char *c2 = new char[len2 + 1];
	strcpy(c2, s2.c_str());
	if (!out.is_open())
		cout << "fail to open file in printPath" << endl;
	for (int i = 0; i < len2 + 2; i++) {
		for (int j = 0; j < len1 + 2; j++) {
			if (j == 0) {
				if (i < len2) {
					cout << setw(TABLE_GAP) << c2[len2 - 1 - i];
					out << setw(TABLE_GAP) << c2[len2 - 1 - i];
				}
				else if (i == len2) {
					cout << setw(TABLE_GAP) << "$";
					out << setw(TABLE_GAP) << "$";
				}
				else {
					cout << setw(TABLE_GAP) << " ";
					out << setw(TABLE_GAP) << " ";
				}
			}
			else {
				if (i < len2) {
					if (j != 1) {
						if (flagArray[i][j - 1] == 1)
							SetConsoleTextAttribute(hOut, FOREGROUND_RED);
						else
							SetConsoleTextAttribute(hOut, FOREGROUND_RED |
								FOREGROUND_BLUE | FOREGROUND_GREEN);
						cout << setw(TABLE_GAP) << pathArray[i][j - 1];
						out << setw(TABLE_GAP) << pathArray[i][j - 1];
						SetConsoleTextAttribute(hOut, FOREGROUND_RED |
							FOREGROUND_BLUE | FOREGROUND_GREEN);
					}
					else {
						if (flagArray[i][0] == 1)
							SetConsoleTextAttribute(hOut, FOREGROUND_RED);
						else
							SetConsoleTextAttribute(hOut, FOREGROUND_RED |
								FOREGROUND_BLUE | FOREGROUND_GREEN);
						cout << setw(TABLE_GAP) << len2 - i;
						out << setw(TABLE_GAP) << len2 - i;
						SetConsoleTextAttribute(hOut, FOREGROUND_RED |
							FOREGROUND_BLUE | FOREGROUND_GREEN);
					}
				}
				else if (i == len2) {
					if (flagArray[i][j - 1] == 1)
						SetConsoleTextAttribute(hOut, FOREGROUND_RED);
					else
						SetConsoleTextAttribute(hOut, FOREGROUND_RED |
							FOREGROUND_BLUE | FOREGROUND_GREEN);
					cout << setw(TABLE_GAP) << j - 1;
					out << setw(TABLE_GAP) << j - 1;
					SetConsoleTextAttribute(hOut, FOREGROUND_RED |
						FOREGROUND_BLUE | FOREGROUND_GREEN);
				}
				else {
					if (j != 1) {
						cout << setw(TABLE_GAP) << c1[j - 2];
						out << setw(TABLE_GAP) << c1[j - 2];
					}
					else {
						cout << setw(TABLE_GAP) << "$";
						out << setw(TABLE_GAP) << "$";
					}
				}
			}
		}
		cout << endl;
		out << endl;
	}

	delete[] c1;
	delete[] c2;
	delete[] pathArray;
	delete[] flagArray;
}

void printString(const vector<string>& s1, const vector<string>& s2, ofstream& out, int* errorRecord) {
	const std::size_t len1 = s1.size(), len2 = s2.size();
	//store pathArray
	int** pathArray;
	//flag point
	int** flagArray;
	pathArray = new int*[len2 + 1];
	flagArray = new int*[len2 + 1];
	for (int i = 0; i < len2 + 1; i++) {
		pathArray[i] = new int[len1 + 1];
		memset(pathArray[i], 0, (len1 + 1) * sizeof(int));
		flagArray[i] = new int[len1 + 1];
		memset(flagArray[i], 0, (len1 + 1) * sizeof(int));
	}

	printStringPath(s1, s2, pathArray, flagArray, errorRecord);
	HANDLE hOut;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!out.is_open())
		cout << "fail to open file in printStringPath" << endl;
	for (int i = 0; i < len2 + 2; i++) {
		for (int j = 0; j < len1 + 2; j++) {
			if (j == 0) {
				if (i < len2) {
					cout << setw(TABLE_GAP) << s2[len2 - 1 - i];
					out << setw(TABLE_GAP) << s2[len2 - 1 - i];
				}
				else if (i == len2) {
					cout << setw(TABLE_GAP) << "$";
					out << setw(TABLE_GAP) << "$";
				}
				else {
					cout << setw(TABLE_GAP) << " ";
					out << setw(TABLE_GAP) << " ";
				}
			}
			else {
				if (i < len2) {
					if (j != 1) {
						if (flagArray[i][j - 1] == 1)
							SetConsoleTextAttribute(hOut, FOREGROUND_RED);
						else
							SetConsoleTextAttribute(hOut, FOREGROUND_RED |
								FOREGROUND_BLUE | FOREGROUND_GREEN);
						cout << setw(TABLE_GAP) << pathArray[i][j - 1];
						out << setw(TABLE_GAP) << pathArray[i][j - 1];
						SetConsoleTextAttribute(hOut, FOREGROUND_RED |
							FOREGROUND_BLUE | FOREGROUND_GREEN);
					}
					else {
						if (flagArray[i][0] == 1)
							SetConsoleTextAttribute(hOut, FOREGROUND_RED);
						else
							SetConsoleTextAttribute(hOut, FOREGROUND_RED |
								FOREGROUND_BLUE | FOREGROUND_GREEN);
						cout << setw(TABLE_GAP) << len2 - i;
						out << setw(TABLE_GAP) << len2 - i;
						SetConsoleTextAttribute(hOut, FOREGROUND_RED |
							FOREGROUND_BLUE | FOREGROUND_GREEN);
					}
				}
				else if (i == len2) {
					if (flagArray[i][j - 1] == 1)
						SetConsoleTextAttribute(hOut, FOREGROUND_RED);
					else
						SetConsoleTextAttribute(hOut, FOREGROUND_RED |
							FOREGROUND_BLUE | FOREGROUND_GREEN);
					cout << setw(TABLE_GAP) << j - 1;
					out << setw(TABLE_GAP) << j - 1;
					SetConsoleTextAttribute(hOut, FOREGROUND_RED |
						FOREGROUND_BLUE | FOREGROUND_GREEN);
				}
				else {
					if (j != 1) {
						cout << setw(TABLE_GAP) << s1[j - 2];
						out << setw(TABLE_GAP) << s1[j - 2];
					}
					else {
						cout << setw(TABLE_GAP) << "$";
						out << setw(TABLE_GAP) << "$";
					}
				}
			}
		}
		cout << endl;
		out << endl;
	}
	delete[] pathArray;
	delete[] flagArray;
}

int main() {
	//spellCheck();
	vector<string> story;
	vector<string> storyCorrect;
	readFile(FILE_PATH, OUT_STORY_NAME, story);
	readFile(FILE_PATH, OUT_STORY_CORRECT_NAME, storyCorrect);
	ofstream out(PRINT_TABLE_NAME);
	//0: substitution 1: insertion 2: deletion
	int errorRecord[3] = {0, 0, 0};
	printString(story, storyCorrect, out, errorRecord);
	cout << errorRecord[0] << " " << errorRecord[1] << " " << errorRecord[2] << endl;
	//print("noced", "notice", out);
	//cout << pureLevenshteinDistance("noce", "notice") << endl;
	return 0;
}