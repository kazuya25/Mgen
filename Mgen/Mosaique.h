#include "Bibliotheque.h"
//#include "image.h"

class Mosaique
{
private:
	Image modele;
	Bibliotheque bibliotheque;
public:
	Mosaique(Image & mod, Bibliotheque & bib);
	Image creerMosaique();
};
