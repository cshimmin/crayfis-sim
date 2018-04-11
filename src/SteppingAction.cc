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
/// \file electromagnetic/TestEm1/src/SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//
// $Id: SteppingAction.cc 76293 2013-11-08 13:11:23Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"
#include "Run.hh"
#include "EventAction.hh"
#include "HistoManager.hh"

#include "PixelPVPlacement.hh"

#include "G4RunManager.hh"
#include "G4SteppingManager.hh"
#include "G4VProcess.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* event)
:G4UserSteppingAction(), fEventAction(event)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  Run* run = static_cast<Run*>(
             G4RunManager::GetRunManager()->GetNonConstCurrentRun()); 
  //G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
   
  G4double EdepStep = aStep->GetTotalEnergyDeposit();
  G4double EdepStepNI = aStep->GetNonIonizingEnergyDeposit();
  if (EdepStep > 0.) {
    run->AddEdep(EdepStep);
  }

  const G4VProcess* process = aStep->GetPostStepPoint()->GetProcessDefinedStep();
  if (process) run->CountProcesses(process->GetProcessName());

  // step length of primary particle
  G4int ID         = aStep->GetTrack()->GetTrackID();
  G4double steplen = aStep->GetStepLength();
  //if (ID == 1) analysisManager->FillH1(3,steplen);
/*  
  //mass and charge
  //
  G4StepPoint* postPoint = aStep->GetPostStepPoint();
  G4double charge = postPoint->GetCharge();
  G4double mass   = postPoint->GetMass();
  G4cout << "\n   charge= " << charge << "  mass= " << mass << G4endl;
*/      

  // get the track
  G4Track *track = aStep->GetTrack();
  // get the volumne
  G4VPhysicalVolume * vol = track->GetVolume();
  //G4cout << "volumn name is" << vol->GetName() << G4endl;
  if (vol->GetName() == "pixelP") {
    PixelPVPlacement* pix = (PixelPVPlacement*)vol;
    //G4cout << "Hit (" << pix->idx_x << ", " << pix->idx_y << ")  ::  ";
    //G4cout << track->GetPosition() << G4endl;
    fEventAction->AddPixHit(EdepStep, pix->idx_x, pix->idx_y);

    if (EdepStep > 0.) {
      fEventAction->AddEdep(EdepStep);
    }
    if (EdepStepNI > 0.) {
      fEventAction->AddEdepNI(EdepStepNI);
    }
  }
  else if (vol->GetName() == "front_box") {
    fEventAction->AddEdepFront(EdepStep);
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
