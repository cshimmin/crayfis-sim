#include "OutputManager.hh"
#include "g4root.hh"
#include <sstream>

OutputManager* OutputManager::_inst = 0;

OutputManager::OutputManager() :
	fOutfile(0), fNtuple(0), phi(0.0)
{
}

OutputManager* OutputManager::Instance() {
	if (_inst == 0) {
		_inst = new OutputManager();
	}

	return _inst;
}

void OutputManager::initialize() {
	std::stringstream ss;
	ss << G4AnalysisManager::Instance()->GetFileName() << ".root";
	fOutfile = new TFile(ss.str().c_str(), "recreate");
	fNtuple = new TTree("pixels", "pixels");
	fEdep = new TTree("fEdep", "fEdep");

	fCuts = new TH1I("cuts", "cuts", 2, 0, 2);

	fNtuple->Branch("pix_x", &pix_x);
	fNtuple->Branch("pix_y", &pix_y);
	fNtuple->Branch("pix_val", &pix_val);
  /*
	fNtuple->Branch("pix_n1", &pix_n1);
	fNtuple->Branch("pix_n2", &pix_n2);
	fNtuple->Branch("pix_n3", &pix_n3);
	fNtuple->Branch("pix_n4", &pix_n4);
	fNtuple->Branch("pix_n5", &pix_n5);
	fNtuple->Branch("pix_n6", &pix_n6);
	fNtuple->Branch("pix_n7", &pix_n7);
	fNtuple->Branch("pix_n8", &pix_n8);
	fNtuple->Branch("pix_color", &pix_color);
	fNtuple->Branch("pix_r", &pix_r);
	fNtuple->Branch("pix_g", &pix_g);
	fNtuple->Branch("pix_b", &pix_b);
  */
	fNtuple->Branch("pix_n", &pix_n);

	fNtuple->Branch("phi", &phi, "phi/D");
	fNtuple->Branch("energy", &energy, "energy/D");
	fNtuple->Branch("Edep", &Edep, "Edep/D");

	fEdep->Branch("Edep", &Edep, "Edep/D");
}

void OutputManager::finalize() {
	if (fOutfile) {
		fOutfile->Write();
		fOutfile->Close();
		fOutfile = 0;
	}
}

void OutputManager::resetNtuple() {
	pix_x.clear();
	pix_y.clear();
	pix_val.clear();
  /*
	pix_n1.clear();
	pix_n2.clear();
	pix_n3.clear();
	pix_n4.clear();
	pix_n5.clear();
	pix_n6.clear();
	pix_n7.clear();
	pix_n8.clear();
	pix_color.clear();
	pix_r.clear();
	pix_g.clear();
	pix_b.clear();
  */
	pix_n = 0;

	fCuts->Fill(0);
}

void OutputManager::fillNtuple() {
	pix_n = pix_x.size();
	fNtuple->Fill();
	fCuts->Fill(1);
}
void OutputManager::fillEdep() {
	fEdep->Fill();
}

void OutputManager::addPixel(int x, int y) {
	pix_x.push_back(x);
	pix_y.push_back(y);
	pix_val.push_back(pix_hits[x][y]);

	G4double val = pix_hits[x][y];
  /*
	double n1 = (x>0&&y>0) ? pix_hits[x-1][y-1] : 0;
	double n2 = (y>0) ? pix_hits[x][y-1] : 0;
	double n3 = (y>0) ? pix_hits[x+1][y-1] : 0;
	double n4 = (x>0) ? pix_hits[x-1][y] : 0;
	double n5 = pix_hits[x+1][y];
	double n6 = (x>0) ? pix_hits[x-1][y+1] : 0;
	double n7 = pix_hits[x][y+1];
	double n8 = pix_hits[x+1][y+1];

	pix_n1.push_back(n1);
	pix_n2.push_back(n2);
	pix_n3.push_back(n3);
	pix_n4.push_back(n4);
	pix_n5.push_back(n5);
	pix_n6.push_back(n6);
	pix_n7.push_back(n7);
	pix_n8.push_back(n8);

	int color;
	int pol;
	if ( (x+y)%2==0 ) {
		color = 1;
		pol = (x%2==0) ? 1 : 0;
	}
	else if ( x%2==0 ) {
		color = 0;
	}
	else {
		color = 2;
	}
	pix_color.push_back(color);

	double r,g,b;
	switch (color) {
		case 0:
			// red pixel
			r = val;
			g = 0.25*(n2+n4+n5+n7);
			b = 0.25*(n1+n3+n6+n8);
			break;
		case 1:
			// green pixel
			g = val;
			if (pol == 1) {
				b = 0.5*(n4+n5);
				r = 0.5*(n2+n7);
			} else {
				r = 0.5*(n4+n5);
				b = 0.5*(n2+n7);
			}
			break;
		case 2:
			// blue pixel
			b = val;
			g = 0.25*(n2+n4+n5+n7);
			b = 0.25*(n1+n3+n6+n8);
			break;
	}
	pix_r.push_back(r);
	pix_g.push_back(g);
	pix_b.push_back(b);
  */
}

void OutputManager::writePixels(G4double threshold) {
	for (int i =0; i < MAX_PIX; ++i) {
		for (int j = 0; j < MAX_PIX; ++j) {
			if (pix_hits[i][j] > threshold) {
			    addPixel(i,j);
			}
		}
	}
}

void OutputManager::clearHits() {
 for (int i = 0; i < MAX_PIX; ++i) {
 for (int j = 0; j < MAX_PIX; ++j) {
    pix_hits[i][j] = 0;
 }
 }
}

