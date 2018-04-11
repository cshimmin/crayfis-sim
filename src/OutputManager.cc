#include "OutputManager.hh"
#include "g4root.hh"
#include <sstream>

OutputManager* OutputManager::_inst = 0;

OutputManager::OutputManager() :
	fOutfile(0), fPixels(0),
  fPid(0), fEnergy(0.0), fTheta(0.0), fPhi(0.0),
	fEdep(0), fEdep_front(0), fEdep_nonionizing(0)
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
	fPixels = new TTree("pixels", "pixels");
	fEvent = new TTree("events", "events");

	fCuts = new TH1I("cuts", "cuts", 2, 0, 2);

	fPixels->Branch("pix_x", &pix_x);
	fPixels->Branch("pix_y", &pix_y);
	fPixels->Branch("pix_val", &pix_val);
	fPixels->Branch("pix_n", &pix_n);

  fPixels->Branch("pid", &fPid, "pid/I");
	fPixels->Branch("energy", &fEnergy, "energy/D");
	fPixels->Branch("phi", &fPhi, "phi/D");
	fPixels->Branch("theta", &fTheta, "theta/D");

	fPixels->Branch("Edep", &fEdep, "Edep/D");
	fPixels->Branch("Edep_front", &fEdep_front, "Edep_front/D");
	fPixels->Branch("Edep_nonionizing", &fEdep_nonionizing, "Edep_nonionizing/D");

  fEvent->Branch("pid", &fPid, "pid/I");
  fEvent->Branch("Eincident", &fEnergy, "Eincident/D");
  fEvent->Branch("theta", &fTheta, "theta/D");
  fEvent->Branch("phi", &fPhi, "phi/D");
	fEvent->Branch("Edep", &fEdep, "Edep/D");
	fEvent->Branch("Edep_front", &fEdep_front, "Edep_front/D");
	fEvent->Branch("Edep_nonionizing", &fEdep_nonionizing, "Edep_nonionizing/D");
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

void OutputManager::fillPixels() {
	pix_n = pix_x.size();
	fPixels->Fill();
	fCuts->Fill(1);
}

void OutputManager::fillEvent() {
	fEvent->Fill();
}

void OutputManager::addPixel(int x, int y) {
	pix_x.push_back(x);
	pix_y.push_back(y);
	pix_val.push_back(pix_hits[x][y]);

	G4double val = pix_hits[x][y];
}

void OutputManager::setParticle(G4int pid, G4double energy, G4double theta, G4double phi) {
  fPid = pid;
  fEnergy = energy;
  fTheta = theta;
  fPhi = phi;
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

