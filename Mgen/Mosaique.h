#ifndef _INC_MOSAIQUE_H
#define _INC_MOSAIQUE_H

#include <sstream>
#include "Bibliotheque.h"
#include "image.h"

//class Bibliotheque;

class Mosaique
{
private:
	Image modele;
	Bibliotheque *bibliotheque;
public:
	// Prend une image appellee modele et une bibliotheque
	Mosaique(Image & const  mod, Bibliotheque & const  bib) : modele(mod), bibliotheque(&bib){};
	//Accesseur 
	Bibliotheque getBibliotheque();
	Image getModele();
	void setBibliotheque(Bibliotheque *bib);

	//Cree une mosaique du modele a partir des images de la bibliotheque
	Image creerMosaique(string methode, int tailleX, int tailleY, bool useLoadedImages);
	
	// trouve l'image la plus proche de a en terme de moyenne et présente dans la bibliotheque
	Image plusProcheMoyenne(Image a);

	// trouve l'image la plus proche de a en terme de moyenne pour chacune des couleurs et présente dans la bibliotheque
	// On somme l'erreur quadratique des erreurs pour chacunes des couleurs et on compare
	Image plusProcheMoyenneByCouleur(Image a);

	// trouve l'image la plus proche de a en terme de variance et présente dans la bibliotheque
	Image plusProcheVariance(Image a);

	// trouve l'image la plus proche de a en terme de variance par couleur et présente dans la bibliotheque
	// On somme l'erreur quadratique des erreurs pour chacunes des couleurs et on compare
	Image plusProcheVarianceByCouleur(Image a);

	// trouve l'image la plus proche de a en terme de Covariance et présente dans la bibliotheque
	Image plusProcheCovariance(Image a);

	//
	Image plusProcheCouleurIntensite(Image a);

	//Moindre Carre couleur par couleur
	Image MSE(Image a);
	
	//MSE en utilisant la norme 1
	Image superMSE(Image a);
	
	// trouve l'image la plus proche de a en terme de Covariance et présente dans la bibliotheque
	// Mais en minimisant l'ecart de covariance par couleur
	Image plusProcheCovarianceByCouleur(Image a);
	void function(void* arg);
};

#endif