#include "Action.hh"
#include "Run.hh"
#include "SteppingAction.hh"
#include "PrimaryGeneratorAction.hh"
ActionInitialization::ActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::Build() const
{

    PrimaryGeneratorAction* generator = new PrimaryGeneratorAction();
    SetUserAction(generator);

    

    MyRunAction* runAction = new MyRunAction();
    SetUserAction(runAction);

    SteppingAction* steppingAction = new SteppingAction();
    SetUserAction(steppingAction);

    G4cout << "Actions set: PrimaryGeneratorAction, RunAction, SteppingAction" << G4endl;
}

  void ActionInitialization::BuildForMaster() const
  {
      MyRunAction* runAction = new MyRunAction();
      SetUserAction(runAction);
  }
