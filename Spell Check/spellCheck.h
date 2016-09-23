#ifndef SPELLCHECK_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

//convert file 
#define BUFFER_SIZE (10000)
#define FILE_PATH ("")
#define IN_STORY_NAME ("story.txt")
#define OUT_STORY_NAME ("pureStory.txt")
#define IN_DICT_NAME ("dict.txt")
#define IN_STORY_CORRECT_NAME ("storycorrect.txt")
#define OUT_STORY_CORRECT_NAME ("pureStoryCorrect.txt")
#define OUT_STORY_RESULT_NAME ("storyResult.txt")
#define OUT_STORY_RESULT_WITH_CORRECT_NAME ("storyResultWithCorrect.txt")

//story and storycorrect word number
#define STORY_SIZE (613)
//dict word number
#define DICT_SIZE (6618)
#define ABSOLUTE_BEAM (3)
#define RELATIVE_BEAM (3)

void convertFile(string filePath, string inFileName, string outFileName);
void readFile(string filePath, string inFileName, vector<string>& vec);
unsigned int pureLevenshteinDistance(const std::string& s1, const std::string& s2);
unsigned int flagLevenshteinDistance(const std::string& s1, const std::string& s2);
unsigned int beamLevenshteinDistance(const std::string& s1, const std::string& s2);
int recursionLevenshteinDistance(char *s, int len_s, char *t, int len_t);
void spellCheck();


#endif SPELLCHECK_H

