#include "Bibliotheque.h"

using namespace std;


Bibliotheque::Bibliotheque(void)
{
}

//On va creer une bibli avec les couleurs primaires et charger les images
//Image de taille X*Y
Bibliotheque::Bibliotheque(int tailleX,int tailleY) {
	redim = false;
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

//Redimensionne toutes les images de la bibliotheques
//Commence par /2 toutes les dimensions (moyenne des pixels sur des carres 2*2 -> 1*1)
//Puis on centre l'image pour obtenir la bonne taille
void Bibliotheque::redimImageBib(int tailleX,int tailleY) {
	redim = true;
	cout << "Redimensionnement des images " <<endl;
	//this->redimImages = (vector<Image>)realloc(sizeof(vector<Image>));
	vector<Image> newLigne;

	for(int i=0;i<this->loadedImages.size();i++) {
		vector<Image> redimI;
		redimI.push_back(loadedImages[i]);
		while(redimI[redimI.size()-1].getWidth()/2 >= tailleX && redimI[redimI.size()-1].getHeight()/2 >= tailleY) {
				redimI.push_back(redimI[redimI.size()-1].redim());
			}

		//	cout <<"while "<<redimI[redimI.size()-1].getWidth()<<" - - "<<redimI[redimI.size()-1].getHeight()<<endl;
		int x = (redimI[redimI.size()-1].getWidth() - tailleX)/2 -1 ;
		int y = (redimI[redimI.size()-1].getHeight() - tailleY)/2-1;
		if(x<0) { x=0;}
		if(y<0){y=0;}
		//cout << x <<"   _ _ _ _  " << y <<endl;
		newLigne.push_back(redimI[redimI.size()-1].subImage(x,y,tailleX,tailleY));
	}
	this->redimImages = newLigne;
}

