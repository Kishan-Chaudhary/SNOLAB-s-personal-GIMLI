#ifndef Detector_h
#define Detector_h

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"
#include "G4Event.hh"
#include "G4Alpha.hh" 
#include "G4TouchableHistory.hh"
#include "G4CsvAnalysisManager.hh"
#include "G4VHit.hh"
class SensitiveDetector : public G4VSensitiveDetector
{
public:
  SensitiveDetector(G4String);
  ~SensitiveDetector();

private:
  virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
 /* void Initialize(G4HCofThisEvent* HCE);
  void EndOfEvent(G4HCofThisEvent* HCE);*/
  //Analysis, fill the root Ntuples
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  G4int PreviousEdepEventID;
  G4int CurrentEdepEventID;
  G4int TotalEdepEvent;
  G4int fCurrentEventID;
  G4int fCurrentTrackID;
  G4int TritonCount;
};

#endif
