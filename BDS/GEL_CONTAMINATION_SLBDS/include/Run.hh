#ifndef Run_h
#define Run_h

#include "G4UserRunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Timer.hh"
#include <cmath>

class MyRunAction : public G4UserRunAction
{
public:
  MyRunAction();
  ~MyRunAction();

  virtual void BeginOfRunAction(const G4Run*) override;
  virtual void EndOfRunAction(const G4Run*) override;

private:
    G4Timer* timer;
};


#endif
