#include <stdio.h>

#include "Tester.h"

int main(int argc, char *argv[])
{
	testImage();
	testBibiliotheque();
	testImage2();
	//rotateAllImagesInFolder("inputImages/", "outputImages/");
	cout << "Appuyer sur entree pour quitter" << endl;
	getchar();
	return 0;
}
