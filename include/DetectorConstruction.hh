#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Cache.hh"
#include "G4Box.hh"

class G4LogicalVolume;
class G4Material;
class G4UniformMagField;
class DetectorMessenger;
class FieldMessenger;
class G4GlobalMagFieldMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    DetectorConstruction();
   ~DetectorConstruction();

   enum ModelComponent {
	   FRONT_LAYER = 0,
	   DETECTOR_LAYER,
   };

  public:
  
     virtual G4VPhysicalVolume* Construct();
     virtual void ConstructSDandField();
     
     void SetDepth    (G4double, ModelComponent);
     void SetMaterial (G4String, ModelComponent);
     void SetPixWidth (G4double);
     void SetNPix     (G4int);

  public:
  
     const
     G4VPhysicalVolume* GetWorld()      {return fPBox;};           
                    
     G4double           GetDepth()      {return fPixDepth+fFrontDepth;};
     //G4double GetWorldDepth() { return fSensorBox->GetXHalfLength()*2.; }
     G4double GetWorldDepth() { return fFrontDepth; }
     G4Material*        GetMaterial()   {return fMaterial;};
     G4int		GetNPix()	{return fNpix;};
     G4double		GetPixWidth()	{return fPixWidth;};
     G4double		GetPixDepth()   {return fPixDepth;};
     G4Material*        GetFrontMaterial() {return fFrontMaterial;};
     G4double		GetFrontDepth()   {return fFrontDepth;};

     
     void               PrintParameters();
                       
  private:
  
     G4Box*                fSensorBox;
     G4VPhysicalVolume*    fPBox;
     
     G4double              fFrontDepth;
     G4double		   fPixWidth;
     G4double		   fPixDepth;
     G4int		   fNpix;
     G4Material*           fFrontMaterial;
     G4Material*           fMaterial;
     G4Material*           fAir;

     DetectorMessenger* fDetectorMessenger;
     G4Cache<G4GlobalMagFieldMessenger*> fFieldMessenger;

  private:
    
     void               DefineMaterials();
     G4VPhysicalVolume* ConstructVolumes();     
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#endif

