#include "Mosaique.h"
#include "Bibliotheque.h"
#include <sstream>

// Pour l'instant la variable methode n'est pas utilisee mais on pourra faire évoluer ce code...
// On redimensionne les images à la taille souhaité
//Puis on cherche celle qui correspondent le mieu à nos sous images
Image Mosaique::creerMosaique(string methode, int tailleX, int tailleY)
{
	this->bibliotheque->redimImageBib(tailleX,tailleY);
	vector<vector<Image> > images = modele.decoupe(tailleX, tailleY);
	vector<vector<Image> > newImages;
	for (int i = 0 ; i < images.size(); i++)
	{
		vector<Image> ligne = images[i];
		vector<Image> newLigne;
		for (int j = 0; j < ligne.size(); j++){
			cout << "Image "<<i<<"  -  "<<j<<endl;
			Image img = Image(plusProcheMoyenne(ligne[j]));
			//img.save("testRecolle/t.jpg");
			newLigne.push_back(img);
		}
		newImages.push_back(newLigne);
	}
	return(Image(newImages));
}

// trouve l'image la plus proche de a en terme de moyenne et présente dans la bibliotheque
Image Mosaique::plusProcheMoyenne(Image a)
{
	double moyenne = a.getMoyenne();
	vector<double> difference;
	double bestCompare = 1e99;
	int bestInd = 0;
	for(int i = 0; i < (bibliotheque->redimImages).size(); i++)
	{
		double toCompare = abs( (bibliotheque->redimImages)[i].getMoyenne() - moyenne );
		if (toCompare < bestCompare) 
		{
			bestInd = i;
			bestCompare = toCompare;
		}
	}
	return (bibliotheque->redimImages)[bestInd];
}

// trouve l'image la plus proche de a en terme de moyenne et présente dans la bibliotheque
Image Mosaique::plusProcheVariance(Image a)
{
	double var = a.varianceCalculateur();
	vector<double> difference;
	double bestCompare = 1e99;
	int bestInd = 0;
	for(int i = 0; i < (bibliotheque->redimImages).size(); i++)
	{
		double toCompare = abs( (bibliotheque->redimImages)[i].varianceCalculateur() - var );
		if (toCompare < bestCompare) 
		{
			bestInd = i;
			bestCompare = toCompare;
		}
	}
	return (bibliotheque->redimImages)[bestInd];
}