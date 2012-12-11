#ifndef IMAGE
#define IMAGE

#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
using namespace std;

class Image
{
private:
	unsigned char*	data;
	int	width;
	int	height;
	double moyenne;
public: 
	// Attention on ne peut réassigner 2 fois une meme image : Image img = Image(...) puis img = Image(....) est interdit car ca crée une fuite mémoire !
	// D'ailleurs pour l'instant l'opérateur = n'est pas défini
	Image(string filename);
	Image(int width, int height);
	Image(int width, int height, unsigned char*);

	// Attention la signature Image(Image &img); n'est pas un constructeur de copie usuel, il faut avoir un argument de type const TOUJOURS
	// Sinon on ne peut pas utiliser les template comme vector etc...
	Image(const Image &img);

	// Recolle un vecteur de lignes d'images en créant une nouvelle image
	Image(vector<vector<Image> > vect);

	// Ce destructeur est obligatoire puisqu'on alloue la mémoire pour la variable *data
	~Image();

	// Charge une image a partir d'un chemin
	void load(string filename);

	// renvoie la largeur de l'image
	int getWidth() const;

	// renvoie la hauteur de l'image
	int getHeight() const;

	// renvoie les données brutes des l'image
	unsigned char * getData() const;

	// Sauvegarde l'image dans un fichier
	void save(string filename);

	// Fait une symétrie par rapport à l'axe horizontale au centre de la figure
	void flipHorizontally();

	// Decoupe une image en élements de taille deltaX et deltaY
	// Renvoie un vecteur de lignes
	// Chaque element de cette ligne contient un rectangle
	vector<vector<Image> > decoupe(int deltaX, int deltaY);
	
	// Colle une image en coordonnee (x,y)
	void colle(Image img, int x, int y);

	// Extrait une image de taille sizeX x sizeY en partant du point (x,y) (à partir du coin en haut à gauche)
	Image subImage(int x, int y, int sizeX, int sizeY);

	// Calcule la moyenne
	double moyenneCalculateur();
	
	// Rend la moyenne
	// Si elle est déja calculée elle donne la moyenne
	double getMoyenne();

	// Crée l'operateur parenthèse
	unsigned char & operator()(int x, int y, int i);
	
	// Crée l'operateur de comparaison 
	bool operator==(const Image & a);

	// Crée l'operateur d'assignation
	void operator=(const Image &a);
};

#endif
