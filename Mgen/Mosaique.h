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
	// Pour l'instant la variable methode n'est pas utilisee mais on pourra faire �voluer ce code...
	// Ce code est erronn� !
	// Il faudrait redimensionner les images ou les rogner pour que cela convienne
	// Car les images ne sont pas de m�me taille
	Image creerMosaique(string methode, int tailleX, int tailleY);
	// trouve l'image la plus proche de a en terme de moyenne et pr�sente dans la bibliotheque
	Image plusProcheMoyenne(Image a);
	// trouve l'image la plus proche de a en terme de variance et pr�sente dans la bibliotheque
	Image Mosaique::plusProcheVariance(Image a);
};
