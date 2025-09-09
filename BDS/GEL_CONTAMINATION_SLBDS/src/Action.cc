#include "Action.hh"
#include "Run.hh"
#include "PrimaryGeneratorAction.hh"
ActionInitialization::ActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::Build() const
{

    PrimaryGeneratorAction* generator = new PrimaryGeneratorAction();
    SetUserAction(generator); 

    MyRunAction* myrunAction = new MyRunAction();
    SetUserAction(myrunAction);

 
    G4cout << "Actions set: PrimaryGeneratorAction, MyRunAction" << G4endl;
}

  void ActionInitialization::BuildForMaster() const
  {
      MyRunAction* myrunAction = new MyRunAction();
      SetUserAction(myrunAction);
  }
