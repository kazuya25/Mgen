#ifndef _INC_BIBLIOTHEQUE_CPP
#define _INC_BIBLIOTHEQUE_CPP
#include "Bibliotheque.h"


using namespace std;


Bibliotheque::Bibliotheque(void)
{
}

//Crée une bibliothèque avec les couleurs primaires (additives et soustractives) ainsi que le blanc et charge ces images
//Ces images ont une taille imposée de tailleX * tailleY (arguments)
Bibliotheque::Bibliotheque(int tailleX,int tailleY) {
	redim = false;
	// On enregistre les noms de fichiers au cas ou on souhaiterait enregistrer les fichiers.
	this->images.push_back("rouge.jpg");
	this->loadedImages.push_back(Image(tailleX,tailleY,char(255),char(0),char(0)));
	this->images.push_back("vert.jpg");
	this->loadedImages.push_back(Image(tailleX,tailleY,char(0),char(255),char(0)));
	this->images.push_back("bleu.jpg");
	this->loadedImages.push_back(Image(tailleX,tailleY,char(0),char(0),char(255)));
	this->images.push_back("jaune.jpg");
	this->loadedImages.push_back( Image(tailleX,tailleY,char(255),char(255),char(0)));
	this->images.push_back("magenta.jpg");
	this->loadedImages.push_back(Image(tailleX,tailleY,char(255),char(0),char(255)));
	this->images.push_back("cyan.jpg");
	this->loadedImages.push_back(Image(tailleX,tailleY,char(0),char(255),char(255)));
	this->images.push_back("blanc.jpg");
	this->loadedImages.push_back(Image(tailleX,tailleY,char(255),char(255),char(255)));

}

//Cree une bibliotheque
//Charge seulement le nom des images dans un vecteur
Bibliotheque::Bibliotheque(string path)
{
	redim = false;
	saveImagesInList(path);
	folders.push_back(path);
}

////Cree une bibliotheque
//Charge le nom des images dans un vecteur 
//et selon bool charge aussi des objets images
Bibliotheque::Bibliotheque(string path, bool toLoad)
{
	redim = false;
	saveImagesInList(path);
	folders.push_back(path);
	if (toLoad)
	{
		loadImages(images);
	}
}

//Charge les images contenu dans le vecteur de noms d'image (attribut)
void Bibliotheque::loadImages()
{
	loadImages(images);
}

//Charge les images contenu dans le vecteur de noms d'image (parametre)
void Bibliotheque::loadImages(vector<string> imagesToLoad)
{
	
	for(int i = 0; i < (int) imagesToLoad.size(); i++)
	{
		loadedImages.push_back(imagesToLoad[i]);
	}
}

//Sauvegarde les images provenant de la liste de dossier (attribut)
void Bibliotheque::saveImagesInList()
{
	saveImagesInList(folders);
}

//Sauvegarde les images provenant de la liste de dossier (parametre)
void Bibliotheque::saveImagesInList(vector<string> folderList)
{
	for(int i = 0; i < (int) folderList.size(); i++)
	{
		saveImagesInList(folderList[i]);
	}
}
//Sauvegarde les images provenant d'un dossier (attribut)
void Bibliotheque::saveImagesInList(string folderPath)
{
	vector<string> files = getFilesInFolders(folderPath);
	for (int i = 0; i < files.size(); i++) 
	{
		string end1 = files[i].substr(files[i].length()-4, 4);
		string end2 = files[i].substr(files[i].length()-5, 5);
		transform(end1.begin(), end1.end() , end1.begin(), ::tolower);
		transform(end2.begin(), end2.end() , end2.begin(), ::tolower);
		if( end1 == ".jpg" || end2 == ".jpeg")   
			images.push_back(files[i]);
	}
}

//Destructeur
Bibliotheque::~Bibliotheque(void)
{
}

//Renvoie un vecteur de noms de fichier qui ne contienne pas aExclure
vector<string> getFilesInFolders(string folderPath, string aExclure)
{
	vector<string> files;
	DIR *dp;
	struct dirent *dirp;
	if((dp  = opendir(folderPath.c_str())) == NULL) 
	{
	        throw "This path is not a directory or cannot be opened";
	}
	while ((dirp = readdir(dp)) != NULL) 
	{
		string s(dirp->d_name);
		if( s != "." && s != ".." && s != aExclure)
			files.push_back(folderPath + "/" + s);
	}
	closedir(dp);
	return files;
}
//Renvoie les fichiers contenu dans un dossier (parametre)
vector<string> getFilesInFolders(string folderPath)
{
	return getFilesInFolders(folderPath, "");
}
//Retourne toutes les images contenu dans la bibliotheque
void rotateAllImagesInFolder(string inputfolder, string outputfolder)
{
	DIR *dp;
	struct dirent *dirp;

	if( (dp  = opendir(inputfolder.c_str()) ) == NULL ) 
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

bool folderExists(string dir){
	DIR  *dip;
	if ((dip = opendir(dir.c_str())) == NULL) {
		cout << "Ce dossier '" << dir << " n'est pas accessible avec ce programme (Veuillez verifier l'existence et/ou les droits)" << endl;
		return false;
	}
	return true;
};

bool fileExists(string file){
	DIR  *dip;
	FILE * pFile = fopen (file.c_str(),"r");
	if ( pFile == NULL) {
		cout << "Ce fichier '" << file << " n'est pas accessible avec ce programme (Veuillez verifier l'existence et/ou les droits)" << endl;
		return false;
	} else fclose(pFile);
	return true;
};
//Redimensionne toutes les images de la bibliotheque
//Commence par /2 toutes les dimensions (moyenne des pixels sur des carres 2*2 -> 1*1)
//Puis on centre l'image pour obtenir la bonne taille
void Bibliotheque::redimImageBib(int tailleX, int tailleY, bool useLoadedImages) {
	redim = true;
	cout << "- Redimensionnement des images" << endl;
	vector<Image> newImages;

	int size = useLoadedImages?loadedImages.size():images.size();

	int avancement = 0;
	clock_t debut,fin;
	debut = clock();

	for(int i=0; i<size; i++) {
		Image* redimI;

		int newAvancement = (int)(i * 100 / size);
		if (newAvancement > avancement){
			avancement = newAvancement;
			cout << avancement << "% .. ";
		}

		if (useLoadedImages) redimI = new Image(loadedImages[i]);
		else redimI = new Image(images[i]);

		while(redimI->getWidth()/2 >= tailleX && redimI->getHeight()/2 >= tailleY) {
				Image tempImage = redimI->redim();
				delete redimI;
				redimI = new Image(tempImage);
			}

		int x = (redimI->getWidth() - tailleX)/2 -1 ;
		int y = (redimI->getHeight() - tailleY)/2-1;
		if(x<0) x = 0;
		if(y<0) y = 0;

		newImages.push_back(redimI->subImage(x,y,tailleX,tailleY));
		delete redimI;
	}
	this->redimImages = newImages;
	fin = clock();

	cout << endl;
	cout << " => Stats : " << endl;
	cout << "  -> Nombre de fichiers : " << size << endl;
	cout << "  -> Duree : " << fin - debut << endl;
	cout << "- Redimensionnement des images OK" << endl;
}

#endif