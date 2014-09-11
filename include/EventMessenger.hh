#ifndef EventMessenger_h
#define EventMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class EventAction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;

class EventMessenger: public G4UImessenger
{
  public:
    EventMessenger(EventAction* );
   ~EventMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    EventAction*    fEvent;
    G4UIdirectory*  fTestemDir;
    G4UIdirectory*  fEventDir;
    G4UIcmdWithADoubleAndUnit* fMinPixOutCmd;
    G4UIcmdWithADoubleAndUnit* fMinPixEventCmd;
    G4UIcmdWithAnInteger* fNPixEventCmd;
};

#endif
