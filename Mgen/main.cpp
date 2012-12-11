#include <stdio.h>
#include "Tester.h"

// Point d'entree
int main(int argc, char *argv[])
{
	// Lance une batterie de tests pour l'aide au developpement (merci de toujours garder ces tests)
	testImage();
	testBibiliotheque();
	testImage2();
	testMosaique();
	cout << "Appuyer sur entree pour quitter" << endl;
	getchar();
	return 0;
}
