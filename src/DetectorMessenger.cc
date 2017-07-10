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
 fFrontMaterialCmd(0),
 fMaterialCmd(0),
 fFrontDepthCmd(0),
 fDepthCmd(0)
{ 
  fTestemDir = new G4UIdirectory("/testem/");
  fTestemDir->SetGuidance("commands specific to this example");
  
  fDetDir = new G4UIdirectory("/testem/det/");
  fDetDir->SetGuidance("detector construction commands");
        
  fFrontMaterialCmd = new G4UIcmdWithAString("/testem/det/setFrontMat", this);
  fFrontMaterialCmd->SetGuidance("Select material of the front matter.");
  fFrontMaterialCmd->SetParameterName("choice", false);
  fFrontMaterialCmd->AvailableForStates(G4State_PreInit);
  fFrontMaterialCmd->SetToBeBroadcasted(false);

  fMaterialCmd = new G4UIcmdWithAString("/testem/det/setMat",this);
  fMaterialCmd->SetGuidance("Select material of the box.");
  fMaterialCmd->SetParameterName("choice",false);
  fMaterialCmd->AvailableForStates(G4State_PreInit);
  fMaterialCmd->SetToBeBroadcasted(false);

  fFrontDepthCmd = new G4UIcmdWithADoubleAndUnit("/testem/det/setFrontDepth",this);
  fFrontDepthCmd->SetGuidance("Set depth of front matter.");
  fFrontDepthCmd->SetParameterName("Depth",false);
  fFrontDepthCmd->SetRange("Depth>=0.");
  fFrontDepthCmd->SetUnitCategory("Length");
  fFrontDepthCmd->AvailableForStates(G4State_PreInit);
  fFrontDepthCmd->SetToBeBroadcasted(false);
  
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
  delete fFrontMaterialCmd;
  delete fMaterialCmd;
  delete fFrontDepthCmd;
  delete fDepthCmd;
  delete fDetDir;
  delete fTestemDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if (command == fFrontMaterialCmd) {
    fDetector->SetMaterial(newValue, DetectorConstruction::FRONT_LAYER);
  }
  if (command == fMaterialCmd) {
    fDetector->SetMaterial(newValue, DetectorConstruction::DETECTOR_LAYER);
  }
  if (command == fFrontDepthCmd) {
    fDetector->SetDepth(fFrontDepthCmd->GetNewDoubleValue(newValue), DetectorConstruction::FRONT_LAYER);
  }
  if (command == fDepthCmd) {
    fDetector->SetDepth(fDepthCmd->GetNewDoubleValue(newValue), DetectorConstruction::DETECTOR_LAYER);
  }
  if (command == fPixWidthCmd) {
    fDetector->SetPixWidth(fPixWidthCmd->GetNewDoubleValue(newValue));
  }
  if (command == fNPixCmd) {
    fDetector->SetNPix(fNPixCmd->GetNewIntValue(newValue));
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
