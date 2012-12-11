#include "Mosaique.h"
#include "Bibliotheque.h"

// Pour l'instant la variable methode n'est pas utilisee mais on pourra faire évoluer ce code...
// Ce code est erronné !
// Il faudrait redimensionner les images ou les rogner pour que cela convienne
// Car les images ne sont pas de même taille
Image Mosaique::creerMosaique(string methode, int tailleX, int tailleY)
{
	vector<vector<Image> > images = modele.decoupe(tailleX, tailleY);
	vector<vector<Image> > newImages;
	for (int i = 0 ; i < images.size(); i++)
	{
		vector<Image> ligne = images[i];
		vector<Image> newLigne;
		for (int j = 0; j < ligne.size(); j++)
		{
			Image img = Image(plusProcheMoyenne(ligne[j]));
			//img.decoupe(
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
	for(int i = 0; i < (bibliotheque->loadedImages).size(); i++)
	{
		double toCompare = abs( (bibliotheque->loadedImages)[i].getMoyenne() - moyenne );
		if (toCompare < bestCompare) 
		{
			bestInd = i;
			bestCompare = toCompare;
		}
	}
	return (bibliotheque->loadedImages)[bestInd];
}