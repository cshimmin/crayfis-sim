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

	fCuts = new TH1I("cuts", "cuts", 2, 0, 2);

	fNtuple->Branch("pix_x", &pix_x);
	fNtuple->Branch("pix_y", &pix_y);
	fNtuple->Branch("pix_val", &pix_val);
	fNtuple->Branch("pix_avg3", &pix_avg3);
	fNtuple->Branch("pix_n", &pix_n);

	fNtuple->Branch("phi", &phi, "phi/D");
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
	pix_avg3.clear();
	pix_n = 0;

	fCuts->Fill(0);
}

void OutputManager::fillNtuple() {
	pix_n = pix_x.size();
	fNtuple->Fill();
	fCuts->Fill(1);
}

void OutputManager::addPixel(int x, int y, double val, double avg3) {
	pix_x.push_back(x);
	pix_y.push_back(y);
	pix_val.push_back(val);
	pix_avg3.push_back(avg3);
}

