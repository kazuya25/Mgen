#ifndef _INC_IMAGE_H
#define _INC_IMAGE_H

#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <map>

using namespace std;

const string intToColors[] = {"R", "V", "B"};
class Image
{
private:
	unsigned char*	data;
	int	width;
	int	height;
	map<string, double> stats;

public: 
	/** -------------------- CONSTRUCTEURS ----------------------*/
	// Attention on ne peut réassigner 2 fois une meme image : Image img = Image(...) puis img = Image(....) est interdit car ca crée une fuite mémoire !
	// D'ailleurs pour l'instant l'opérateur = n'est pas défini
	Image(string filename);
	Image(int width, int height);
	Image(int width, int height, unsigned char*);
	//cree une image monochromatique
	Image(int width, int height,char R,char V,char B);

	// Attention la signature Image(Image &img); n'est pas un constructeur de copie usuel, il faut avoir un argument de type const TOUJOURS
	// Sinon on ne peut pas utiliser les template comme vector etc...
	Image(const Image &img);

	// Recolle un vecteur de lignes d'images en créant une nouvelle image
	Image(vector<vector<Image> > vect);

	// Ce destructeur est obligatoire puisqu'on alloue la mémoire pour la variable *data
	~Image();


	/** -------------------- OPERATEURS ----------------------*/
	// Crée l'operateur parenthèse
	unsigned char & operator()(int x, int y, int i);
	
	// Crée l'operateur de comparaison 
	bool operator==(const Image & a);

	// Crée l'operateur d'assignation
	void operator=(const Image &a);


	/** -------------------- GETTERS ----------------------*/
	// renvoie la largeur de l'image
	int getWidth() const;

	// renvoie la hauteur de l'image
	int getHeight() const;

	// renvoie les données brutes des l'image
	unsigned char * getData() const;

	// renvoie une statistique de type double (la calcule si elle n'a pas déja été calculée)
	// Moyenne/MoyenneR/MoyenneV/MoyenneB/Variance/VarianceR/VarianceV/VarianceB
	double get(string);

	/** -------------------- Traitement de l'image ----------------------*/
	// Charge une image a partir d'un chemin
	void load(string filename);
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

	// Redimensionne l'image par 2
	Image redim();

	// Transforme l'image en noir et blanc (monochromatiseur)
	Image monochromatise();

	/** -------------------- Calculs ----------------------*/
	// Calcule la moyenne des 3 couleurs par pixels sur toute la grille
	double moyenneCalculateur();

	// Calcule la moyenne des 3 couleurs pour un pixel
	double moyenneCalculateur(int x,int y);

	// Calcule la moyenne RVB (par couleur)
	double moyenneCalculateurRVB(int col);
	
	//Calcul la variance des 3 couleurs par pixels sur toute la grille
	double varianceCalculateur();

	// Calcule la variance RVB (par couleur)
	double varianceCalculateurRVB(int col);
	
	//Calcul covariance des 3 couleurs par pixels sur toute la grille
	double Image::covarianceCalculateur(Image &img);

	//Calcul covariance RVB (par couleur)
	double Image::covarianceCalculateurRVB(Image &img,int col);

	//Calcul la somme des MSE par couleur
	double Image::moyenneDifference(Image &img);

	//Calcul la somme des MSE par couleur et monochromatique
	double Image::superMSE(Image &img, unsigned int);
};

#endif