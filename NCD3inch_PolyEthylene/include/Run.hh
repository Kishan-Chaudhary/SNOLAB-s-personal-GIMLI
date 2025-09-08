#ifndef Run_h
#define Run_h

#include "G4UserRunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4AnalysisMessenger.hh"
#include "G4SystemOfUnits.hh"
#include <cmath>

class MyRunAction : public G4UserRunAction
{
public:
  MyRunAction();
  ~MyRunAction();

  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

  G4AnalysisManager* man;
};


#endif
