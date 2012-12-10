#include <iostream>
#include <cstdio>
#include "image.h"
#include <dirent.h>


using namespace std;


void rotateAllImagesInFolder(string inputfolder, string outputfolder)
{
	DIR *dp;
	struct dirent *dirp;

	if((dp  = opendir(inputfolder.c_str())) == NULL) 
	{
	        cout << "Error opening " << inputfolder << endl;
        	return;
	}

	while ((dirp = readdir(dp)) != NULL) 
	{
		string s(dirp->d_name);
		if(s.length() > 4)
		{
			cout << inputfolder + s << "... ";
			cout.flush();
			Image i(inputfolder + s);
			i.flipHorizontally();
			i.save(outputfolder + s);
			cout << "done." << endl;
		}		
	}

	closedir(dp);
}


int main(int argc, char *argv[])
{
	rotateAllImagesInFolder("inputImages/", "outputImages/");
	return 0;
}
