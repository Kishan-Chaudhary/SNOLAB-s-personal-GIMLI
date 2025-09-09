#include "Action.hh"
#include "Run.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::Build() const
{
    PrimaryGenerator* generator = new PrimaryGenerator();
    SetUserAction(generator);

    MyRunAction* runAction = new MyRunAction();
    SetUserAction(runAction);


    //The SteppingAction should be edited for step actions.
    SteppingAction* steppingAction = new SteppingAction();
    SetUserAction(steppingAction);

    G4cout << "Actions set: PrimaryGenerator, RunAction, SteppingAction" << G4endl;
}

  void ActionInitialization::BuildForMaster() const
  {
      MyRunAction* runAction = new MyRunAction();
      SetUserAction(runAction);
  }
