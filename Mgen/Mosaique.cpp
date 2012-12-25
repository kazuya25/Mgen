#include "Mosaique.h"
#include "Bibliotheque.h"
#include <sstream>

//Accesseur
Bibliotheque Mosaique::getBibliotheque() {
	return *(this->bibliotheque);
}

Image Mosaique::getModele() {
	return this->modele;
}

// Les images de la bibliotheque doivent avoir été redimensionne auparavant
//Puis on cherche celle qui correspondent le mieu à nos sous images
Image Mosaique::creerMosaique(string methode, int tailleX, int tailleY)
{
	//this->bibliotheque->redimImageBib(tailleX,tailleY);
	vector<vector<Image> > images = modele.decoupe(tailleX, tailleY);
	vector<vector<Image> > newImages;
	for (int i = 0 ; i < images.size(); i++)
	{
		vector<Image> ligne = images[i];
		vector<Image> newLigne;
		cout << "Image "<<i<<endl;
		for (int j = 0; j < ligne.size(); j++){
			//Choix de la methode (Mettre un switch ?? et des cste??)
			if(methode == "Moyenne") {
				Image img = Image(plusProcheMoyenneByCouleur(ligne[j]));
				newLigne.push_back(img.subImage(0,0,ligne[j].getWidth(),ligne[j].getHeight()));
			}else if (methode=="Covariance"){
				Image img = Image(plusProcheCovariance(ligne[j]));
				newLigne.push_back(img.subImage(0,0,ligne[j].getWidth(),ligne[j].getHeight()));
			}else if(methode=="%Moyenne Old"){
				Image img = Image(plusProcheMoyenne(ligne[j]));
				newLigne.push_back(img.subImage(0,0,ligne[j].getWidth(),ligne[j].getHeight()));
			}else{
				Image img = Image(plusProcheCovarianceByCouleur(ligne[j]));
				newLigne.push_back(img.subImage(0,0,ligne[j].getWidth(),ligne[j].getHeight()));
			}
		}
		newImages.push_back(newLigne);
	}
	return(Image(newImages));
}

// trouve l'image la plus proche de a en terme de moyenne et présente dans la bibliotheque
Image Mosaique::plusProcheMoyenne(Image a)
{
	double moyenne = a.getMoyenne();
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
	double bestCompare = 1e99;
	int bestInd = 0;
	for(int i = 0; i < (bibliotheque->redimImages).size(); i++)
	{
		double toCompare = abs( (bibliotheque->redimImages)[i].varianceCalculateur() - var);
		if (toCompare < bestCompare) 
		{
			bestInd = i;
			bestCompare = toCompare;
		}
	}
	return (bibliotheque->redimImages)[bestInd];
}

// trouve l'image la plus proche de a en terme de moyenne et présente dans la bibliotheque
Image Mosaique::plusProcheCovariance(Image a)
{
	double bestCompare = -1;
	int bestInd = 0;
	for(int i = 0; i < (bibliotheque->redimImages).size(); i++)
	{
		Image img = (bibliotheque->redimImages)[i].subImage(0,0,a.getWidth(),a.getHeight());
		double toCompare = img.covarianceCalculateur(a);
		if (toCompare > bestCompare) 
		{
			bestInd = i;
			bestCompare = toCompare;
		}
	}
	return (bibliotheque->redimImages)[bestInd];
}

//A refaire, ne donne pas de bons resultats
Image Mosaique::RMSE(Image a) {
	double bestCompare = 1e99;
	int bestInd = 0;
	for(int i = 0; i < (bibliotheque->redimImages).size(); i++)
	{
		Image img = (bibliotheque->redimImages)[i].subImage(0,0,a.getWidth(),a.getHeight());
		double toCompare = img.moyenneDifference(a);
		if (toCompare < bestCompare) 
		{
			bestInd = i;
			bestCompare = toCompare;
		}
	}
	return (bibliotheque->redimImages)[bestInd];
}
// trouve l'image la plus proche de a en terme de moyenne et présente dans la bibliotheque
//Mais en minimisant l'ecart de moyenne par couleur
Image Mosaique::plusProcheMoyenneByCouleur(Image a) {
	double bestCompare = 1e99;
	int bestInd = 0;
	for(int i = 0; i < (bibliotheque->redimImages).size(); i++)
	{
		double toCompare = 0;
		for(int c=0;c<3;c++) {
			toCompare += (a.moyenneCalculateurRVB(c) -  (bibliotheque->redimImages)[i].moyenneCalculateurRVB(c)) * (a.moyenneCalculateurRVB(c) -  (bibliotheque->redimImages)[i].moyenneCalculateurRVB(c));
		}
		if (toCompare < bestCompare) 
		{
			bestInd = i;
			bestCompare = toCompare;
		}
	}
	return (bibliotheque->redimImages)[bestInd];
}

// trouve l'image la plus proche de a en terme de covariance et présente dans la bibliotheque
//Mais en minimisant l'ecart de covariance par couleur
Image Mosaique::plusProcheCovarianceByCouleur(Image a) {
	double bestCompare = -3;
	int bestInd = 0;
	for(int i = 0; i < (bibliotheque->redimImages).size(); i++)
	{
		Image img = (bibliotheque->redimImages)[i].subImage(0,0,a.getWidth(),a.getHeight());
		double toCompare = 0;
		for(int c=0;c<3;c++) {
			toCompare += a.covarianceCalculateurRVB(img,c);
		}
		if (toCompare > bestCompare) 
		{
			bestInd = i;
			bestCompare = toCompare;
		}
	}
	return (bibliotheque->redimImages)[bestInd];
}
