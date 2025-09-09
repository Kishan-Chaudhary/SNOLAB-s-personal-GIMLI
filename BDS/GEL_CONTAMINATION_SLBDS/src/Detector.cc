#include "Detector.hh"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"
#include "G4Alpha.hh" 
#include "G4TouchableHistory.hh"
#include "G4CsvAnalysisManager.hh"

SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name), fCurrentEventID(-1), fCurrentTrackID(-1)
{}

SensitiveDetector::~SensitiveDetector()
{}

G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
{
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4int trackID = aStep->GetTrack()->GetTrackID();
    G4int parentID = aStep->GetTrack()->GetParentID();


     //Checking if the current step is part of a new event or a new track
    if (evt != fCurrentEventID || trackID != fCurrentTrackID) {
        fCurrentEventID = evt;
        fCurrentTrackID = trackID;

        G4ParticleDefinition* particle = aStep->GetTrack()->GetDefinition();

        G4StepPoint* preStepPoint = aStep->GetPreStepPoint();

        // Access the kinetic energy at the pre-step
        G4double ekin1 = preStepPoint->GetKineticEnergy();
        G4double CutOff = 10 * keV;

        // Print or store particle information for debugging
        G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();
       
            G4ThreeVector position = aStep->GetTrack()->GetPosition();
            G4cout << G4endl << "Event in Sensitive Detector(BDS): " << evt << ", TrackID: " << trackID << ", ParentID: "<<parentID << ", Particle: " << particleName
                << ", Kinetic Energy: " << ekin1 << " MeV"
                << ", Position: " << position / cm << " cm" << G4endl;
       
        


        //Analysis, fill the root Ntuples
        G4AnalysisManager* man = G4AnalysisManager::Instance();

        if (ekin1 > CutOff) {
            man->FillNtupleDColumn(0, 1, ekin1);       
        }
        else {
            man->FillNtupleDColumn(0, 2, ekin1);
        }
        man->AddNtupleRow(0);


        // Particle specific Ntuples, makes seperate Ntuples for primary and secondary particles
        if (particleName == "neutron") {
            if (parentID == 0) {
                man->FillNtupleDColumn(1, 0, ekin1);  
                man->AddNtupleRow(1);
            }
            if (parentID != 0)
            {
                man->FillNtupleDColumn(2, 0, ekin1);
                man->AddNtupleRow(2);
            }
        }
        
        else if (particleName == "gamma") {
            if (parentID == 0) {
                man->FillNtupleDColumn(3, 0, ekin1); 
                man->AddNtupleRow(3);

            }
            if (parentID != 0)
            {
                man->FillNtupleDColumn(4, 0, ekin1);
                man->AddNtupleRow(4);
            }
        }
        
        else if (particleName == "proton") {

            if (parentID == 0) {
                man->FillNtupleDColumn(5, 0, ekin1); 
                man->AddNtupleRow(5);

            }
            if (parentID != 0)
            {
                man->FillNtupleDColumn(6, 0, ekin1);
                man->AddNtupleRow(6);
            }
        }

        else if (particle == G4Alpha::AlphaDefinition()) {
            if (parentID == 0) {
                man->FillNtupleDColumn(7, 0, ekin1);  
                man->AddNtupleRow(7);

            }
            if (parentID != 0)
            {
                man->FillNtupleDColumn(8, 0, ekin1);
                man->AddNtupleRow(8);
            } 
        }


    }

    return true;
}
