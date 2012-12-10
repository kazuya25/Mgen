#include "Tester.h"
#include <sstream>
/*
	Bibliotheque(void);

	// Enregistre les noms des images dans un dossier NON récursivement !
	// NE CHARGE PAS les images
	Bibliotheque(string path);

	// Enregistre les noms des images dans un dossier NON récursivement !
	Bibliotheque(string path, bool toLoad);

	~Bibliotheque(void);


	void saveImagesInList(string path);
	void loadImages(vector<Image>);
*/

void testBibiliotheque(){
	Bibliotheque bib1 = Bibliotheque("./test");
	assert(bib1.images.size() == 2);
	assert(bib1.images[0] == "./test/Alep.jpg");
	assert(bib1.images[1] == "./test/Besancon.jpg");
	assert(bib1.loadedImages.size() == 0);
	assert(bib1.folders.size() == 1);
	assert(bib1.folders[0] == "./test");

	Bibliotheque bib2 = Bibliotheque("./test", true);
	assert(bib2.loadedImages.size() == 2);

	Bibliotheque bib3 = Bibliotheque();
	bib3.folders.push_back("./test");
	assert(bib3.folders.size() == 1);

	bib3.saveImagesInList();
	assert(bib3.loadedImages.size() == 0);
	assert(bib3.images.size() == 2);

	bib3.loadImages();
	assert(bib3.images.size() == 2);
	assert(bib3.loadedImages.size() == 2);

	cout << "Tests de la classe Bibliotheque reussis" << endl;

}
void testImage()
{
	//assert (1 == 0);

	// Test des constructeurs
	Image img1 = Image(7, 9);
	assert(img1.getWidth() == 7);
	assert(img1.getHeight() == 9);

	Image img2 = Image("test/Alep.jpg");
	assert(img2.getWidth() == 588);
	assert(img2.getHeight() == 294);

	Image img3 = Image(img2);
	assert(img3.getWidth() == 588);
	assert(img3.getHeight() == 294);

	// Test du retournement
	img3.flipHorizontally();
	assert(img3.getWidth() == 588);
	assert(img3.getHeight() == 294);

	assert(img3(0,0,0) == img2(0,293,0));
	assert(img3(0,2,1) == img2(0,291,1));
	assert(img3(0,4,2) == img2(0,289,2));

	// Test du découpage
	vector<vector<Image> > vect = img2.decoupe(50,30);
	for (int i = 0; i < (int)vect[0].size(); i++){
		std::ostringstream oss;
		oss << i;
		string str = oss.str();
		//Renvoie la ligne 1 de la photo Alep.jpg
		vect[0].at(i).save("./testDecoupe/elA" + str + ".jpg");
		//Renvoie la ligne 2 de la photo Alep.jpg
		vect[1].at(i).save("./testDecoupe/elB" + str + ".jpg");
	}

	// Test du recollage
	Image img4 = Image(vect);
	img4.save("./testRecolle/Alep.jpg");

	img4 == img2;
	assert(img4 == img2);
	cout << "Tests de la classe Image reussis" << endl;
}

void testImage2(){
	Image img1 = Image("test/Alep.jpg");
	vector<vector<Image> > vect = img1.decoupe(50,30);
	Image img2 = Image(vect);
	assert(img2.getMoyenne() == img1.getMoyenne() );
}