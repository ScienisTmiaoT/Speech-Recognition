#include "readFile.h"

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
