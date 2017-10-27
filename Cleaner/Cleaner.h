#ifndef CLEANER_H
#define CLEANER_H

#include<iostream>
#include<vector>
#include<Windows.h>
#include<string>
#include<algorithm>
#include<assert.h>
#include<cstdlib>
#include<array>
#include<ctime>
#include<ratio>
#include<chrono>


using namespace std;
using namespace chrono;

const string DATA_DIR = "C:\\Users\\PRIYASH_11\\Desktop\\";
const string ROOT_DIR_NAME = "ROOT";
const vector<string>UNSUPPORTED_FORMAT = { "Thumbs.db","desktop.ini" };
const vector<string>UNSUPPORTED_PREFIX = { "~" };

class IDataProcess
{
	
public:
	virtual void listFilesAndDirs(const string& dir_name) = 0;
	virtual void files_result(vector<string>& files) = 0;
	virtual void dirs_result(vector<string>& dirs) = 0;
	virtual void files_and_dirs_result(vector<string>& files, vector<string>& dirs) = 0;
	virtual bool cleanDirectory() = 0;
};


class DataProcessor : public IDataProcess
{
	HANDLE handler;
	WIN32_FIND_DATA fdata;
	vector<string>files;
	vector<string>dirs;
	vector<string>ext_list;
	bool status = false;
private:
	LPCWSTR ToLPCWSTR(string text);
	string ToString(WCHAR* text);
public:
	DataProcessor();
	~DataProcessor();
	void listFilesAndDirs(const string& dir_name);
	void files_and_dirs_result(vector<string>& files, vector<string>& dirs);
	void files_result(vector<string>& files);
	void dirs_result(vector<string>& dirs);
	bool cleanDirectory();
private:
	void createDir(string fileName);
	int createDirBasedOnExtension();
	void dir_creator(string file);
	void findFilesAndMoveToDir(string extenstion);
	void fileMove(string src, string dst);
	void findDirAndMoveToRootDir();
	void RootDirCreator(string root_dir_name_with_full_path);
	bool DataProcessor::DirExist(const string& dir_name, const string& dir_to_search);
	string getCurrentTimeStamp();
	string replaceAll(const string& line, const char& search, const char& replace);
	void renameDir(const string& src, const string& dst);
};


class DataManager
{
	IDataProcess* dprocess;
	vector<string>files;
	vector<string>dirs;
	vector<string>ext_list;
public:
	DataManager();
	~DataManager();
	void clean();
	void printData();

private:
	void findFilesAndDirs();

};


#endif