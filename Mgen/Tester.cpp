#include "Tester.h"
#include <sstream>

void testBibiliotheque(){
	Bibliotheque bib1 = Bibliotheque("./test");
	assert(bib1.images.size() == 4);
	assert(bib1.images[0] == "./test/Alep.jpg");
	assert(bib1.images[1] == "./test/Besancon.jpg");
	assert(bib1.loadedImages.size() == 0);
	assert(bib1.folders.size() == 1);
	assert(bib1.folders[0] == "./test");

	Bibliotheque bib2 = Bibliotheque("./test", true);
	assert(bib2.loadedImages.size() == 4);

	Bibliotheque bib3 = Bibliotheque();
	bib3.folders.push_back("./test");
	assert(bib3.folders.size() == 1);

	bib3.saveImagesInList();
	assert(bib3.loadedImages.size() == 0);
	assert(bib3.images.size() == 4);

	bib3.loadImages();
	assert(bib3.images.size() == 4);
	assert(bib3.loadedImages.size() == 4);

	for(int i=0;i<4;i++) {
		std::ostringstream oss;
		oss << i<<".jpg";
		bib3.loadedImages[i].save("testRedim/test_"+oss.str());
	}

	bib3.redimImageBib(180,30);
	assert(bib3.images.size() == 4);
	assert(bib3.loadedImages.size() == 4);
	for(int i=0;i<4;i++) {
		std::ostringstream oss;
		oss << i<<".jpg";
		assert(bib3.redimImages[i].getHeight() == 30);
		assert(bib3.redimImages[i].getWidth() == 180);
		
		bib3.redimImages[i].save("testRedim/test"+oss.str());
	}
	
	//TEst de la bibli monochromatique
	cout <<"Test bib" << endl;
	Bibliotheque bib4 = Bibliotheque(50,80);
	for(int i=0;i<7;i++) {
		bib4.loadedImages[i].save("testMono/"+bib4.images[i]);
	}
	
	Bibliotheque bib5 = Bibliotheque("testMono",true);
	assert(bib5.images.size()==7);

	cout << "Tests de la classe Bibliotheque reussis" << endl;

}
void testImage()
{
	//assert (1 == 0);

	// Test de l'assignation
	Image img0 = Image("test/Alep.jpg");
	Image img0Copy = img0;
	assert( img0 == img0Copy);

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
	assert(1 == (int)img2.covarianceCalculateur(img1));
	
	Image img22 = Image("test/Alep.jpg");
	assert(img22.covarianceCalculateur(img2) == img2.covarianceCalculateur(img22));
	//test image monochromatique
	Image img3 = Image(10,40,1,1,1);
	for(int i=0;i<3*10*40;i++) {
		assert(img3.getData()[i]==1);
	}
	img3.save("testRecolle/mono.jpg");


	cout << "Tests de la classe 2 Image reussis" << endl;
}

void testMosaique(){

	Bibliotheque bib = Bibliotheque(5,20);
	cout << "fin de la lecture des noms d'images" <<endl;
	Mosaique mosaique = Mosaique(Image("inputImages/images (6).jpg"), bib);
	
	
	cout << "Tests de la classe 2 "<<endl;
	int tailleX = 5;
	int tailleY = 7;
//	bib.redimImageBib(tailleX,tailleY);
	//mosaique.creerMosaique("Covariance", tailleX,tailleY).save("outputImages/altia_mono_cov.jpg");
//	mosaique.creerMosaique("Moyenne", tailleX,tailleY).save("outputImages/altia_moyCol.jpg");
	//mosaique.creerMosaique("Moyenne Old", tailleX,tailleY).save("outputImages/altia_mono_moy.jpg");
//	mosaique.creerMosaique("New", tailleX,tailleY).save("outputImages/altia_byRMSE.jpg");
	
	Bibliotheque bib2 = Bibliotheque("inputImages", true);
	mosaique.setBibliotheque(&bib2);

	//mosaique.creerMosaique("Covariance", tailleX,tailleY).save("outputImages/Alep_Input_cov.jpg");
	mosaique.creerMosaique("Moyenne", tailleX,tailleY).save("outputImages/Alep_TETE_moyCol.jpg");
	//mosaique.creerMosaique("Moyenne Old", tailleX,tailleY).save("outputImages/Alep_Input_moy.jpg");
	mosaique.creerMosaique("New", tailleX,tailleY - 2).save("outputImages/TEET2_Input_byRMSE.jpg");
	
	cout << "Tests de la classe 2 Mosaique pas encore faits" << endl;
	cout << "Il manque encore une fonction de resize d'image" << endl;
}
