#include "Generator.hh"
#include "G4Geantino.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Event.hh"


PrimaryGenerator::PrimaryGenerator()
{
    // Create the particle gun
    fParticleGun = new G4ParticleGun(1);
}

PrimaryGenerator::~PrimaryGenerator()
{
    delete fParticleGun;
}

void PrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "gamma";
    G4ParticleDefinition* particle = particleTable->FindParticle("gamma");

    // Set the particle definition
    fParticleGun->SetParticleDefinition(particle);

    // Set the particle energy
    fParticleGun->SetParticleEnergy(1 * MeV);
  
  //Defining particle position to be random and isometric along outside of world sphere
  G4double dcosTheta = 2*G4UniformRand()-1.;
  G4double dsinTheta = std::sqrt(1. - dcosTheta*dcosTheta);
  G4double dphi = twopi*G4UniformRand(); 
  G4double dx = 149.9*cm*dsinTheta*std::cos(dphi),
           dy = 149.9*cm*dsinTheta*std::sin(dphi),
           dz = 149.9*cm*dcosTheta;
  G4ThreeVector position(dx,dy,dz);
  
  G4double pi = 3.14159265359;
  //Defining particle momentum direction to be shot at a random angle inwards within the world sphere
  G4double vTheta = std::acos(dcosTheta)+pi/2+G4UniformRand()*pi;
  G4double vcosTheta = std::cos(vTheta);
  G4double vsinTheta = std::sin(vTheta);
  G4double vphi =  dphi-pi/2+G4UniformRand()*pi;
  G4double vx = vsinTheta*std::cos(vphi),
           vy = vsinTheta*std::sin(vphi),
           vz = vcosTheta;
  G4ThreeVector momentum(vx,vy,vz);
    


  //Defining particle momentum to be random and isometric (Point Source)
  //G4double cosTheta = 2*G4UniformRand() - 1.;
  //G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
  //G4double phi = twopi*G4UniformRand(); 
  //G4double vx = sinTheta*std::cos(phi),
  //         vy = sinTheta*std::sin(phi),
  //         vz = cosTheta;
  //G4ThreeVector momentum(vx,vy,vz);
 
  fParticleGun->SetParticlePosition(position);
  fParticleGun->SetParticleMomentumDirection(momentum);
  //fParticleGun->SetParticleMomentum(G4UniformRand()*10*MeV);

  fParticleGun->GeneratePrimaryVertex(anEvent);
}
