#include "ReadDir.h"

//get all file name
void GetAllFiles(string path, vector<string>& files)
{

	long hFile = 0;
	//file info  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					GetAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}

		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}

}

//get specific format file
void GetAllFormatFiles(string path, vector<string>& files, string format)
{
	//file handler
	long   hFile = 0;
	//file info
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*" + format).c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib&  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
					//add file path
					//GetAllFormatFiles(p.assign(path).append("\\").append(fileinfo.name), files, format);
					//add file name
					GetAllFormatFiles(fileinfo.name, files, format);
				}
			}
			else
			{
				//add file path
				//files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				//add file name
				files.push_back(fileinfo.name);
			}
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
}


