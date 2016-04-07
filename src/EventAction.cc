//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file electromagnetic/TestEm1/src/EventAction.cc
/// \brief Implementation of the EventAction class
//
// $Id: EventAction.cc 76293 2013-11-08 13:11:23Z gcosmo $
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "EventAction.hh"

#include "HistoManager.hh"

#include "G4Event.hh"
#include "G4UnitsTable.hh"

#include "EventMessenger.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
:G4UserEventAction(),
 fMinPixOut(0.0)
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
 OutputManager::Instance()->clearHits();
 OutputManager::Instance()->resetNtuple();

 fHasHit = false;
 if (fNPixEvent>0) {
	 fPixAboveThreshold.clear();
 }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{
  if (fTotalEnergyDeposit <= 0) return;

  OutputManager *outman = OutputManager::Instance();

  outman->fillEdep();

  if (fNPixEvent>0 && fPixAboveThreshold.size()<fNPixEvent) {
	  return;
  }

  outman->setEdep(fTotalEnergyDeposit);
  outman->writePixels(fMinPixOut);
 
  outman->fillNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void EventAction::AddPixHit(G4double Edep, int x, int y) {
	OutputManager *outman = OutputManager::Instance();
	outman->pix_hits[x][y] += Edep;
	if (outman->pix_hits[x][y] > fMinPixOut) {
		fHasHit=true;
		if (fNPixEvent>0 && outman->pix_hits[x][y] > fMinPixEvent) {
			fPixAboveThreshold.insert(std::make_pair(x,y));
		}
	}
}
