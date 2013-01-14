#include "Tester.h"
#include <sstream>

void testBibiliotheque(){
	cout << "Tests de la classe Bibliotheque" << endl;
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

	Bibliotheque bib = Bibliotheque(50,50);
	int tailleX = 8;
	int tailleY = 8;
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
	
	tailleX = 80;
	tailleY = 80;
	Bibliotheque bib2 = Bibliotheque("C:\\tofs2", false);
	Mosaique mosaique2 = Mosaique(Image("C:\\P1060196.JPG"), bib2);
	mosaique2.creerMosaique("SuperMSE", tailleX, tailleY, false).save("otherTests/myTestSuperMSE.jpg");
	mosaique2.creerMosaique("Moyenne", tailleX, tailleY, false).save("otherTests/myTestMoy.jpg");
	mosaique2.creerMosaique("MoyenneCol", tailleX, tailleY, false).save("otherTests/myTestMoyCol.jpg");
	mosaique2.creerMosaique("MoyenneColInt", tailleX, tailleY, false).save("otherTests/myTestColInt.jpg");
	mosaique2.creerMosaique("Covariance", tailleX, tailleY, false).save("otherTests/myTestCov.jpg");
	mosaique2.creerMosaique("MSE", tailleX, tailleY, false).save("otherTests/myTestMSE.jpg");
	/*
	//mosaique.creerMosaique("Covariance", tailleX,tailleY).save("outputImages/altia_mono_cov.jpg");
	//mosaique.creerMosaique("Covariance", tailleX,tailleY).save("outputImages/altia_mono_cov.jpg");
	//mosaique.creerMosaique("Covariance", tailleX,tailleY).save("outputImages/altia_mono_cov.jpg");
	//Mosaique mosaique = Mosaique(Image("C:\\P1060196.JPG"), bib);
	
	//cout << "Tests de la classe 2 "<<endl;

//	bib.redimImageBib(tailleX,tailleY);
	//mosaique.creerMosaique("Covariance", tailleX,tailleY).save("outputImages/altia_mono_cov.jpg");
//	mosaique.creerMosaique("Moyenne", tailleX,tailleY).save("outputImages/altia_moyCol.jpg");
	//mosaique.creerMosaique("Moyenne Old", tailleX,tailleY).save("outputImages/altia_mono_moy.jpg");
//	mosaique.creerMosaique("New", tailleX,tailleY).save("outputImages/altia_byRMSE.jpg");
	
	
	mosaique.setBibliotheque(&bib2);

	//mosaique.creerMosaique("Covariance", tailleX,tailleY).save("outputImages/Alep_Input_cov.jpg");
	mosaique.creerMosaique("Moyenne", tailleX,tailleY).save("outputImages/test.jpg");
	//mosaique.creerMosaique("Moyenne Old", tailleX,tailleY).save("outputImages/Alep_Input_moy.jpg");
	mosaique.creerMosaique("RMSE", tailleX, tailleY).save("outputImages/test2.jpg");
	*/
	cout << "Tests de la classe 2 Mosaique pas encore faits" << endl;
	cout << "Il manque encore une fonction de resize d'image" << endl;
}
