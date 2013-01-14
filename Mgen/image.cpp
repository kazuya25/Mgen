#include "image.h"

// Pour faire marcher jpeglib il faut aller dans Proprieties>C/C++>General>Additional Include Directories
#include <jpeglib.h>
#include <cstdlib>
#include <math.h>
using namespace std;

//----------------------- IMAGE

// Crée une image vide de taille fixée
Image::Image(int width, int height)
{
	this->width = width;
	this->height = height;
	data = new unsigned char[3 * width * height];
	for(int i=0; i < 3*width*height; i ++)
		data[i] = 0;
}

//Crée une image monochromatique RVB de taile fixée
Image::Image(int width, int height,char R,char V,char B) {
	this->width = width;
	this->height = height;
	this->data = new unsigned char[3 * width * height];
	for(int i=0; i < 3*width*height; i ++) {
		switch (i%3) {
			case 0 :
				this->data[i] = R;
				break;
			case 1:
				this->data[i] = V;
				break;
			case 2:
				this->data[i] = B;
				break;
		}
	}
}

// Crée une image à partir de données brutes
Image::Image(int newWidth, int newHeight, unsigned char* newData)
{
	this->width = newWidth;
	this->height = newHeight;
	this->data = newData;
}

//Crée une une instance de la classe Image d'une image jpg ou jpeg
Image::Image(string filename)
{
	load(filename);
}

//Destructeur : la destruction doit être obligatoire sous peine de memoryleak/bug de mémoire
Image::~Image()
{
	//for (map<string, Statistique*>::iterator it=stats.begin(); it!=stats.end(); ++it) {}
	stats.clear();
	delete data;
}

// Crée une image (instance de la classe Image) en copiant une autre classe
Image::Image(const Image &img)
{
	this->width = img.width;
	this->height = img.height;
	this->data = new unsigned char[3*width*height];
	for(int i=0; i < 3*width*height; i ++)
		this->data[i] = img.data[i];
}
// Retourne la largeur de l'image
int Image::getWidth() const
{
	return width;
}
// Retourne les données brutes de l'image
unsigned char * Image::getData() const
{
	return data;
}
// Retourne la longueur/hauteur de l'image
int Image::getHeight() const
{
	return height;
}


// On met en argument un vecteurs de lignes
Image::Image(vector<vector<Image> > images)
{
	// On récupère la taille de la ligne
	if (images.size() == 0) throw "Ce vecteur est vide";
	vector<Image> vect = images[0];
	if(vect.size() == 0) throw "La premiere ligne est vide";
	Image dernierElLigne = vect[vect.size()-1];
	Image premierElLigne = vect[0];

	// Necessaire car le dernier element de la ligne n'est pas de la même taille que les autres
	width = (vect.size()-1) * premierElLigne.getWidth() + dernierElLigne.getWidth();

	// On fait de meme pour la taille de la colonne
	vector<Image> vect2 = images[images.size()-1];
	Image dernierElColonne = vect2[0];

	height = (images.size()-1) * premierElLigne.getHeight() + dernierElColonne.getHeight();

	data = new unsigned char[3 * width * height];
	for(int i=0; i < 3*width*height; i ++)
		this->data[i] = 0;

	int y0 = 0;
	for (int i = 0; i < images.size(); i++)
	{
		int x0 = 0;
		vector<Image> ligne = images[i];
		for(int j = 0; j < ligne.size(); j++)
		{
			Image img = ligne[j];
			this->colle(img, x0, y0);
			x0 += img.getWidth();
		}
		y0 += ligne[0].getHeight();
	}
}
//Charge une image dans la structure
void Image::load(string filename)
{
	FILE *infile;
	struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
	JSAMPARRAY buffer;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    if ((infile = fopen(filename.c_str(), "rb")) == NULL) 
	{
        cout << "Can't open " << filename << endl;
        exit(EXIT_FAILURE);
    }

	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);

	width = cinfo.image_width;
	height = cinfo.image_height;

	data = new unsigned char[3*width*height];

	if(data == NULL)
	{
		cout << "Error in Memory allocation. " << endl;
		exit(EXIT_FAILURE);
	}

	jpeg_start_decompress(&cinfo);

	int i = 0;

	buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, width*3, 1);

	while (cinfo.output_scanline < cinfo.output_height) 
	{
		jpeg_read_scanlines(&cinfo, buffer, 1);

		for(int j=0; j < 3*width; j++)
			data[(cinfo.output_scanline-1)*3*width + j] = (*buffer)[j];
	}
	
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	fclose(infile);
}
//Sauvegarde une image dans le fichier donnée
void Image::save(string filename)
{
	FILE *outfile;
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW row_pointer;          

	if((outfile = fopen(filename.c_str(), "wb")) == NULL)
	{
		cout << "Can't open " << filename << " for saving image." << endl;
		exit(EXIT_FAILURE);
    }

 
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outfile);
  
	cinfo.image_width      = width;
	cinfo.image_height     = height;
	cinfo.input_components = 3;
	cinfo.in_color_space   = JCS_RGB;

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality (&cinfo, 96, true);	  /*set the quality [0..100]  */
	jpeg_start_compress(&cinfo, true);

  
	while (cinfo.next_scanline < cinfo.image_height) 
	{
		row_pointer = (JSAMPROW) &data[cinfo.next_scanline*width*3];
		jpeg_write_scanlines(&cinfo, &row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	fclose(outfile);
}
//Permet d'obtenir l'intensite de la couleur i du pixel (x,y)
unsigned char & Image::operator()(int x, int y, int i)
{
	return data[y*3*width + x*3 + i];
}

//Operateur de test d'égalite d'image
bool Image::operator==(const Image & a)
{
	bool verification = false;

	verification = (a.getHeight() == getHeight());
	verification = verification & (a.getWidth() == getWidth());
	int i;
	for (i = 0; i < 3*a.getWidth()*a.getHeight() && verification; i++)
		verification = verification && ((a.getData())[i] == (getData())[i]);
	return verification;
}
//Affectation
void Image::operator=(const Image & a)
{
	this->width = a.getWidth();
	this->height = a.getHeight();
	this->data = new unsigned char[3*width*height];
	*data = *a.getData();
}
void Image::flipHorizontally()
{
	for(int y = 0; y < height / 2; y ++)
		for(int x = 0; x < width; x ++)
			for(int c = 0; c < 3; c++)
			{
				int d = (*this)(x, y, c);
				(*this)(x, y, c) = (*this)(x,height-y-1, c);
				(*this)(x, height-y-1, c) = d;
			}
}
//Renvoie une sous image de taille sizeX,sizeY a partir du pixel (x0,y0) 
Image Image::subImage(int x0, int y0, int sizeX, int sizeY)
{
	int xF = x0 + sizeX;
	int yF = y0 + sizeY;
	unsigned char *newData = new unsigned char[3*sizeX*sizeY];
	for(int y = y0; y < yF; y ++)
		for(int x = x0; x < xF; x ++)
			for(int c = 0; c < 3; c++)
				newData[(y-y0)*3*sizeX + (x-x0)*3 + c] = (*this)(x, y, c);
	
	return(Image(sizeX, sizeY, newData));
}

// Les rectangles qui ne sont pas de bonnes tailles (sur les bords) sont quand même envoyés
// on renvoie des lignes
vector<vector<Image> > Image::decoupe(int deltaX, int deltaY)
{
	vector<vector<Image> > result;
	//if (deltaX > width || deltaY > height) throw "La taille de l'element de la mosaique doit etre inférieure à l'image";
	for (int i = 0; i < height; i += deltaY)
	{
		vector<Image> ligneImages;
		for (int j = 0; j < width; j += deltaX)
		{
			int dX = min(deltaX, width - j);
			int dY = min(deltaY, height - i);
			ligneImages.push_back(subImage(j, i, dX, dY));
		}
		result.push_back(ligneImages);
	}
	return result;
}
// Permet de coller une image dans une autre image à partir du pixel x,y
void Image::colle(Image img, int x, int y)
{
	for (int i = 0; i < img.getHeight(); i++)
		for (int j = 0; j < img.getWidth(); j++)
			for (int c = 0; c < 3; c++)
				(*this)(x + j, y + i, c) = img(j, i, c);
}
// Transforme l'image en noir et blanc (monochromatiseur)
Image Image::monochromatise()
{
	Image img = Image(width, height);
	unsigned char * newData = new unsigned char[3*width*height];
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++){
			int myMean = 0;
			for (int c = 0; c < 3; c++){
				myMean +=(*this)(j, i, c);
			}
			myMean = (int)(myMean/3);
			for (int c = 0; c < 3; c++){
				img(j, i, c) = myMean;
			}
		}
				
	return img;
}


// Rend une statistique de type double
double Image::get(string stat)
{
	if (stats.find(stat) == stats.end()) // la clé dans la map n'existe pas
	{
		if (stat == "Moyenne")
			stats[stat] = moyenneCalculateur();
		else if (stat == "MoyenneR") 
			stats[stat] = moyenneCalculateurRVB(0);
		else if (stat == "MoyenneV") 
			stats[stat] = moyenneCalculateurRVB(1);
		else if (stat == "MoyenneB")
			stats[stat] = moyenneCalculateurRVB(2);
		else if (stat == "Variance") 
			stats[stat] = varianceCalculateur();
		else if (stat == "VarianceR") 
			stats[stat] = varianceCalculateurRVB(0);
		else if (stat == "VarianceV") 
			stats[stat] = varianceCalculateurRVB(1);
		else if (stat == "VarianceB") 
			stats[stat] = varianceCalculateurRVB(2);

		else throw "Cette statistique '" + stat + "' n'existe pas !";
		return stats[stat];
	}
	else return stats[stat];
}

//Calcule la moyenne des couleurs d'un pixel
double Image::moyenneCalculateur(int x,int y) {
	double moy = 0;
	for(int i=0;i<3;i++) {
		moy += (*this)(x,y,i);
	}
	return moy/3;
}

//Calcule la moyenne des couleurs d'une image
double Image::moyenneCalculateur()
{
	int moyenne = 0;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			for (int c = 0; c < 3; c++)
				moyenne += (*this)(j, i, c);
	moyenne /= 3 * height * width; 
	return moyenne;
}

//Calcule la variance de couleur d'une image
double Image::varianceCalculateur()
{
	double moyenne = this->get("Moyenne");
	double var = 0;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			for (int c = 0; c < 3; c++)
				var +=  pow((double)((*this)(j, i, c) - moyenne ), 2);
	var /= 3 * height * width;
	//cout <<"var : "<<var<<endl;
	return var;
}


//Renvoie une image de taille 2* plus petite (en respectant les proportions)
Image Image::redim() {
	Image img = Image(this->width/2,this->height/2);
	
	for(int i=0;i<img.width;i++) {
		for(int j=0;j<img.height;j++) {
			for(int c=0;c<3;c++) {
				int col = 0;
				int x = (i+1)*2-1;
				int y =(j+1)*2-1;
				col += (*this)(x,y,c);
				col += (*this)(x-1,y,c);
				col += (*this)(x,y-1,c);
				col += (*this)(x-1,y-1,c);
				img(i,j,c) = col/4;
			}
		}
	}
	return img;
}

// Calcule la covariance entre 2 images
double Image::covarianceCalculateur(Image &img) {
		if(this->width != img.width || this->height!=img.height) {
			cout << "Attention vous calculez une covariance sur des img de taille différentes"<<endl;
			throw "Attention vous calculez une covariance sur des img de taille différentes";
		}else{
			double cov = 0;
			double moy = get("Moyenne");
			double moy2 = img.get("Moyenne");
			for(int i=0;i<img.width;i++) {
				for(int j=0;j<img.height;j++) 
					for(int c=0;c<3;c++) {
						cov+= ((*this)(i,j,c)-moy)*(img(i,j,c) - moy2);
					}
			}
			cov /= (3*this->width*this->height);
			cov /= sqrt(get("Variance")*img.get("Variance"));
			return cov;
		}
}

// Calcule la variance RVB (par couleur)
double Image::varianceCalculateurRVB(int col) {
	double moyenne = get("Moyenne" + intToColors[col]);
	double var = 0;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			var +=  pow((double)(*this)(j, i, col)-moyenne, 2);
	var /=  height * width;
	//cout <<"var : "<<var<<endl;
	return var;
}

//Calcul covariance RVB (par couleur)
double Image::covarianceCalculateurRVB(Image &img,int col) {
		if(this->width != img.width || this->height!=img.height) {
			cout << "Attention vous calculez une covariance sur des img de taille différentes"<<endl;
			throw "Attention vous calculez une covariance sur des img de taille différentes";
		}else{
			double cov = 0;
			double moy = get("Moyenne" + intToColors[col]);
			double moy2 = img.get("Moyenne" + intToColors[col]);
			for(int i=0;i<img.width;i++) {
				for(int j=0;j<img.height;j++) {
					cov+= ((*this)(i,j,col)-moy)*(img(i,j,col) - moy2);
				}
			}
			cov /= (this->width*this->height);
			cov/= sqrt(this->get("Variance" + intToColors[col])*img.get("Variance" + intToColors[col]));
			return cov;
		}
}
//Calcul la moyenne de l'intensite de la couleur col dans l'image
double Image::moyenneCalculateurRVB(int col) {
	double tmp=0;
	if(col<3) {
		for(int i=0;i<this->width;i++) {
			for(int j=0;j<this->height;j++) {
				tmp += (*this)(i,j,col);
			}
		}
		return tmp/(this->width*this->height);
	}else{
		cout <<"Couleur non existante : " << col << endl;
		throw "Cette couleur n'existe pas [RVB (0,1,2)]";
	}
}
//Renvoie la covariance entre une image et une autre
double Image::moyenneDifference(Image &img) {
	if(this->width != img.width || this->height!=img.height) {
		cout << "Attention vous calculez une covariance sur des img de taille différentes"<<endl;
		throw "Attention vous calculez une covariance sur des img de taille différentes";
	}else{
		double tmp=0;
		for(int i=0;i<this->width;i++) {
			for(int j=0;j<this->height;j++) {
				//tmp += (this->moyenneCalculateur(i,j) - img.moyenneCalculateur(i,j))*(this->moyenneCalculateur(i,j) - img.moyenneCalculateur(i,j))/(255*255);
				for(int c=0;c<3;c++) {
					tmp += pow((double)(*this)(i,j,c)-img(i,j,c), 2);
				}
			}
				
		}
		return sqrt(tmp);//sqrt(256*256*256)
	}
}

double Image::superMSE(Image &img) {
	if(this->width != img.width || this->height!=img.height) {
		cout << "Attention vous calculez une covariance sur des img de taille différentes"<<endl;
		throw "Attention vous calculez une covariance sur des img de taille différentes";
	}else{
		double tmp=0;
		for(int i=0;i<this->width;i++) {
			for(int j=0;j<this->height;j++) {
				//tmp += (this->moyenneCalculateur(i,j) - img.moyenneCalculateur(i,j))*(this->moyenneCalculateur(i,j) - img.moyenneCalculateur(i,j))/(255*255);
				for(int c=0;c<3;c++) {
					tmp += pow((double)(*this)(i,j,c)-img(i,j,c), 2);
					for (int l=-2; l<3; l++)
						for (int k = -2; k <3; k++){
							if(i+k>=0 && i+k<width && j+l>=0 && j+l<height)
								tmp += pow((double)(*this)(i+k,j+l,c)-img(i+k,j+l,c), 2)/pow((double)abs(k)+abs(l)+1, 2);
						}
				}
			}
				
		}
		return sqrt(tmp);//sqrt(256*256*256)
	}
}
/*
//------------------------------- Moyenne
short Moyenne::compare(ComparableComputing & s) {
	Moyenne* m = static_cast<Moyenne*>(&s); 
	if (m->getMoyenne() < moyenne) return -1;
	else if (m->getMoyenne() < moyenne) return 1;
	else return 0;
	return false;
}

void Moyenne::compute(){
	//Calcul la moyenne des couleurs d'une image
	moyenne = 0;
	for (int i = 0; i < image->getHeight(); i++)
		for (int j = 0; j < image->getWidth(); j++)
			for (int c = 0; c < 3; c++)
				moyenne += (*image)(j, i, c);
	moyenne /= 3 * image->getHeight() * image->getWidth();
}
double Moyenne::getMoyenne(){
	if (moyenne == -1) compute();
	return moyenne;
}


Statistique* Image::getStat(string stat){
	if (stats.count(stat)){
		return stats[stat];
	}
	if (stat == "Moyenne") {
		Statistique * statistic = new Moyenne(this);
		stats[stat] = statistic;
		return stats[stat];
	}
	throw "La statistique \"" + stat + "\" n\'existe pas";
}
*/