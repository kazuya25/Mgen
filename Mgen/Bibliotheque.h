#include <iostream>
#include <cstdio>
#include "image.h"
#include <dirent.h>
#include <string>
#include <vector>

class Bibliotheque
{
public:
	// Crée une bibliothèque vide
	Bibliotheque(void);

	// Enregistre les noms des images dans un dossier NON récursivement !
	// NE CHARGE PAS les images
	Bibliotheque(string path);

	// Enregistre les noms des images dans un dossier NON récursivement !
	Bibliotheque(string path, bool toLoad);

	~Bibliotheque(void);

	void saveImagesInList();
	void saveImagesInList(vector<string> folderList);
	void saveImagesInList(string folderPath);
	void loadImages(vector<string>);
	void loadImages();

	vector<string> images;
	vector<Image> loadedImages;
	vector<string> folders;
};

void rotateAllImagesInFolder(string inputfolder, string outputfolder);
vector<string> getFilesInFolders(string folderPath);