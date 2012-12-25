#include <iostream>
#include <cstdio>
#include "image.h"
#include <dirent.h>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

class Bibliotheque
{
public:
	// Crée une bibliothèque vide
	Bibliotheque(void);

	// Crée une bibliothèque avec les couleurs primaires
	Bibliotheque::Bibliotheque(int tailleX,int tailleY);

	// Enregistre les noms des images dans un dossier NON récursivement !
	// NE CHARGE PAS les images
	Bibliotheque(string path);

	// Enregistre les noms des images dans un dossier NON récursivement !
	Bibliotheque(string path, bool toLoad);

	~Bibliotheque(void);

	// Manipulation d'images
	void saveImagesInList();
	void saveImagesInList(vector<string> folderList);
	void saveImagesInList(string folderPath);
	
	void loadImages(vector<string>);
	void loadImages();
	
	//Redimensionne toutes les images de la bibliotheques
	void redimImageBib(int tailleX,int tailleY);

	// contient une liste d'images NON CHARGEES
	vector<string> images;
	// contient une liste d'images CHARGEES
	vector<Image> loadedImages;
	// contient une liste de dossiers dont les images ne sont pas forcément chargees
	vector<string> folders;
	// contient une liste d'images CHARGEES redimensionnees
	vector<Image> redimImages;
};

// fonction de manipulation d'images dans un dossier, pas besoin de dependance avec la classe bibliotheque
void rotateAllImagesInFolder(string inputfolder, string outputfolder);
// fonction de recuperation des fichiers dans un dossier
vector<string> getFilesInFolders(string folderPath);