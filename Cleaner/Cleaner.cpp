#include"Cleaner.h"


DataProcessor::DataProcessor()
{
	
}

DataProcessor::~DataProcessor()
{
	files.clear();
	dirs.clear();
	handler = NULL;
}


LPCWSTR DataProcessor::ToLPCWSTR(string text)
{
	std::wstring stemp = std::wstring(text.begin(), text.end());
	LPCWSTR sw = (LPCWSTR)stemp.c_str();
	return sw;
}


string DataProcessor::ToString(WCHAR* data)
{
	wstring wstr(data);
	string temp_str(wstr.begin(), wstr.end());
	return temp_str;
}

void DataProcessor::listFilesAndDirs(const string& dir_name)
{
	wstring pattern(dir_name.begin(),dir_name.end());
	pattern.append(L"*.*");
	if ((handler = FindFirstFile(pattern.c_str(), &fdata)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (fdata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				if (wcscmp(fdata.cFileName, L".") != 0 && wcscmp(fdata.cFileName, L"..") != 0)
				{
					//FOLDER
					//cout << fdata->cFileName << endl;
					dirs.push_back(ToString(fdata.cFileName));
				}
			}
			else
			{
				//FILE
				//cout << fdata->cFileName << endl;
				string file_desktop = ToString(fdata.cFileName);
				files.push_back(file_desktop);
				size_t ext_pos = file_desktop.find_last_of(".");
				string file_ext_str = file_desktop.substr(ext_pos + 1);
				ext_list.push_back(file_ext_str);
				file_ext_str.clear();

			}

		} while (FindNextFile(handler,&fdata));
		sort(ext_list.begin(), ext_list.end());
		ext_list.erase(unique(ext_list.begin(), ext_list.end()), ext_list.end());
	}
	else
	{
		cout << "Error with handler" << endl;
	}
}

void DataProcessor::createDirBasedOnExtension()
{
	if (CreateDirectory(ToLPCWSTR(DATA_DIR), NULL))
	{

	}
}


void DataProcessor::files_and_dirs_result(vector<string>& files , vector<string>& dirs)
{
	files = this->files;
	dirs = this->dirs;
}

void DataProcessor::files_result(vector<string>& files)
{
	files = this->files;
}

void DataProcessor::dirs_result(vector<string>& dirs)
{
	dirs = this->dirs;
}










DataManager::DataManager()
{
	dprocess = new DataProcessor();
}


DataManager::~DataManager()
{

}

void DataManager::findFiles()
{
	dprocess->listFilesAndDirs(DATA_DIR);
	dprocess->files_result(files);
}

void DataManager::findDirs()
{
	dprocess->listFilesAndDirs(DATA_DIR);
	dprocess->dirs_result(dirs);
}	

void DataManager::findFilesAndDirs()
{
	dprocess->listFilesAndDirs(DATA_DIR);
	dprocess->files_and_dirs_result(files , dirs);
}


void DataManager::printData()
{
	for (auto i : files)
	{
		cout << i.c_str() << endl;
	}
}