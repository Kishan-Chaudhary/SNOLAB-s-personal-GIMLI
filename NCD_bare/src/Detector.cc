#include "Detector.hh"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"
#include "G4Event.hh"
#include "G4Alpha.hh" 
#include "G4TouchableHistory.hh"
#include "G4CsvAnalysisManager.hh"
//#include "EventAction.hh"

SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name), fCurrentEventID(-1), fCurrentTrackID(-1), TritonCount(0), PreviousEdepEventID(0),CurrentEdepEventID(1),TotalEdepEvent(0)
{}

SensitiveDetector::~SensitiveDetector()
{}

G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
{
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4int trackID = aStep->GetTrack()->GetTrackID();
    G4int parentID = aStep->GetTrack()->GetParentID();
    G4double edep = aStep->GetTotalEnergyDeposit();
    CurrentEdepEventID = evt;
     //Checking if the current step is part of a new event or a new track
    //if (evt != fCurrentEventID || trackID != fCurrentTrackID) {
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
                << ", Position: " << position / cm << " cm" << ", EDEP: "<<edep <<" MeV "<< G4endl;
       
        


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
            man->FillNtupleDColumn(3, 0, ekin1);
            man->AddNtupleRow(3);
        }
        
        else if (particleName == "gamma") {//Changes here, remove ekin1 comparison for previous hit setting
            if (parentID == 0) {
                man->FillNtupleDColumn(4, 0, ekin1); 
                man->AddNtupleRow(4);

            }
            if (parentID != 0)
            {
                man->FillNtupleDColumn(5, 0, ekin1);
                man->AddNtupleRow(5);
            }
        }
        
        else if (particleName == "proton") {

            if (parentID == 0) {
                man->FillNtupleDColumn(6, 0, ekin1); 
                man->AddNtupleRow(6);

            }
            if (parentID != 0)
            {
                man->FillNtupleDColumn(7, 0, ekin1);
                man->AddNtupleRow(7);
            }
        }

        else if (particle == G4Alpha::AlphaDefinition()) {
            if (parentID == 0) {
                man->FillNtupleDColumn(8, 0, ekin1);  
                man->AddNtupleRow(8);

            }
            if (parentID != 0)
            {
                man->FillNtupleDColumn(9, 0, ekin1);
                man->AddNtupleRow(9);
            } 
        }
        else if (particleName == "triton") {

            if (parentID == 0) {
                man->FillNtupleDColumn(10, 0, ekin1);
                man->AddNtupleRow(10);

            }
            if (parentID != 0)
            {
                man->FillNtupleDColumn(11, 0, ekin1);
                man->AddNtupleRow(11);
            }
			TritonCount++;
            aStep->GetTrack()->SetTrackStatus(fStopAndKill); //only for counting triton at first sight
			G4cout << G4endl<< G4endl <<"Number of Tritons: " << TritonCount << G4endl;
        }
        if (edep > 0.0 && particleName == "electron")
        {
            TotalEdepEvent += edep;
            man->FillNtupleDColumn(12, 0, edep);
            man->AddNtupleRow(12);
        }
       /* TotalEdepEvent = TotalEdepEvent + edep;
        man->FillNtupleDColumn(12, 0, TotalEdepEvent);
        man->AddNtupleRow(12);*/
        //if (particleName == "triton" || particleName == "proton")
        //{
        //     if (PreviousEdepEventID == evt)
        //      {
        //        TotalEdepEvent = TotalEdepEvent + edep;//Sum edep to previous TotalEdepEvent if same event
        //      }

      
        //    else if (PreviousEdepEventID != evt)
        //    {
        //         if (PreviousEdepEventID >= 0 && TotalEdepEvent > 0)
        //         {
        //             
        //             
        //         }
        //        TotalEdepEvent = edep;
        //        PreviousEdepEventID = evt;

        //    }
        //}
   // }

    return true;
}
