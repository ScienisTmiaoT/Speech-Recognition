#ifndef READ_DIR

#include <io.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

void GetAllFiles(string path, vector<string>& files);
void GetAllFormatFiles(string path, vector<string>& files, string format);

#define READ_DIR
#endif


