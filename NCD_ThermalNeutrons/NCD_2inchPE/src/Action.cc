#include "Action.hh"
#include "Run.hh"
#include "EventAction.hh"
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

    EventAction* eventAction = new EventAction();
    SetUserAction(eventAction);

    G4cout << "Actions set: PrimaryGeneratorAction, RunAction, EventAction" << G4endl;
}

  void ActionInitialization::BuildForMaster() const
  {
      MyRunAction* runAction = new MyRunAction();
      SetUserAction(runAction);
  }
