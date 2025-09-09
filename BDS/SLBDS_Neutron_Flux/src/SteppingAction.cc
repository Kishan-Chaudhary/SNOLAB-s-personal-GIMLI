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
   // G4Track* track = step->GetTrack();
    //G4ParticleDefinition* particle = track->GetDefinition();
    //G4double ekin1 = track->GetKineticEnergy();//Default units of MeV
  /*
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4String particleName = track->GetDefinition()->GetParticleName();
    G4ThreeVector position = track->GetPosition();
    G4cout << G4endl << "Event: " << evt << ", Particle: " << particleName
        << ", Kinetic Energy: " << ekin1 / MeV << " MeV"
        << ", Position: " << position / cm << " cm" << G4endl;
       */
      /*
    if (track->GetParticleDefinition()->GetParticleName() == "alpha") {
        G4String materialName = track->GetMaterial()->GetName();
        G4ThreeVector position = track->GetPosition();
        G4double energy = track->GetKineticEnergy();

        // Output neutron information (print or store in a data structure)
        G4cout << "Alpha detected in material(Sensitive Detector) " << materialName
            << " at position " << position << " with energy " << energy / MeV << " MeV" << G4endl;
    };*/
    
    // Check if the track is a neutron
    /*
    if (track->GetParticleDefinition()->GetParticleName() == "neutron") {
        G4String materialName = track->GetMaterial()->GetName();
        G4ThreeVector position = track->GetPosition();
        G4double energy = track->GetKineticEnergy();

        // Output neutron information (print or store in a data structure)
        G4cout << "Neutron created in material " << materialName
            << " at position " << position << " with energy " << energy / MeV << " MeV" << G4endl;
            
        // You can also store this information in a data structure for further analysis
         }; 

   */
}

