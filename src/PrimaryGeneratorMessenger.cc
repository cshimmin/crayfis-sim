#include "PrimaryGeneratorMessenger.hh"
#include "G4UIdirectory.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"


PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(
                                             PrimaryGeneratorAction* Gun)
:G4UImessenger(),Action(Gun),
 fGunDir(0), 
 fThetaDist(0),
 fEnergyDist(0),
 fEnergyScale(0)
{
  fGunDir = new G4UIdirectory("/testem/gun/");
  fGunDir->SetGuidance("particle gun control");
 
  fThetaDist = new G4UIcmdWithAString("/testem/gun/theta/hist", this);
  fThetaDist->SetGuidance("file containing a histogram with the desired theta distribution.");
  fThetaDist->SetGuidance("The file should contain a TH1F named 'theta' in units of radians.");
  fThetaDist->SetParameterName("file", true);
  fThetaDist->SetDefaultValue("");
  fThetaDist->AvailableForStates(G4State_Idle);

  fEnergyDist = new G4UIcmdWithAString("/testem/gun/energy/hist", this);
  fEnergyDist->SetGuidance("file containing a histogram with the desired energy distribution.");
  fEnergyDist->SetGuidance("The file should contain a TH1F named 'energy' in units of MeV.");
  fEnergyDist->SetParameterName("histo",true);
  fEnergyDist->SetDefaultValue("");
  fEnergyDist->AvailableForStates(G4State_Idle);

  fEnergyScale = new G4UIcmdWithABool("/testem/gun/energy/logScale", this);
  fEnergyScale->SetGuidance("Whether the given energy histogram has x-values in a log10 scale");
  fEnergyScale->SetDefaultValue(false);
  fEnergyScale->AvailableForStates(G4State_Idle);
}


PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete fThetaDist;
  delete fEnergyDist;
  delete fEnergyScale;
  delete fGunDir;
}


void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,
                                               G4String newValue)
{ 
  if (command == fThetaDist) {
    Action->SetThetaHistogramFile(newValue);
  }
  if (command == fEnergyDist) {
    Action->SetEnergyHistogramFile(newValue);
  }
  if (command == fEnergyScale) {
    Action->SetEnergyLogScale(((G4UIcmdWithABool*)command)->GetNewBoolValue(newValue));
  }
}
