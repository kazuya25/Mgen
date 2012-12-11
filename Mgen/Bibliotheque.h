#include <iostream>
#include <cstdio>
#include "image.h"
#include <dirent.h>
#include <string>
#include <vector>
#include <algorithm>

class Bibliotheque
{
public:
	// Cr�e une biblioth�que vide
	Bibliotheque(void);

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

	// contient une liste d'images NON CHARGEES
	vector<string> images;
	// contient une liste d'images CHARGEES
	vector<Image> loadedImages;
	// contient une liste de dossiers dont les images ne sont pas forc�ment chargees
	vector<string> folders;
};

// fonction de manipulation d'images dans un dossier, pas besoin de dependance avec la classe bibliotheque
void rotateAllImagesInFolder(string inputfolder, string outputfolder);
// fonction de recuperation des fichiers dans un dossier
vector<string> getFilesInFolders(string folderPath);