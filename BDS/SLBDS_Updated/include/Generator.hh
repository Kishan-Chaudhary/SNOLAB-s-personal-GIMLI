#ifndef Generator_h
#define Generator_h

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Event.hh"


class PrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGenerator();
  ~PrimaryGenerator();

  virtual void GeneratePrimaries(G4Event*);

private:
  G4ParticleGun *fParticleGun;
};

#endif
