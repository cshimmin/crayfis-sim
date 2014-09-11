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
/// \file electromagnetic/TestEm1/src/DetectorMessenger.cc
/// \brief Implementation of the DetectorMessenger class
//
// $Id: DetectorMessenger.cc 77081 2013-11-21 10:35:09Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction * Det)
:G4UImessenger(),fDetector(Det),
 fTestemDir(0),
 fDetDir(0),
 fMaterCmd(0),
 fSizeCmd(0)
{ 
  fTestemDir = new G4UIdirectory("/testem/");
  fTestemDir->SetGuidance("commands specific to this example");
  
  fDetDir = new G4UIdirectory("/testem/det/");
  fDetDir->SetGuidance("detector construction commands");
        
  fMaterCmd = new G4UIcmdWithAString("/testem/det/setMat",this);
  fMaterCmd->SetGuidance("Select material of the box.");
  fMaterCmd->SetParameterName("choice",false);
  fMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fMaterCmd->SetToBeBroadcasted(false);
  
  fSizeCmd = new G4UIcmdWithADoubleAndUnit("/testem/det/setSize",this);
  fSizeCmd->SetGuidance("Set size of the box");
  fSizeCmd->SetParameterName("Size",false);
  fSizeCmd->SetRange("Size>0.");
  fSizeCmd->SetUnitCategory("Length");
  fSizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSizeCmd->SetToBeBroadcasted(false);

  fDepthCmd = new G4UIcmdWithADoubleAndUnit("/testem/det/setDepth",this);
  fDepthCmd->SetGuidance("Set depth of the sensor");
  fDepthCmd->SetParameterName("Depth",false);
  fDepthCmd->SetRange("Depth>0.");
  fDepthCmd->SetUnitCategory("Length");
  fDepthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fDepthCmd->SetToBeBroadcasted(false);

  fPixWidthCmd = new G4UIcmdWithADoubleAndUnit("/testem/det/setPixWidth",this);
  fPixWidthCmd->SetGuidance("Set width of the pixels.");
  fPixWidthCmd->SetParameterName("PixWidth",false);
  fPixWidthCmd->SetRange("PixWidth>0.");
  fPixWidthCmd->SetUnitCategory("Length");
  fPixWidthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fPixWidthCmd->SetToBeBroadcasted(false);

  fNPixCmd = new G4UIcmdWithAnInteger("/testem/det/setNPix",this);
  fNPixCmd->SetGuidance("Set number of pixels on each axis.");
  fNPixCmd->SetParameterName("NPix",false);
  fNPixCmd->SetRange("NPix>0");
  fNPixCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fNPixCmd->SetToBeBroadcasted(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fMaterCmd;
  delete fSizeCmd; 
  delete fDetDir;
  delete fTestemDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == fMaterCmd )
   { fDetector->SetMaterial(newValue);}
   
  if( command == fSizeCmd )
   { fDetector->SetSize(fSizeCmd->GetNewDoubleValue(newValue));}

  if( command == fDepthCmd )
   { fDetector->SetDepth(fDepthCmd->GetNewDoubleValue(newValue));}

  if( command == fPixWidthCmd )
   { fDetector->SetPixWidth(fPixWidthCmd->GetNewDoubleValue(newValue));}

  if( command == fNPixCmd )
   { fDetector->SetNPix(fNPixCmd->GetNewIntValue(newValue));}
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
