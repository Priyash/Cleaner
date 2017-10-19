#include"Cleaner.h"


DataProcessor::DataProcessor()
{
	RootDirCreator(ROOT_DIR_NAME);
}

void DataProcessor::RootDirCreator(string root_dir_name_with_full_path)
{
	string root_cmd = "mkdir ";
	root_cmd.append(DATA_DIR + root_dir_name_with_full_path);
	system(root_cmd.c_str());
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

void DataProcessor::createDir(string fileName)
{
	string extension_dir(DATA_DIR);
	extension_dir.append(fileName);
	//extension_dir.append("_FILE");
	//cout << extension_dir << endl;
	dir_creator(extension_dir);
	extension_dir.clear();

}

void DataProcessor::dir_creator(string file)
{
	string dir_cmd = "mkdir ";
	dir_cmd.append(file);
	system(dir_cmd.c_str());
}

void DataProcessor::fileMove(string src, string dst)
{
	string move_cmd = "move ";
	move_cmd.append(src + " ");
	move_cmd.append(dst);
	system(move_cmd.c_str());
}


void DataProcessor::findFilesAndMoveToDir(string format)
{
	for (auto i : files)
	{
		string file_format = i.substr(i.find_last_of(".") + 1);
		if (file_format == format)
		{
			string src = DATA_DIR + "\"" + i + "\"";
			string dst = DATA_DIR + format;
			fileMove(src, dst);
		}
		else
		{
			continue;
		}
	}
}

void DataProcessor::findDirAndMoveToRootDir()
{
	for (auto i : dirs)
	{
		string src_dir = DATA_DIR;
		src_dir.append("\"" + i + "\"");
		string dst_dir = DATA_DIR + ROOT_DIR_NAME;
		fileMove(src_dir, dst_dir);
	}
}

int DataProcessor::createDirBasedOnExtension()
{
	int status = 0;
	if (!ext_list.empty())
	{
		for (auto i : ext_list)
		{
			cout << i << endl;
			createDir(i);
			dirs.push_back(i);
			findFilesAndMoveToDir(i);
			findDirAndMoveToRootDir();
			
		}
		status = 1;
	}
	else
	{
		status = -1;
	}
	return status;
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

bool DataProcessor::cleanDirectory()
{
	return createDirBasedOnExtension();
}

//======================================MANAGER INTERFACE==================================










DataManager::DataManager()
{
	dprocess = new DataProcessor();
}


DataManager::~DataManager()
{

}

	
void DataManager::findFilesAndDirs()
{
	dprocess->listFilesAndDirs(DATA_DIR);
	dprocess->files_and_dirs_result(files , dirs);
}

void DataManager::clean()
{
	findFilesAndDirs();
	if (dprocess->cleanDirectory())
	{
		cout << "Dir has been created" << endl;

	}
	else
	{
		cout << "Error on creating directory" << endl;
	}
}


void DataManager::printData()
{
	for (auto i : files)
	{
		cout << i.c_str() << endl;
	}
}