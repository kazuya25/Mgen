#define _INC_BIB
#include <iostream>
#include <cstdio>
#include "image.h"
#include <dirent.h>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <ctime>


class Bibliotheque
{
public:
	// Cr�e une biblioth�que vide
	Bibliotheque(void);

	// Cr�e une biblioth�que avec les couleurs primaires
	Bibliotheque::Bibliotheque(int tailleX,int tailleY);

	// Enregistre les noms des images dans un dossier NON r�cursivement !
	// NE CHARGE PAS les images
	Bibliotheque(string path);

	// Enregistre les noms des images dans un dossier NON r�cursivement !
	Bibliotheque(string path, bool toLoad);

	~Bibliotheque(void);

	// Manipulation d'images
	void saveImagesInList();
	void saveImagesInList(vector<string> folderList);
	void saveImagesInList(string folderPath);
	
	void loadImages(vector<string>);
	void loadImages();
	
	// Redimensionne toutes les images de la bibliotheques
	// La variable useLoadedImages va d�terminer quel ensemble d'images utiliser : pr�charg�es (this->loadedImages) ou non (this->images)
	// Soit les images sont pr�charg�es et on utilise le vecteur d'images "this->loadedImages"
	// Soit les images ne sont pas charg�es, on les charges � la vol�e et on utilise le vecteur de "string this->images"
	void redimImageBib(int tailleX,int tailleY, bool useLoadedImages);

	// contient une liste d'images NON CHARGEES
	vector<string> images;
	// contient une liste d'images CHARGEES
	vector<Image> loadedImages;
	// contient une liste de dossiers dont les images ne sont pas forc�ment chargees
	vector<string> folders;
	// contient une liste d'images CHARGEES redimensionnees
	vector<Image> redimImages;
	//Un booleen pour le redimensionnement des images
	bool redim;

};

// fonction de manipulation d'images dans un dossier, pas besoin de dependance avec la classe bibliotheque
void rotateAllImagesInFolder(string inputfolder, string outputfolder);
// fonction de recuperation des fichiers dans un dossier
vector<string> getFilesInFolders(string folderPath);
