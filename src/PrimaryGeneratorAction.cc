#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"

#include "TFile.h"
#include "TH1F.h"

#include "OutputManager.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction( DetectorConstruction* det)
:G4VUserPrimaryGeneratorAction(),
 fParticleGun(0),
 fDetector(det),
 fGunMessenger(0),
 fThetaHistogram(0),
 fEnergyHistogram(0),
 fEnergyLogScale(false)
{
  fParticleGun  = new G4ParticleGun(1);
  SetDefaultKinematic();

  //create a messenger for this class
  fGunMessenger = new PrimaryGeneratorMessenger(this);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fGunMessenger;

  if (fEnergyHistogram) {
    delete fEnergyHistogram;
  }
  if (fThetaHistogram) {
    delete fThetaHistogram;
  }
}

void PrimaryGeneratorAction::SetDefaultKinematic()
{
  G4ParticleDefinition* particle
           = G4ParticleTable::GetParticleTable()->FindParticle("e-");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(100*MeV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,-0.5*fDetector->GetDepth()));
}

void PrimaryGeneratorAction::SetThetaHistogramFile(const G4String& fileName)
{
  if (fThetaHistogram) {
    delete fThetaHistogram;
    fThetaHistogram = 0;
  }

  if (fileName == "") {
    return;
  }

  TFile *f = TFile::Open(fileName);
  fThetaHistogram = (TH1F*) f->Get("theta");
  fThetaHistogram->SetDirectory(0);
  f->Close();
  fThetaHistogram->Print();
}

void PrimaryGeneratorAction::SetEnergyHistogramFile(const G4String& fileName)
{
  if (fEnergyHistogram) {
    delete fEnergyHistogram;
    fEnergyHistogram = 0;
  }

  if (fileName == "") {
    return;
  }

  TFile *f = TFile::Open(fileName);
  fEnergyHistogram = (TH1F*) f->Get("energy");
  if (!fEnergyHistogram) {
    // support old naming convention
    fEnergyHistogram = (TH1F*) f->Get("particleEnergy");
  }
  fEnergyHistogram->SetDirectory(0);
  f->Close();
  fEnergyHistogram->Print();
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if (fThetaHistogram) {
    // if random theta is specified, pick a uniform random phi direction as well.
    double newPhi = 2*3.14159*G4UniformRand();
    double newTheta = fThetaHistogram->GetRandom();
    G4ThreeVector newMomentum;
    newMomentum.setRThetaPhi(1, newTheta, newPhi);
    fParticleGun->SetParticleMomentumDirection(newMomentum);
  }

  if (fEnergyHistogram) {
    double newEnergy = fEnergyHistogram->GetRandom();
    if (fEnergyLogScale) {
      newEnergy = pow(10., newEnergy);
    }
    fParticleGun->SetParticleEnergy(newEnergy);
  }

  fParticleGun->GeneratePrimaryVertex(anEvent);

  G4double e0 = fParticleGun->GetParticleEnergy();
  G4int pid = fParticleGun->GetParticleDefinition()->GetPDGEncoding();
  G4ThreeVector p = fParticleGun->GetParticleMomentumDirection();

  OutputManager::Instance()->setParticle(pid, e0, p.getTheta(), p.getPhi());
}
