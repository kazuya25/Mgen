#ifndef _INC_MOSAIQUE_CPP
#define _INC_MOSAIQUE_CPP

#include "Mosaique.h"
#include <cmath>
//#include "Bibliotheque.h"

//Accesseur
Bibliotheque Mosaique::getBibliotheque() {
	return *(this->bibliotheque);
}

Image Mosaique::getModele() {
	return this->modele;
}

void Mosaique::setBibliotheque(Bibliotheque *bib) {
	this->bibliotheque = bib;
}



// Les images de la bibliotheque doivent avoir été redimensionne auparavant
//Puis on cherche celle qui correspondent le mieu à nos sous images
Image Mosaique::creerMosaique(string methode, int tailleX, int tailleY, bool useLoadedImages)
{
	cout << "Creation de la mosaique avec la methode "<<methode <<endl;
	if(!this->bibliotheque->redim ||  this->bibliotheque->redimImages[0].getWidth() != tailleX || this->bibliotheque->redimImages[0].getHeight() != tailleY) {
		this->bibliotheque->redimImageBib(tailleX, tailleY, useLoadedImages);
	}

	vector<vector<Image> > images = modele.decoupe(tailleX, tailleY);
	vector<vector<Image> > newImages;
	Image (Mosaique::*fct)(Image) = NULL;
	if      (methode == "Moyenne")			fct = &Mosaique::plusProcheMoyenne;
	else if (methode == "MoyenneCol")		fct = &Mosaique::plusProcheMoyenneByCouleur;
	else if (methode == "Variance")			fct = &Mosaique::plusProcheVariance;
	else if (methode == "VarianceCol")		fct = &Mosaique::plusProcheVarianceByCouleur;
	else if (methode == "Covariance")		fct = &Mosaique::plusProcheCovariance;
	else if (methode == "CovarianceCol")	fct = &Mosaique::plusProcheCovarianceByCouleur;
	else if (methode == "MSE")				fct = &Mosaique::MSE;
	else if (methode == "MoyenneColInt")	fct = &Mosaique::plusProcheCouleurIntensite;
	else if (methode == "SuperMSE")			fct = &Mosaique::superMSE;
	else throw "Cette methode n'existe pas : '" + methode + "' !";

	int avancement = 0;
	for (int i = 0 ; i < images.size(); i++)
	{
		int newAvancement = (int)(i * 100 / images.size());
		if (newAvancement > avancement){
			avancement = newAvancement;
			cout << avancement << "% .. ";
		}

		vector<Image> ligne = images[i];
		vector<Image> newLigne;
		//cout << "Image "<<i<<endl;
		for (int j = 0; j < ligne.size(); j++){
			Image img = Image((this->*fct)(ligne[j]));
			newLigne.push_back(img.subImage(0,0,ligne[j].getWidth(),ligne[j].getHeight()));
		}
		newImages.push_back(newLigne);
	}
	cout << endl;
	return(Image(newImages));
}

// 1-
// trouve l'image la plus proche de a en terme de moyenne et présente dans la bibliotheque
Image Mosaique::plusProcheMoyenne(Image a)
{
	double moyenne = a.get("Moyenne");
	double bestCompare = 10e99;
	int bestInd = 0;
	for(int i = 0; i < (bibliotheque->redimImages).size(); i++)
	{
		double toCompare = fabs( (bibliotheque->redimImages)[i].get("Moyenne") - moyenne );
		if (toCompare < bestCompare) 
		{
			bestInd = i;
			bestCompare = toCompare;
		}
	}
	return (bibliotheque->redimImages)[bestInd];
}
// 2-
// trouve l'image la plus proche de a en terme de moyenne et présente dans la bibliotheque
//Mais en minimisant l'ecart de moyenne par couleur
Image Mosaique::plusProcheMoyenneByCouleur(Image a) {
	double bestCompare = 10e99;
	int bestInd = 0;
	for(int i = 0; i < (bibliotheque->redimImages).size(); i++)
	{
		double toCompare = 0;
		for(int c=0;c<3;c++) {
			string what = "Moyenne"+intToColors[c];
			toCompare += pow(a.get(what) -  (bibliotheque->redimImages)[i].get(what), 2);
		}
		if (toCompare < bestCompare) 
		{
			bestInd = i;
			bestCompare = toCompare;
		}
	}
	return (bibliotheque->redimImages)[bestInd];
}
// 3-
// trouve l'image la plus proche de a en terme de variance et présente dans la bibliotheque
Image Mosaique::plusProcheVariance(Image a)
{
	double var = a.get("Variance");
	double bestCompare = 10e99;
	int bestInd = 0;
	for(int i = 0; i < (bibliotheque->redimImages).size(); i++)
	{
		double toCompare = abs( (bibliotheque->redimImages)[i].get("Variance") - var);
		if (toCompare < bestCompare) 
		{
			bestInd = i;
			bestCompare = toCompare;
		}
	}
	return (bibliotheque->redimImages)[bestInd];
}
// 4-
// trouve l'image la plus proche de a en terme de variance et présente dans la bibliotheque
Image Mosaique::plusProcheVarianceByCouleur(Image a)
{
	double bestCompare = 10e99;
	int bestInd = 0;
	for(int i = 0; i < (bibliotheque->redimImages).size(); i++)
	{
		double toCompare = 0;
		for(int c=0;c<3;c++) {
			string what = "Variance"+intToColors[c];
			toCompare += pow(a.get(what) -  (bibliotheque->redimImages)[i].get(what), 2);
		}
		if (toCompare < bestCompare) 
		{
			bestInd = i;
			bestCompare = toCompare;
		}
	}
	return (bibliotheque->redimImages)[bestInd];
}

// 5-
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

// 6- MSE (mean square error)
Image Mosaique::MSE(Image a) {
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

// 6- SuperMSE (mean square error)
Image Mosaique::superMSE(Image a) {
	double bestCompare = 1e99;
	int bestInd = 0;
	for(int i = 0; i < (bibliotheque->redimImages).size(); i++)
	{
		Image img = (bibliotheque->redimImages)[i].subImage(0,0,a.getWidth(),a.getHeight());
		double toCompare = img.superMSE(a,1);
		if (toCompare < bestCompare) 
		{
			bestInd = i;
			bestCompare = toCompare;
		}
	}
	return (bibliotheque->redimImages)[bestInd];
}

// Ne marche pas puisque cet indicateur rend 0 !
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



Image Mosaique::plusProcheCouleurIntensite(Image a)
{
	
	double moyenne = a.get("Moyenne");
	double bestCompare = 10e99;
	int bestInd = 0;
	for(int i = 0; i < (bibliotheque->redimImages).size(); i++)
	{
		double toCompare = abs( (bibliotheque->redimImages)[i].get("Moyenne") - moyenne );
		double temp = 0;
		for(int c=0;c<3;c++) {
			string what = "Moyenne"+intToColors[c];
			temp += pow(a.get(what) -  (bibliotheque->redimImages)[i].get(what), 2);
		}
		toCompare += sqrt(temp)/3;
		if (toCompare < bestCompare) 
		{
			bestInd = i;
			bestCompare = toCompare;
		}
	}
	return (bibliotheque->redimImages)[bestInd];
}

#endif
