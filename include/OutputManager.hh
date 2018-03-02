#include "TFile.h"
#include "TTree.h"
#include "TH1I.h"

#include "globals.hh"

// Maximum number of pixels to allocate memory for.
// Ideally we could just receive this information from
// the config file and dynamically allocate,
// but it's a big pain in the ass to get the
// DetectorConstruction to communicate with this object :(
#define MAX_PIX 5001

class OutputManager
{
public:
	static OutputManager* Instance();

	void initialize();
	void finalize();

	void resetNtuple();
	void fillNtuple();
	void fillEdep();
	void addPixel(int x, int y);

	void setPhi(double newPhi) {phi = newPhi;};
	void setTheta(double newTheta) {theta = newTheta;};
	void setEnergy(double newEnergy) {energy = newEnergy;};
	void setEdep(double newEdep) { Edep = newEdep;};
	double getNGen() { return fCuts->GetBinContent(1); };
	double getNPass() { return fCuts->GetBinContent(2); };

	void writePixels(G4double threshold);
	void clearHits();

	G4double pix_hits[MAX_PIX+1][MAX_PIX+1];
private:
	OutputManager();
	static OutputManager* _inst;

	TFile*                  fOutfile;
	TTree*                  fNtuple;
	TTree*                  fEdep;
	TH1I*			fCuts;

	std::vector<int>        pix_x;
	std::vector<int>        pix_y;
	std::vector<double>     pix_val;
  /*
	std::vector<double>     pix_n1;
	std::vector<double>     pix_n2;
	std::vector<double>     pix_n3;
	std::vector<double>     pix_n4;
	std::vector<double>     pix_n5;
	std::vector<double>     pix_n6;
	std::vector<double>     pix_n7;
	std::vector<double>     pix_n8;
	std::vector<int>    	pix_color;
	std::vector<double>    	pix_r;
	std::vector<double>    	pix_g;
	std::vector<double>    	pix_b;
  */
	int                     pix_n;

	double phi;
	double theta;
	double energy;
	double Edep;
};
