#include "Tester.h"


void testBibiliotheque(){
	cout << "Tests de la classe Bibliotheque" << endl;
	Bibliotheque bib1 = Bibliotheque("./test");
	assert(bib1.images.size() == 4);
	assert(bib1.images[0] == "./test/Alep.jpg");
	assert(bib1.images[1] == "./test/Besancon.jpg");
	assert(bib1.loadedImages.size() == 0);
	assert(bib1.folders.size() == 1);
	assert(bib1.folders[0] == "./test");

	assert(folderExists("./test") == true);

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

	bib3.redimImageBib(180,30, true);
	assert(bib3.images.size() == 4);
	assert(bib3.loadedImages.size() == 4);
	for(int i=0;i<4;i++) {
		std::ostringstream oss;
		oss << i<<".jpg";
		assert(bib3.redimImages[i].getHeight() == 30);
		assert(bib3.redimImages[i].getWidth() == 180);
		
		bib3.redimImages[i].save("testRedim/test"+oss.str());
	}
	
	//Test de la bibli monochromatique
	Bibliotheque bib4 = Bibliotheque(50,80);
	for(int i=0; i<bib4.loadedImages.size() ;i++) {
		(bib4.loadedImages[i]).save("testMono/" + bib4.images[i]);
	}
	
	Bibliotheque bib5 = Bibliotheque("testMono",true);
	assert(bib5.images.size()==7);

	cout << "Tests de la classe Bibliotheque ..... OK" << endl;

}
void testImage()
{
	//assert (1 == 0);
	cout << "Tests de la classe Image -> 1er batch" << endl;

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
	cout << "Tests de la classe Image -> 1er batch ..... OK" << endl;
}

void testImage2(){
	cout << "Tests de la classe Image -> 2eme batch" << endl;
	Image img1 = Image("test/Alep.jpg");
	vector<vector<Image> > vect = img1.decoupe(50,30);
	Image img2 = Image(vect);

	assert(1 == (int)img2.covarianceCalculateur(img1));
	
	Image img22 = Image("test/Alep.jpg");
	assert(img22.covarianceCalculateur(img2) == img2.covarianceCalculateur(img22));
	
	//Test du monochromatiseur
	Image img23 = Image("test/Alep.jpg");
	Image img24 = img23.monochromatise();
	img24.save("otherTests/monoAlep.jpg");
	for(int i=0;i<img24.getWidth() * img24.getHeight() * 3;i+=3) {
		assert(img24.getData()[i] == img24.getData()[i+1]);
		assert(img24.getData()[i+2] == img24.getData()[i+1]);
	}

	//test image monochromatique
	Image img3 = Image(10,40,1,1,1);
	for(int i=0;i<3*10*40;i++) {
		assert(img3.getData()[i]==1);
	}
	img3.save("testRecolle/mono.jpg");


	cout << "Tests de la classe Image -> 2eme batch ..... OK" << endl;
}

void testMosaique(){
	cout <<	"Test de la classe Mosaique" << endl;
	Bibliotheque bib = Bibliotheque(50,50);
	for (int i=0; i < 5; i+=5){
		int tailleX = 10+i;
		int tailleY = 10+i;
		Mosaique mosaique = Mosaique(Image("test/Besancon.jpg"), bib);
		mosaique.creerMosaique("SuperMSE", tailleX, tailleY, true).save("otherTests/BesanconSuperMSE.jpg");
		mosaique.creerMosaique("Moyenne", tailleX, tailleY, true).save("otherTests/BesanconMoy.jpg");
		mosaique.creerMosaique("MoyenneCol", tailleX, tailleY, true).save("otherTests/BesanconMoyCol.jpg");
		mosaique.creerMosaique("Variance", tailleX, tailleY, true).save("otherTests/BesanconVar.jpg");
		mosaique.creerMosaique("VarianceCol", tailleX, tailleY, true).save("otherTests/BesanconVarCol.jpg");
		mosaique.creerMosaique("Covariance", tailleX, tailleY, true).save("otherTests/BesanconCov.jpg");
		mosaique.creerMosaique("CovarianceCol", tailleX, tailleY, true).save("otherTests/BesanconCovCol.jpg");
		mosaique.creerMosaique("MSE", tailleX, tailleY, true).save("otherTests/BesanconMSE.jpg");
		mosaique.creerMosaique("MoyenneColInt", tailleX, tailleY, true).save("otherTests/BesanconMoyColInt.jpg");
	}
	cout <<	"Test de la classe Mosaique ... OK" << endl;
}
