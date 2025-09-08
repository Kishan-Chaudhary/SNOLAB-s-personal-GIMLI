#include <iostream>

#include "G4RunManager.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4ParticleDefinition.hh"
#include "DetectorConstruction.hh"

#include "PhysicsList.hh"
#include "HadronElasticPhysicsHP.hh"
#include "GammaNuclearPhysics.hh"
#include "ElectromagneticPhysics.hh"

#include "Action.hh"

int main(int argc,char** argv) {
 
  //choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
 
  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager();     
  runManager->SetUserInitialization(new DetectorConstruction());
  runManager->SetUserInitialization(new PhysicsList());
  runManager->SetUserInitialization(new ActionInitialization());
  runManager->Initialize();

  //Visualization initialize
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();    
  

  // get the pointer to the User Interface manager 
  G4UImanager* UI = G4UImanager::GetUIpointer();
  // UI->SetCoutDestination(this);
  // 
  //UI initialize and termination
  G4UIExecutive * ui = new G4UIExecutive(argc,argv);      
  
  UI->ApplyCommand("/run/numberOfThreads 10");


  //Use verbosity levels if troubleshooting is required 
  //UI->ApplyCommand("/run/verbose 2");          // Set run manager verbosity
  //UI->ApplyCommand("/event/verbose 2");        // Set event verbosity
  //UI->ApplyCommand("/tracking/verbose 2");     // Set tracking verbosity
  //UI->ApplyCommand("/step/verbose 2");         // Set step verbosity
  UI->ApplyCommand("/process/verbose 2");      // Set process verbosity
  
  ////Starts Dawnfile
  //UI->ApplyCommand("/vis/open DAWNFILE");
  //UI->ApplyCommand("/vis/scene/add/trajectories");  // Add trajectories to visualization
  //UI->ApplyCommand("/vis/scene/endOfEventAction accumulate");  // Accumulate scenes at end of each event
  //UI->ApplyCommand("/vis/verbose errors 2");                   // Enable verbose output for errors
  //UI->ApplyCommand("/vis/verbose warnings 2");                 // Enable verbose output for warnings
  ////Sets View to a nice angle
  //UI->ApplyCommand("/vis/viewer/set/viewpointThetaPhi 60. 30.");
  ////Draws Geometry
  //UI->ApplyCommand("/vis/drawVolume");
  ////Refreshes for new events
  //UI->ApplyCommand("/vis/viewer/set/autoRefresh true");
  ////Draws particle trajectories
  //UI->ApplyCommand("/vis/scene/add/trajectories smooth");
  //UI->ApplyCommand("/process/had/deex/correlatedGamma true");


  //Particle Filter: Use it to see particular particle tracks in visualization 
  //UI->ApplyCommand("/vis/filtering/trajectories/create/particleFilter");
  //UI->ApplyCommand("/vis/filtering/trajectories/particleFilter-0/add alpha");

  //Sets neutrons to be drawn as blue
  UI->ApplyCommand("/vis/modeling/trajectories/drawByParticleID-0/set neutron blue");
  //Sets the max amount of events that are visualized
  UI->ApplyCommand("/vis/scene/endOfEventAction accumulate 100");
  //Sets the interval between gun particles
  UI->ApplyCommand("/gun/time 1000 s");
 

  //Turn the Fission Fragements on,but set verbose inside UI itself
  UI->ApplyCommand("/process/had/particle_hp/produce_fission_fragment true");
  UI->ApplyCommand("/control/execute AmBe_TestSource.mac");
 /* UI->ApplyCommand("/control/execute macro/MultipleRuns.mac");*/
  //Wendt Fission Model
  //UI->ApplyCommand("/process/had/particle_hp/use_Wendt_fission_model true");

  /*ui->SessionStart();
  delete ui;*/
  
  //Visualization termination
  delete visManager;

  //Job termination 
  //
  delete runManager;

  return 0;
}
