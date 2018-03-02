#include "OutputManager.hh"
#include "g4root.hh"
#include <sstream>

OutputManager* OutputManager::_inst = 0;

OutputManager::OutputManager() :
	fOutfile(0), fNtuple(0), phi(0.0), theta(0.0), energy(0.0)
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
	fNtuple->Branch("pix_n", &pix_n);

	fNtuple->Branch("phi", &phi, "phi/D");
	fNtuple->Branch("theta", &theta, "theta/D");
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

