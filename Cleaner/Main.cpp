#include"Cleaner.h"

#include<iostream>

using namespace std;



int main()
{

	DataManager* manager = new DataManager();
	manager->findFiles();
	//manager->printData();


	return 0;
}