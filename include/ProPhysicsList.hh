#ifndef PROPHYSICSLIST_HH
#define PROPHYSICSLIST_HH

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4OpticalPhysics.hh"
#include "G4Scintillation.hh"

class ProPhysicsList : public G4VModularPhysicsList
{
public:
    ProPhysicsList();
    ~ProPhysicsList();
};


#endif