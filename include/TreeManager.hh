#ifndef TreeManager_h
#define TreeManager_h 1

#include "globals.hh"

#include "g4root.hh"

class TreeManager
{
  public:
    TreeManager();
   ~TreeManager();

  private:
    void Book();

    G4String fFileName;
};

#endif //TreeManager_h
