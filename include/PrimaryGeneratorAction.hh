#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4Event;
class DetectorConstruction;
class PrimaryGeneratorMessenger;
class TH1F;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(DetectorConstruction*);    
   ~PrimaryGeneratorAction();

  public:
    void SetDefaultKinematic();
    void SetThetaHistogramFile(const G4String& fileName);
    void SetEnergyHistogramFile(const G4String& fileName);
    void SetEnergyLogScale(bool isLog) { fEnergyLogScale = isLog; }
    virtual void GeneratePrimaries(G4Event*);
    
    G4ParticleGun* GetParticleGun() {return fParticleGun;}

  private:
    G4ParticleGun*             fParticleGun;
    DetectorConstruction*      fDetector;
    PrimaryGeneratorMessenger* fGunMessenger;     
    TH1F*                      fThetaHistogram;
    TH1F*                      fEnergyHistogram;
    bool                       fEnergyLogScale;
};

#endif
