#include "ProPhysicsList.hh"

ProPhysicsList::ProPhysicsList()
{
    //EM Physics
    RegisterPhysics(new G4EmStandardPhysics());

     // General decay
    RegisterPhysics(new G4DecayPhysics());

    // Hadronic physics including neutrons
    RegisterPhysics(new G4HadronElasticPhysicsHP());
    RegisterPhysics(new G4HadronPhysicsQGSP_BIC_HP());

    // Optional: neutron tracking cuts
    RegisterPhysics(new G4NeutronTrackingCut());
    RegisterPhysics(new G4HadronPhysicsQGSP_BERT_HP());
    
    //Optical
    RegisterPhysics(new G4OpticalPhysics());
   
}

ProPhysicsList::~ProPhysicsList()
{

}