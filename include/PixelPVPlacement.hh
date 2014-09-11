#include "G4PVPlacement.hh"

class PixelPVPlacement : public G4PVPlacement {
	public:
	PixelPVPlacement(
		G4RotationMatrix *pRot,
            const G4ThreeVector &tlate,
                  G4LogicalVolume *pCurrentLogical,
            const G4String& pName,
                  G4LogicalVolume *pMotherLogical,
                  G4bool pMany,
                  G4int  pCopyNo,
                  G4bool pSurfChk=false) :
		G4PVPlacement(pRot, tlate, pCurrentLogical, pName, pMotherLogical, pMany, pCopyNo, pSurfChk) {};
	int idx_x;
	int idx_y;
};
