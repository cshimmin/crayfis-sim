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
	void fillPixels();
	void fillEvent();
	void addPixel(int x, int y);

	void setParticle(G4int pid, G4double energy, G4double theta, G4double phi);
	void setEdep(double newEdep) { fEdep = newEdep;};
	void setEdep_front(double newEdep) { fEdep_front = newEdep;};
	void setEdep_nonionizing(double newEdep) { fEdep_nonionizing = newEdep; }
	double getNGen() { return fCuts->GetBinContent(1); };
	double getNPass() { return fCuts->GetBinContent(2); };

	void writePixels(G4double threshold);
	void clearHits();

	G4double pix_hits[MAX_PIX+1][MAX_PIX+1];
private:
	OutputManager();
	static OutputManager* _inst;

	TFile*                  fOutfile;
	TTree*                  fPixels;
	TTree*                  fEvent;
	TH1I*			fCuts;

	std::vector<int>        pix_x;
	std::vector<int>        pix_y;
	std::vector<double>     pix_val;
	int                     pix_n;

	int fPid;
	double fEnergy;
	double fTheta;
	double fPhi;
	double fEdep;
	double fEdep_front;
	double fEdep_nonionizing;
};
