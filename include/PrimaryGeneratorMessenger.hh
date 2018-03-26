#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;

class PrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    PrimaryGeneratorMessenger(PrimaryGeneratorAction*);
   ~PrimaryGeneratorMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    PrimaryGeneratorAction* Action;
    
    G4UIdirectory*             fGunDir; 
    G4UIcmdWithAString*        fThetaDist;
    G4UIcmdWithAString*        fEnergyDist;
    G4UIcmdWithABool*          fEnergyScale;
};

#endif

