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
/// \file electromagnetic/TestEm1/src/PrimaryGeneratorMessenger.cc
/// \brief Implementation of the PrimaryGeneratorMessenger class
//
// $Id: PrimaryGeneratorMessenger.cc 67268 2013-02-13 11:38:40Z ihrivnac $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorMessenger.hh"
#include "G4UIdirectory.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(
                                             PrimaryGeneratorAction* Gun)
:G4UImessenger(),Action(Gun),
 fGunDir(0), 
 fRndmCmd(0),
 fHistoCmd(0)
{
  fGunDir = new G4UIdirectory("/testem/gun/");
  fGunDir->SetGuidance("gun control");
 
  fRndmCmd = new G4UIcmdWithADouble("/testem/gun/rndm",this);
  fRndmCmd->SetGuidance("random lateral extension on the beam");
  fRndmCmd->SetGuidance("in fraction of 0.5*sizeYZ");
  fRndmCmd->SetParameterName("rBeam",false);
  fRndmCmd->SetRange("rBeam>=0.&&rBeam<=1.");
  fRndmCmd->AvailableForStates(G4State_Idle);  

  fHistoCmd = new G4UIcmdWithAString("/testem/gun/energyHisto",this);
  fHistoCmd->SetGuidance("file containing a histogram with the desired energy distribution.");
  fHistoCmd->SetGuidance("The file should contain a TH1F named 'particleEnergy'.");
  fHistoCmd->SetParameterName("histo",true);
  fHistoCmd->SetDefaultValue("");
  fHistoCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete fRndmCmd;
  delete fHistoCmd;
  delete fGunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,
                                               G4String newValue)
{ 
  if (command == fRndmCmd)
   {Action->SetRndmBeam(fRndmCmd->GetNewDoubleValue(newValue));}   

  if (command == fHistoCmd) {
    Action->SetEnergyHistogramFile(newValue);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

