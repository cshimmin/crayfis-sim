#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "PixelPVPlacement.hh"
#include "G4UniformMagField.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4RunManager.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <sstream>

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(),fSensorBox(0),fPBox(0), fFrontMaterial(0), fMaterial(0)
{
  fPixWidth = 1.5*um;
  fPixDepth = 700*um;
  fFrontDepth = 1*mm;
  fNpix = 1000;
  DefineMaterials();
  SetMaterial("Air", FRONT_LAYER);
  SetMaterial("Silicon", DETECTOR_LAYER);
  fDetectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ delete fDetectorMessenger;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
  //
  // define Elements
  //
  G4double z,a;
  
  G4Element* H  = new G4Element("Hydrogen" ,"H" , z= 1., a=   1.01*g/mole);
  G4Element* C  = new G4Element("Hydrogen" ,"C" , z= 6., a=  12.00*g/mole);
  G4Element* N  = new G4Element("Nitrogen" ,"N" , z= 7., a=  14.01*g/mole);
  G4Element* O  = new G4Element("Oxygen"   ,"O" , z= 8., a=  16.00*g/mole);
  G4Element* Ge = new G4Element("Germanium","Ge", z=32., a=  72.59*g/mole);
  G4Element* Bi = new G4Element("Bismuth"  ,"Bi", z=83., a= 208.98*g/mole);
  
  //
  // define materials
  //
  G4double density;
  G4int ncomponents, natoms;
  G4double fractionmass;  
  
  G4Material* Air = 
  new G4Material("Air", density= 1.290*mg/cm3, ncomponents=2);
  Air->AddElement(N, fractionmass=70.*perCent);
  Air->AddElement(O, fractionmass=30.*perCent);

  fAir = Air;

  G4Material* H2l = 
  new G4Material("H2liquid", density= 70.8*mg/cm3, ncomponents=1);
  H2l->AddElement(H, fractionmass=1.);

  G4Material* H2O = 
  new G4Material("Water", density= 1.000*g/cm3, ncomponents=2);
  H2O->AddElement(H, natoms=2);
  H2O->AddElement(O, natoms=1);
  ///H2O->SetChemicalFormula("H_2O");
  H2O->GetIonisation()->SetMeanExcitationEnergy(78.0*eV);

  density = 0.001*mg/cm3;
  G4Material* CO2 = new G4Material("CO2", density, ncomponents=2);
  CO2->AddElement(C, natoms=1);
  CO2->AddElement(O, natoms=2);
  
  new G4Material("D2_gas", z=2., a= 2.0141*g/mole, density= 0.036*mg/cm3);

  new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);

  new G4Material("Aluminium"  , z=13., a= 26.98*g/mole, density= 2.700*g/cm3);

  new G4Material("Silicon"    , z=14., a= 28.09*g/mole, density= 2.330*g/cm3);
  
  new G4Material("Chromium"   , z=24., a= 51.99*g/mole, density= 7.140*g/cm3);
  
  new G4Material("Germanium"  , z=32., a= 72.61*g/mole, density= 5.323*g/cm3);
  
  G4Material* BGO = 
  new G4Material("BGO", density= 7.10*g/cm3, ncomponents=3);
  BGO->AddElement(O , natoms=12);
  BGO->AddElement(Ge, natoms= 3);
  BGO->AddElement(Bi, natoms= 4);  

  new G4Material("Iron"       , z=26., a= 55.85*g/mole, density= 7.870*g/cm3);

  new G4Material("Tungsten"   , z=74., a=183.85*g/mole, density= 19.30*g/cm3);
  
  new G4Material("Gold"       , z=79., a=196.97*g/mole, density= 19.32*g/cm3);
  
  new G4Material("Lead"       , z=82., a=207.19*g/mole, density= 11.35*g/cm3);

  new G4Material("Uranium"    , z=92., a=238.03*g/mole, density= 18.95*g/cm3);

  
  G4Material* argonGas =   
  new G4Material("ArgonGas", z=18, a=39.948*g/mole, density= 1.782*mg/cm3,
                 kStateGas, 273.15*kelvin, 1*atmosphere);
                       
 G4Material* butane =
 new G4Material("Isobutane",density= 2.42*mg/cm3, ncomponents=2,
                 kStateGas,273.15*kelvin, 1*atmosphere);
 butane->AddElement(C, natoms=4);
 butane->AddElement(H, natoms=10);
 
 G4Material* ArButane =
 new G4Material("ArgonButane", density= 1.835*mg/cm3, ncomponents=2,
                 kStateGas,273.15*kelvin,1.*atmosphere);
 ArButane->AddMaterial(argonGas, fractionmass=70*perCent);
 ArButane->AddMaterial(butane ,  fractionmass=30*perCent);

  //G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  G4Box* sensorBox = new G4Box("SensorBox", 0.5*(fPixDepth+fFrontDepth), 0.5*(fNpix*fPixWidth), 0.5*(fNpix*fPixWidth));
  fSensorBox = sensorBox;
  G4LogicalVolume* sensorBoxL = new G4LogicalVolume(sensorBox, fAir, "SensorBoxL");
  fPBox = new G4PVPlacement(0, G4ThreeVector(), sensorBoxL, "SensorBoxP", 0, false, 0);

  if (fPixDepth > 0) {
  G4Box* frontBox = new G4Box("FrontBox", fFrontDepth/2., fNpix*fPixWidth/2., fNpix*fPixWidth/2.);
  G4LogicalVolume* frontBoxL = new G4LogicalVolume(frontBox, fFrontMaterial, "FrontBoxL");
  G4PVPlacement *fbP = new G4PVPlacement(0,
                                         G4ThreeVector(fPixDepth/2., 0, 0),
                                         frontBoxL,
                                         "front_box",
                                         sensorBoxL,
                                         false,
                                         0);
  }
  
  G4Box* pixBox = new G4Box("PixelBox", fPixDepth/2., fPixWidth/2., fPixWidth/2.);
  G4LogicalVolume* pixBoxL = new G4LogicalVolume(pixBox, fMaterial, "pixBoxL");

  //std::cout << "BOARK: Setting material to: " << fMaterial->GetName() << std::endl;
  G4cout << "Setting material to: " << fMaterial->GetName() << G4endl;
  PixelPVPlacement *pixBoxP;
  for (int ipix = 0; ipix<fNpix; ++ipix) {
    for (int jpix = 0; jpix<fNpix; ++jpix) {
      std::stringstream ss;
      ss << "pixelP";
      //ss << "pix_"<<ipix<<"_"<<jpix;
      pixBoxP = new PixelPVPlacement(0,
                                     G4ThreeVector(-fFrontDepth/2.,
                                     ipix*fPixWidth - (fNpix-1)*fPixWidth/2.,
                                     jpix*fPixWidth - (fNpix-1)*fPixWidth/2.),
                                     pixBoxL,
                                     ss.str(),
                                     sensorBoxL,
                                     false,
                                     0);
      pixBoxP->idx_x = ipix;
      pixBoxP->idx_y = jpix;
    }
  }
                          
  PrintParameters();
  
  //always return the root volume
  //
  return fPBox;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintParameters()
{
  G4cout << "There are " << fNpix << " pixels of dimension "
	  << G4BestUnit(fPixWidth, "Length") << " x " << G4BestUnit(fPixDepth, "Length")
	  << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetMaterial(G4String materialChoice, ModelComponent comp)
{
  // search the material by its name
  ////G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
  G4Material* pttoMaterial = 
     G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  
  if (pttoMaterial) {
    switch (comp) {
      case FRONT_LAYER:
        fFrontMaterial = pttoMaterial;
        break;
      case DETECTOR_LAYER:
        fMaterial = pttoMaterial;
        break;
      default:
        break;
    }
  }
  else {
    G4cout << "\n--> warning from DetectorConstruction::SetMaterial : "
           << materialChoice << " not found" << G4endl;
  }
  G4RunManager::GetRunManager()->PhysicsHasBeenModified();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetDepth(G4double value, ModelComponent comp)
{
  switch (comp) {
    case FRONT_LAYER:
      fFrontDepth = value;
      break;
    case DETECTOR_LAYER:
      fPixDepth = value;
      break;
    default:
      break;
  }
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetPixWidth(G4double value)
{
  fPixWidth = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetNPix(G4int value)
{
  fNpix = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
  // notify the output manager of the new pixel count
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

void DetectorConstruction::ConstructSDandField()
{
    if ( fFieldMessenger.Get() == 0 ) {
        // Create global magnetic field messenger.
        // Uniform magnetic field is then created automatically if
        // the field value is not zero.
        G4ThreeVector fieldValue = G4ThreeVector();
        G4GlobalMagFieldMessenger* msg =
        new G4GlobalMagFieldMessenger(fieldValue);
        //msg->SetVerboseLevel(1);
        G4AutoDelete::Register(msg);
        fFieldMessenger.Put( msg );
        
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
