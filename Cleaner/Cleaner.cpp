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
				for (int i = 0; i<UNSUPPORTED_FORMAT.size(); i++)
				{
					if (file_desktop == UNSUPPORTED_FORMAT[i])
					{
						status = true;
					}
					else
					{
						continue;
					}
				}
				files.push_back(file_desktop);
				size_t ext_pos = file_desktop.find_last_of(".");
				string file_ext_str = file_desktop.substr(ext_pos + 1);
				string prefix_str = file_desktop.substr(0,1);
				if (!status && prefix_str!="~")
				{
					ext_list.push_back(file_ext_str);
				}
				status = false;
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
	
	//MoveFile(ToLPCWSTR(src), ToLPCWSTR(dst));
	//RemoveDirectory(ToLPCWSTR(dst));
}

string DataProcessor::replaceAll(const string& line, const char& search, const char& replace)
{
	size_t pos = 0;
	string tmp(line);
	pos = tmp.find_first_of(search);
	while (pos != string::npos)
	{
		if (tmp[pos] == search)
		{
			tmp[pos] = replace;
		}
		pos = tmp.find_first_of(search);
	}

	return tmp;
}

string DataProcessor::getCurrentTimeStamp()
{
	duration<int, std::ratio<60 * 60 * 24> > one_day(1);

	system_clock::time_point today = system_clock::now();
	
	time_t tt;

	tt = system_clock::to_time_t(today);
	string current_timestamp = ctime(&tt);
	current_timestamp.erase(remove_if(current_timestamp.begin(), current_timestamp.end(), [](char c)->bool {return c == '\n'; }));
	current_timestamp = replaceAll(current_timestamp, ' ', '_');
	current_timestamp = replaceAll(current_timestamp, ':', '_');

	return current_timestamp;
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

bool DataProcessor::DirExist(const string& dir_name , const string& dir_to_search)
{
	wstring pattern(dir_name.begin(), dir_name.end());
	pattern.append(L"*.*");
	vector<string>root_dirs;
	bool isDirFound = false;
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
					root_dirs.push_back(ToString(fdata.cFileName));
				}
			}
			else
			{
				//FOR FILE SEARCH USE THIS SECTION

			}

		} while (FindNextFile(handler, &fdata));

	}
	else
	{
		cout << "Error with handler" << endl;
	}


	for (auto i : root_dirs)
	{
		if (dir_to_search == i)
		{
			isDirFound = true;
			break;
		}
		else
		{
			continue;
		}
	}

	return isDirFound;
}

void DataProcessor::renameDir(const string& src, const string& dst)
{
	string rename_cmd = "ren ";
	rename_cmd.append(src);
	rename_cmd.append(" ");
	rename_cmd.append(dst);

	system(rename_cmd.c_str());
}


void DataProcessor::findDirAndMoveToRootDir()
{
	for (auto i : dirs)
	{
		if (i == "ROOT") continue;
		cout << "Moving " + i + " to root directory" << endl;
		string src_dir = DATA_DIR;
		src_dir.append(i);
		string dst_dir = DATA_DIR + ROOT_DIR_NAME;
		string rename_src_dir(src_dir);
		if (DirExist(dst_dir + "\\", i))
		{
			string time_stamp = "_";
			time_stamp.append(getCurrentTimeStamp());
			
			string dst_renamed_dir = i + time_stamp;
			rename_src_dir.append(time_stamp);
			renameDir(src_dir, dst_renamed_dir);

			dst_dir.append("\\" + i + "\\");
			dst_dir.append(dst_renamed_dir.c_str());

			src_dir = DATA_DIR;
			src_dir.append(dst_renamed_dir);
			fileMove(src_dir, dst_dir);
		}
		else
		{
			fileMove(src_dir, dst_dir);
		}
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
			sort(dirs.begin(), dirs.end());
			dirs.erase(unique(dirs.begin(), dirs.end()), dirs.end());
			findFilesAndMoveToDir(i);
			findDirAndMoveToRootDir();
			
		}
		status = 1;
	}
	else if (!dirs.empty())
	{
		findDirAndMoveToRootDir();
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