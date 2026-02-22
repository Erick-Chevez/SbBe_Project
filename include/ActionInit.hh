#ifndef ACTIONINIT_HH
#define ACTIONINIT_HH

#include "G4VUserActionInitialization.hh"

#include "PrimaryGenerator.hh"
#include "MyRunAction.hh"
class ActionInit : public G4VUserActionInitialization
{
public:
    ActionInit();
    ~ActionInit();

    virtual void BuildForMaster() const;
    virtual void Build() const;

};



#endif
