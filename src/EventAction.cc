#include "EventAction.hh"

#include "HistoManager.hh"

#include "G4Event.hh"
#include "G4UnitsTable.hh"

#include "EventMessenger.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
:G4UserEventAction(),
 fTotalEnergyDeposit(0),
 fNonionizingEnergyDeposit(0),
 fNhits(0),
 fMinPixOut(0.0),
 fNPixEvent(0)
{
  fEventMessenger = new EventMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{
  delete fEventMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
 OutputManager *outman = OutputManager::Instance();
 fTotalEnergyDeposit = 0.;
 fTotalEnergyDepositFront = 0.;
 fNonionizingEnergyDeposit = 0.;
 outman->clearHits();
 outman->resetNtuple();

 fNhits = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{
  //if (fTotalEnergyDeposit <= 0) return;

  OutputManager *outman = OutputManager::Instance();

  outman->setEdep(fTotalEnergyDeposit);
  outman->setEdep_front(fTotalEnergyDepositFront);
  outman->setEdep_nonionizing(fNonionizingEnergyDeposit);
  outman->fillEvent();

  if (fNPixEvent>0 && fNhits<fNPixEvent) {
	  return;
  }

  outman->writePixels(fMinPixOut);
  outman->fillPixels();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void EventAction::AddPixHit(G4double Edep, int x, int y) {
	OutputManager *outman = OutputManager::Instance();
	outman->pix_hits[x][y] += Edep;
	if (outman->pix_hits[x][y] > fMinPixEvent) {
		fNhits++;
	}
}
