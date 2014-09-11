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
/// \file electromagnetic/TestEm1/src/RunAction.cc
/// \brief Implementation of the RunAction class
//
// $Id: RunAction.cc 76293 2013-11-08 13:11:23Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "HistoManager.hh"
#include "Run.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4EmCalculator.hh"

#include "OutputManager.hh"

#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* kin)
:G4UserRunAction(),fDetector(det),fPrimary(kin),fRun(0),fHistoManager(0)
{ 
 fHistoManager = new HistoManager();

 /*
 G4AnalysisManager* man = G4AnalysisManager::Instance();
 man->CreateNtuple("B4", "Edep and TrackL");
 man->CreateNtupleDColumn("Edep");
 //man->CreateNtupleDColumn("Eabs");
 //man->CreateNtupleDColumn("Egap");
 man->CreateNtupleIColumn("pix_x");
 man->CreateNtupleIColumn("pix_y");
 man->CreateNtupleDColumn("pix_val");
 man->CreateNtupleIColumn("pix_n");
 man->FinishNtuple();
 */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
 delete fHistoManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* RunAction::GenerateRun()
{ 
  fRun = new Run(fDetector); 
  return fRun;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{    
  // save Rndm status
  ////G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  G4Random::showEngineStatus();
     
  // keep run condition
  if ( fPrimary ) { 
    G4ParticleDefinition* particle 
      = fPrimary->GetParticleGun()->GetParticleDefinition();
    G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
    fRun->SetPrimary(particle, energy);
  }    
      
  //histograms
  //
  /*
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->IsActive() ) {
    analysisManager->OpenFile();
  }   
  */

  OutputManager::Instance()->initialize();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{ 
  // print Run summary
  fRun->PrintSummary();    
  
  //compute and print statistics
  G4int nbofEvents = fRun->GetNumberOfEvent();
  if ( isMaster && nbofEvents ) fRun->ComputeStatistics();
          
  //save histograms      
  /*
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();  
  if ( analysisManager->IsActive() ) {
    analysisManager->Write();
    analysisManager->CloseFile();
  }    
  */

  OutputManager* outman = OutputManager::Instance();
  G4cout << "Keeping " << outman->getNPass() << " / " << outman->getNGen() << G4endl;
  G4cout << "Efficiency: " << (100.*outman->getNPass()/outman->getNGen()) << "%" << G4endl;

  OutputManager::Instance()->finalize();
  
  // show Rndm status
  G4Random::showEngineStatus(); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
