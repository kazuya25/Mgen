#include <iostream>
#include <cstdio>
#include "image.h"
#include <dirent.h>
#include <string>
#include <vector>

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