#include "SteppingAction.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4RunManager.hh"

SteppingAction::SteppingAction() : G4UserSteppingAction()
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
//Fill this part for step wise information and storing in AnalysisManager
   
}

