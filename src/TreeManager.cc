#include "TreeManager.hh"
#include "G4UnitsTable.hh"

TreeManager::TreeManager()
  : fFileName("testem1")
{
  Book();
}

TreeManager::~TreeManager()
{
  // not sure if both TreeManager and
  // HistoManager should delete this...
  // assuming it's singleton and will
  // make new instances as needed.
  delete G4AnalysisManager::Instance();
}

void TreeManager::Book()
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetFileName(fFileName);
  analysisManager->SetVerboseLevel(1);
  //analysisManager->SetFirst
}
