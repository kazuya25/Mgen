#include "image.h";

class Bibliotheque;

class Mosaique
{
private:
	Image modele;
	Bibliotheque *bibliotheque;
public:
	// Prend une image appellee modele et une bibliotheque
	Mosaique(Image & const  mod, Bibliotheque & const  bib) : modele(mod), bibliotheque(&bib){};
	
	//Accesseur 
	Bibliotheque Mosaique::getBibliotheque();
	Image Mosaique::getModele();

	//Cree une mosaique du modele a partir des images de la bibliotheque
	Image creerMosaique(string methode, int tailleX, int tailleY);
	
	// trouve l'image la plus proche de a en terme de moyenne et présente dans la bibliotheque
	Image plusProcheMoyenne(Image a);
	
	// trouve l'image la plus proche de a en terme de Covariance et présente dans la bibliotheque
	Image Mosaique::plusProcheCovariance(Image a);
	
	// trouve l'image la plus proche de a en terme de variance et présente dans la bibliotheque
	Image Mosaique::plusProcheVariance(Image a);
	
	//SOrte de moiindres carre inneficace...
	Image Mosaique::RMSE(Image a);
	
	
	// trouve l'image la plus proche de a en terme de moyenne et présente dans la bibliotheque
	//Mais en minimisant l'ecart de covariance par couleur
	Image Mosaique::plusProcheMoyenneByCouleur(Image a);
	// trouve l'image la plus proche de a en terme de Covariance et présente dans la bibliotheque
	//Mais en minimisant l'ecart de covariance par couleur
	Image Mosaique::plusProcheCovarianceByCouleur(Image a);

};
