#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

#include "Randomize.hh"

class G4Event;
class DetectorConstruction;
class PrimaryGeneratorMessenger;
class TH1F;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(DetectorConstruction*);    
   ~PrimaryGeneratorAction();

  public:
    void SetDefaultKinematic();
    void SetRndmBeam(G4double val)  {fRndmBeam = val;}   
    void SetEnergyHistogramFile(const G4String& fileName);
    virtual void GeneratePrimaries(G4Event*);
    
    G4ParticleGun* GetParticleGun() {return fParticleGun;}

  private:
    G4ParticleGun*             fParticleGun;
    DetectorConstruction*      fDetector;
    G4double                   fRndmBeam;       
    PrimaryGeneratorMessenger* fGunMessenger;     
    TH1F*                      fEnergyHistogram;
    G4RandGauss* fRandomizer;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


