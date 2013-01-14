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
	void Mosaique::setBibliotheque(Bibliotheque *bib);

	//Cree une mosaique du modele a partir des images de la bibliotheque
	Image creerMosaique(string methode, int tailleX, int tailleY, bool useLoadedImages);
	

	// 1- trouve l'image la plus proche de a en terme de moyenne et présente dans la bibliotheque
	Image plusProcheMoyenne(Image a);

	// 2- trouve l'image la plus proche de a en terme de moyenne pour chacune des couleurs et présente dans la bibliotheque
	// On somme l'erreur quadratique des erreurs pour chacunes des couleurs et on compare
	Image Mosaique::plusProcheMoyenneByCouleur(Image a);

	// 3- trouve l'image la plus proche de a en terme de variance et présente dans la bibliotheque
	Image Mosaique::plusProcheVariance(Image a);

	// 4- trouve l'image la plus proche de a en terme de variance par couleur et présente dans la bibliotheque
	// On somme l'erreur quadratique des erreurs pour chacunes des couleurs et on compare
	Image Mosaique::plusProcheVarianceByCouleur(Image a);

	// trouve l'image la plus proche de a en terme de Covariance et présente dans la bibliotheque
	Image Mosaique::plusProcheCovariance(Image a);

	//
	Image Mosaique::plusProcheCouleurIntensite(Image a);

	
	
	//Moindre Carre couleur par couleur
	Image Mosaique::MSE(Image a);
	
	//
	Image Mosaique::superMSE(Image a);
	
	// trouve l'image la plus proche de a en terme de Covariance et présente dans la bibliotheque
	// Mais en minimisant l'ecart de covariance par couleur
	Image Mosaique::plusProcheCovarianceByCouleur(Image a);

};
