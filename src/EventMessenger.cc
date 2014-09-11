#include "EventMessenger.hh"

#include "EventAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"

EventMessenger::EventMessenger(EventAction *evt)
:G4UImessenger(), fEvent(evt),
 fTestemDir(0),
 fEventDir(0),
 fMinPixOutCmd(0),
 fMinPixEventCmd(0),
 fNPixEventCmd(0)
{
  fTestemDir = new G4UIdirectory("/testem/");
  fTestemDir->SetGuidance("commands specific to this example");

  fEventDir = new G4UIdirectory("/testem/evt/");
  fEventDir->SetGuidance("event/pixel filtering commands");

  fMinPixOutCmd = new G4UIcmdWithADoubleAndUnit("/testem/evt/setMinPixOut", this);
  fMinPixOutCmd->SetGuidance("Set minimum pixel energy to be written out.");
  fMinPixOutCmd->SetParameterName("MinPixOut",false);
  fMinPixOutCmd->SetRange("MinPixOut>=0.");
  fMinPixOutCmd->SetUnitCategory("Energy");
  fMinPixOutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fMinPixOutCmd->SetToBeBroadcasted(false);

  fMinPixEventCmd = new G4UIcmdWithADoubleAndUnit("/testem/evt/setMinPixEvent", this);
  fMinPixEventCmd->SetGuidance("Set minimum pixel energy required to keep event.");
  fMinPixEventCmd->SetParameterName("MinPixEvent",false);
  fMinPixEventCmd->SetRange("MinPixEvent>=0.");
  fMinPixEventCmd->SetUnitCategory("Energy");
  fMinPixEventCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fMinPixEventCmd->SetToBeBroadcasted(false);

  fNPixEventCmd = new G4UIcmdWithAnInteger("/testem/evt/setNPixEvent",this);
  fNPixEventCmd->SetGuidance("Set number of pixels above threshold to required to keep events.");
  fNPixEventCmd->SetParameterName("NPixEvent",false);
  fNPixEventCmd->SetRange("NPixEvent>0");
  fNPixEventCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fNPixEventCmd->SetToBeBroadcasted(false);

}

EventMessenger::~EventMessenger() {
	delete fMinPixOutCmd;
}

void EventMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if( command == fMinPixOutCmd )
   { fEvent->SetMinPixOut(fMinPixOutCmd->GetNewDoubleValue(newValue)); }
  if( command == fMinPixEventCmd )
   { fEvent->SetMinPixEvent(fMinPixEventCmd->GetNewDoubleValue(newValue)); }
  if( command == fNPixEventCmd )
   { fEvent->SetNPixEvent(fNPixEventCmd->GetNewIntValue(newValue)); }
}
