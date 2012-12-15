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
	// Pour l'instant la variable methode n'est pas utilisee mais on pourra faire évoluer ce code...
	// Ce code est erronné !
	// Il faudrait redimensionner les images ou les rogner pour que cela convienne
	// Car les images ne sont pas de même taille
	Image creerMosaique(string methode, int tailleX, int tailleY);
	// trouve l'image la plus proche de a en terme de moyenne et présente dans la bibliotheque
	Image plusProcheMoyenne(Image a);
	// trouve l'image la plus proche de a en terme de variance et présente dans la bibliotheque
	Image Mosaique::plusProcheVariance(Image a);
};
