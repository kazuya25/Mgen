#include <stdio.h>
#include "Tester.h"

// Point d'entree
int main(int argc, char *argv[])
{
	// Lance une batterie de tests pour l'aide au developpement (merci de toujours garder ces tests)
	//testImage();
//	testBibiliotheque();
//	testMosaique();
	bool user = true;
	string modele;
	string bib;
	cout << "Bienvenue dans le créateur de mosaique" <<endl;
	cout << "Entrez le chemion vers l'image que vous voulez mosaiquez :" <<endl;
	cin >> modele;
	cout << "Entrez le nom du repertoire que vous souhaitez utilisez comme bibliotheque d'image " <<endl;
	cin >> bib;
	getchar();
	cout << "Nous dimensionnons toujours les images par des carres de 10*10"<<endl;
	Bibliotheque bibli = Bibliotheque(bib, true);
	Mosaique mosaique = Mosaique(Image(modele), bibli);
	
	int choice;
	string sortie;
	while(user) {
		cout << "Choisissez la methode de calibrage : " <<endl;
		cout << " 1 - Moyenne " <<endl;
		cout << " 2 - RMSE " << endl;
		cout << " 3 - Changement Bibliotheque" <<endl;
		cout << " 4 - Changement Modele"<<endl;
		cout << " 5 - Quitter "<<endl;
		cin >> choice;
		getchar();
		switch (choix) {
		case 1 :
			cout << " Entrez le repertoire de sortie de l'image" <<endl;
			cin >> sortie;
			getchar();
			mosaique.creerMosaique("Moyenne", 10,10).save(sortie+"Moyenne.jpg");
			cout << "La mosaique a ete crée" <<endl;
			break;
		case 2 :
			cout << " Entrez le repertoire de sortie de l'image" <<endl;
			cin >> sortie;
			getchar();
			mosaique.creerMosaique("New", 10,10).save(sortie+"RMSE.jpg");
			cout << "La mosaique a ete crée" <<endl;
			break;
		case 5 :
			user = false;
			cout << "Au revoir !" <<endl;
			break;
		default :
			cout <<"Vous avez rentrez n'importe quoi !!! "<< endl;
			break;
		}
	}

	//testImage2();
	cout << "Appuyer sur entree pour quitter" << endl;
	getchar();
	return 0;
}


