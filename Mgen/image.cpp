#include "image.h"

// Pour faire marcher jpeglib il faut aller dans Proprieties>C/C++>General>Additional Include Directories
#include <jpeglib.h>
#include <cstdlib>

using namespace std;

// Crée une image vide de taille fixée
Image::Image(int width, int height)
{
	this->moyenne = -1;
	this->width = width;
	this->height = height;
	data = new unsigned char[3 * width * height];
	for(int i=0; i < 3*width*height; i ++)
		data[i] = 0;
}

//Cree une image monochromatique RVB de taile fixée
Image::Image(int width, int height,char R,char V,char B) {
	this->moyenne = -1;
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
	//cout <<"Image creer"<<R<<V<<B<<endl;

}

// Crée une image à partir de données brutes
Image::Image(int newWidth, int newHeight, unsigned char* newData)
{
	this->moyenne = -1;
	this->width = newWidth;
	this->height = newHeight;
	this->data = newData;
}

//Crée une une instance de la classe Image d'une image jpg ou jpeg
Image::Image(string filename)
{
	this->moyenne = -1;
	load(filename);
}

//Destructeur : la destruction doit être obligatoire sous peine de memoryleak/bug de mémoire
Image::~Image()
{
	delete data;
}

// Crée une image (instance de la classe Image) en copiant une autre classe
Image::Image(const Image &img)
{
	this->moyenne = -1;
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
	this->moyenne = -1;
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
				int d = (*this)(x,y, c);
				(*this)(x,y, c) = (*this)(x,height-y-1, c);
				(*this)(x,height-y-1, c) = d;
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
//Permet de coller une image dans une autre image à partir du pixel x,y
void Image::colle(Image img, int x, int y)
{
	for (int i = 0; i < img.getHeight(); i++)
		for (int j = 0; j < img.getWidth(); j++)
			for (int c = 0; c < 3; c++)
				(*this)(x + j, y + i, c) = img(j, i, c);
}
//Calcul la moyenne des couleurs d'une image
double Image::moyenneCalculateur()
{
	moyenne = 0;
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
	moyenne = this->getMoyenne();
	double var = 0;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			for (int c = 0; c < 3; c++)
				var +=  ((*this)(j, i, c)-moyenne )*((*this)(j, i, c)-moyenne );
	var /= 3 * height * width;
	//cout <<"var : "<<var<<endl;
	return var;
}
//Donne la moyenne de couleur d'une image
double Image::getMoyenne()
{
	if (moyenne < 0)
	{
		return moyenneCalculateur();
	}
	else
		return moyenne;
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

//Calcul covariance
double Image::covarianceCalculateur(Image &img) {
		if(this->width != img.width || this->height!=img.height) {
			cout << "Attention vous calculez une covariance sur des img de taille différentes"<<endl;
			return -2;
		}else{
			double cov = 0;
			double moy = this->getMoyenne();
			double moy2 = img.getMoyenne();
			for(int i=0;i<img.width;i++) {
				for(int j=0;j<img.height;j++) {
					double tmp = 0;
					double tmp2 = 0;
					for(int c=0;c<3;c++) {
						cov+= ((*this)(i,j,c)-moy)*(img(i,j,c) - moy2);
					}
				}
			}
			cov /= (3*this->width*this->height);
			cov/= sqrt(this->varianceCalculateur()*img.varianceCalculateur());
			//cout << "cov : "<<cov <<endl;
			return cov;
		}
}

// Calcule la variance RVB (par couleur)
double Image::varianceCalculateurRVB(int col) {
	double moyenne = this->moyenneCalculateurRVB(col);
	double var = 0;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
				var +=  ((*this)(j, i, col)-moyenne )*((*this)(j, i, col)-moyenne );

	var /=  height * width;
	//cout <<"var : "<<var<<endl;
	return var;
}

//Calcul covariance RVB (par couleur)
double Image::covarianceCalculateurRVB(Image &img,int col) {
		if(this->width != img.width || this->height!=img.height) {
			cout << "Attention vous calculez une covariance sur des img de taille différentes"<<endl;
			return -2;
		}else{
			double cov = 0;
			double moy = this->moyenneCalculateurRVB(col);
			double moy2 = img.moyenneCalculateurRVB(col);
			for(int i=0;i<img.width;i++) {
				for(int j=0;j<img.height;j++) {
					cov+= ((*this)(i,j,col)-moy)*(img(i,j,col) - moy2);
				}
			}
			cov /= (this->width*this->height);
			cov/= sqrt(this->varianceCalculateurRVB(col)*img.varianceCalculateurRVB(col));
			//cout << "cov : "<<cov <<endl;
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
		cout << "Cette couleur n'existe pas! [RVB (0,1,2)]"<<endl;
		return -2;
	}
}
//Renvoie la couleur moyenne d'une image - une autre(img)
//Aucune modification sur img
double Image::moyenneDifference(Image &img) {
	if(this->width != img.width || this->height!=img.height) {
		cout << "Attention vous calculez une covariance sur des img de taille différentes"<<endl;
		return -2;
	}else{
		double tmp=0;
		for(int i=0;i<this->width;i++) {
			for(int j=0;j<this->height;j++) {
				for(int c=0;c<3;c++) {
					tmp += (((*this)(i,j,c) - img(i,j,c))/255)*(((*this)(i,j,c) - img(i,j,c))/255);
				}
			}
		}
		return tmp;
	}
}

 