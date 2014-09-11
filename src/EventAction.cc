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

#include "OutputManager.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
:G4UserEventAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
 fTotalEnergyDeposit = 0.;
 for (int i = 0; i < MAX_PIX; ++i) {
 for (int j = 0; j < MAX_PIX; ++j) {
    pix_hits[i][j] = 0;
 }
 }
 OutputManager::Instance()->resetNtuple();
 fHasHit = false;
 if (MIN_NPIX>0) {
	 fPixAboveThreshold.clear();
 }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{
  if (fTotalEnergyDeposit <= 0) return;

  if (MIN_NPIX>0 && fPixAboveThreshold.size()<MIN_NPIX) {
	  return;
  }

  OutputManager *outman = OutputManager::Instance();
  for (int i =0; i < MAX_PIX; ++i) {
  for (int j = 0; j < MAX_PIX; ++j) {
    if (pix_hits[i][j] > MIN_PIX_ENERGY) {
	    outman->addPixel(i,j,pix_hits[i][j]);
    }
  }
  }

  outman->fillNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void EventAction::AddPixHit(G4double Edep, int x, int y) {
	pix_hits[x][y] += Edep;
	if (pix_hits[x][y] > MIN_PIX_ENERGY) {
		fHasHit=true;
		if (MIN_NPIX>0 && pix_hits[x][y] > THRESHOLD_ENERGY) {
			fPixAboveThreshold.insert(std::make_pair(x,y));
		}
	}
}
