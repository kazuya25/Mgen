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

void Mosaique::setBibliotheque(Bibliotheque *bib) {
	this->bibliotheque = bib;
}

// Les images de la bibliotheque doivent avoir été redimensionne auparavant
//Puis on cherche celle qui correspondent le mieu à nos sous images
Image Mosaique::creerMosaique(string methode, int tailleX, int tailleY, bool useLoadedImages)
{
	if(!this->bibliotheque->redim ||  this->bibliotheque->redimImages[0].getWidth() != tailleX || this->bibliotheque->redimImages[0].getHeight() != tailleY) {
		this->bibliotheque->redimImageBib(tailleX, tailleY, useLoadedImages);
	}
	vector<vector<Image> > images = modele.decoupe(tailleX, tailleY);
	vector<vector<Image> > newImages;
	for (int i = 0 ; i < images.size(); i++)
	{
		vector<Image> ligne = images[i];
		vector<Image> newLigne;
		//cout << "Image "<<i<<endl;
		for (int j = 0; j < ligne.size(); j++){
			if(methode == "Moyenne"){
				Image img = Image(plusProcheMoyenne(ligne[j]));
				newLigne.push_back(img.subImage(0,0,ligne[j].getWidth(),ligne[j].getHeight()));
			}else if(methode == "MoyenneCol") {
				Image img = Image(plusProcheMoyenneByCouleur(ligne[j]));
				newLigne.push_back(img.subImage(0,0,ligne[j].getWidth(),ligne[j].getHeight()));
			}else if (methode == "Variance"){
				Image img = Image(plusProcheVariance(ligne[j]));
				newLigne.push_back(img.subImage(0,0,ligne[j].getWidth(),ligne[j].getHeight()));
			}else if (methode == "VarianceCol"){
				Image img = Image(plusProcheVarianceByCouleur(ligne[j]));
				newLigne.push_back(img.subImage(0,0,ligne[j].getWidth(),ligne[j].getHeight()));
			}else if (methode == "Covariance"){
				Image img = Image(plusProcheCovariance(ligne[j]));
				newLigne.push_back(img.subImage(0,0,ligne[j].getWidth(),ligne[j].getHeight()));
			}else if (methode == "CovarianceCol"){
				Image img = Image(plusProcheCovarianceByCouleur(ligne[j]));
				newLigne.push_back(img.subImage(0,0,ligne[j].getWidth(),ligne[j].getHeight()));
			}else if(methode == "MSE"){
				Image img = Image(MSE(ligne[j]));
				newLigne.push_back(img.subImage(0,0,ligne[j].getWidth(),ligne[j].getHeight()));
			}else if(methode == "MoyenneColInt"){
				Image img = Image(plusProcheCouleurIntensite(ligne[j]));
				newLigne.push_back(img.subImage(0,0,ligne[j].getWidth(),ligne[j].getHeight()));
			}else if(methode == "SuperMSE"){
				Image img = Image(superMSE(ligne[j]));
				newLigne.push_back(img.subImage(0,0,ligne[j].getWidth(),ligne[j].getHeight()));
			}else throw "Cette methode n'existe pas : '" + methode + "' !";

			
		}
		newImages.push_back(newLigne);
	}
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
		double toCompare = abs( (bibliotheque->redimImages)[i].get("Moyenne") - moyenne );
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
		double toCompare = img.superMSE(a);
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
