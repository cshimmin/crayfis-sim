#include "TFile.h"
#include "TTree.h"
#include "TH1I.h"

class OutputManager
{
public:
	static OutputManager* Instance();

	void initialize();
	void finalize();

	void resetNtuple();
	void fillNtuple();
	void addPixel(int x, int y, double val, double avg3=0);
	void setPhi(double newPhi) {phi = newPhi;};
	double getNGen() { return fCuts->GetBinContent(1); };
	double getNPass() { return fCuts->GetBinContent(2); };
private:
	OutputManager();
	static OutputManager* _inst;

	TFile*                  fOutfile;
	TTree*                  fNtuple;
	TH1I*			fCuts;

	std::vector<int>        pix_x;
	std::vector<int>        pix_y;
	std::vector<double>     pix_val;
	std::vector<double>     pix_avg3;
	int                     pix_n;

	double phi;
};

