#ifndef CLEANER_H
#define CLEANER_H

#include<iostream>
#include<vector>
#include<Windows.h>
#include<string>
#include<algorithm>

using namespace std;

const string DATA_DIR = "C:\\Users\\PRIYASH_11\\Desktop\\";

class IDataProcess
{
	
public:
	virtual void listFilesAndDirs(const string& dir_name) = 0;
	virtual void files_result(vector<string>& files) = 0;
	virtual void dirs_result(vector<string>& dirs) = 0;
	virtual void files_and_dirs_result(vector<string>& files, vector<string>& dirs) = 0;
	virtual void createDirBasedOnExtension() = 0;
};


class DataProcessor : public IDataProcess
{
	HANDLE handler;
	WIN32_FIND_DATA fdata;
	vector<string>files;
	vector<string>dirs;
	vector<string>ext_list;
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
	void createDirBasedOnExtension();
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
	void findDirs();
	void findFiles();
	void findFilesAndDirs();
	void printData();

};


#endif