#include "ActionInit.hh"

ActionInit::ActionInit()
{}
ActionInit::~ActionInit()
{}

void ActionInit::BuildForMaster() const
{
    RunAction *runAction = new RunAction();
    SetUserAction(runAction);

}

void ActionInit::Build() const
{
    PrimaryGenerator *generator = new PrimaryGenerator();
    SetUserAction(generator);

    RunAction *runAction = new RunAction();
    SetUserAction(runAction);

}