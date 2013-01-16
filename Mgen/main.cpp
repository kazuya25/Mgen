#include <stdio.h>
#include <iostream>

#include "Mosaique.h"
#include "Bibliotheque.h"
#include "Tester.h"

//Menu
void menu(int argc, char *argv[]){
	
	cout <<endl<<endl<< "Bienvenue dans notre mosaiqueur"<<endl;
	
	string bib = "C:\\testMono";
	string photo = "C:\\Besancon.jpg";
	int longueur = 30;
	int largeur = 30;
	string destination = "C:\\test2.jpg";

	string bibChoisie = "KO";
	string photoChoisie = "KO";
	string longueurChoisie = "KO";
	string largeurChoisie = "KO";
	string destinationChoisie = "KO";

	bool finished = false;

	while (!finished){
		cout << "[1]\t" << bibChoisie << " - Choisir le dossier de la librairie d'images"  << endl;
		cout << "[2]\t" << photoChoisie << " - Choisir la photo a mosaiquer" << endl;
		cout << "[3]\t" << longueurChoisie << " - Longueur d'un petit element de la mosaique" << endl;
		cout << "[4]\t" << largeurChoisie << " - Largeur d'un petit element de la mosaique" << endl;
		cout << "[5]\t" << destinationChoisie << " - Ou enregistrer la mosaique" << endl;
		cout << "[6]\t - Lancer la mosaique la mosaique" << endl;
		cout << "[7]\t - Quitter" << endl;
		string choiceStr;
		int choice;
		int choice2;
		getline(cin, choiceStr);
		istringstream ( choiceStr ) >> choice;
		switch(choice){
		case 1:
		{
			cout << "Veuillez taper l'adresse du dossier de la librairie d'images (utiliser glisser deposer)" << endl;
			getline(cin, choiceStr);
			if(folderExists(choiceStr)) {
				cout << "Dossier choisi" << endl;
				bib = choiceStr;
				bibChoisie="OK";
			}else bibChoisie = "KO";
			break;
		}
		case 2:
		{
			cout << "Veuillez taper l'adresse de la photo a mosaiquer (utiliser glisser deposer)" << endl;
			getline(cin, choiceStr);
			if(fileExists(choiceStr)) {
				cout << "Dossier choisi" << endl;
				photo = choiceStr;
				photoChoisie="OK";
			}else photoChoisie = "KO";
			break;
		}
		case 3:
		{
			cout << "Quelle est la longueur d'un petit element de la mosaique" << endl;
			getline(cin, choiceStr);
			istringstream ( choiceStr ) >> choice2;
			if(choice2 > 0) {
				cout << "Longueur choisie" << endl;
				longueurChoisie="OK";
				longueur = choice2;
			}else longueurChoisie = "KO";
			break;
		}
		case 4:
		{
			cout << "Quelle est la largeur d'un petit element de la mosaique" << endl;
			getline(cin, choiceStr);
			istringstream ( choiceStr ) >> choice2;
			if(choice2 > 0) {
				cout << "Largeur choisie" << endl;
				largeurChoisie="OK";
				largeur = choice2;
			}else longueurChoisie = "KO";
			break;
		}
		case 5:
		{
			cout << "Ou souhaitez vous sauvegarder le fichier ?" << endl;
			getline(cin, choiceStr);
			cout << "Dossier de destination choisi" << endl;
			destination = choiceStr;
			destinationChoisie="OK";
			break;
		}
		case 6:
		{
			cout << "Creation de la bibliotheque ?";
			Bibliotheque bib2 = Bibliotheque(bib, false);
			cout << "OK" << endl;

			cout << "Chargement de l'image ?";
			Mosaique mosaique = Mosaique(Image("C:\\P1060196.JPG"), bib2);
			cout << "OK" << endl;

			cout << "Choisir la methode" << endl;
			cout << "[1]\t - Moyenne"  << endl;
			cout << "[2]\t - Moyenne par couleur" << endl;
			cout << "[3]\t - Variance" << endl;
			cout << "[4]\t - Variance par couleur" << endl;
			cout << "[5]\t - Covariance" << endl;
			cout << "[6]\t - Covariance par couleur" << endl;
			cout << "[7]\t - Erreur quadratique pixel/pixel" << endl;
			cout << "[8]\t - Ponderation Intensite/couleur (+efficace)" << endl;
			cout << "[9]\t - Erreur en valeur absolue pixel/pixel" << endl;
			cout << "[0]\t - Revenir au menu precedent" << endl;
			getline(cin, choiceStr);
			
			istringstream ( choiceStr ) >> choice2;
			string what;
			if		(choice2==1)		what = "Moyenne";
			else if (choice2==2)		what = "MoyenneCol";
			else if (choice2==3)		what = "Variance";
			else if (choice2==4)		what = "VarianceCol";
			else if (choice2==5)		what = "Covariance";
			else if (choice2==6)		what = "CovarianceCol";
			else if (choice2==7)		what = "MSE";
			else if (choice2==8)		what = "MoyenneColInt";
			else if (choice2==9)		what = "superMSE";
			else break;
			mosaique.creerMosaique(what, largeur, longueur, false).save(destination);
			break;
		}
		case 7:
		{
			finished = true;
			break;
		}
		}
	}
}



// Point d'entree
int main(int argc, char *argv[])
{
	// Lance une batterie de tests pour l'aide au developpement (merci de toujours garder ces tests)
	// Dans l'ordre de création
	if (argc > 0 && string(argv[1]) == "test") {
		testBibiliotheque();
		testImage();
		testImage2();
		testMosaique();
	}
	cout << argv[1];
	cout << "Tests termines et conformes" << endl;

	menu(argc, argv);

	return 0;
}


