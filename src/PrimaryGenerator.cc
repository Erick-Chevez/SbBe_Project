#include "PrimaryGenerator.hh"



PrimaryGenerator::PrimaryGenerator()
{
    //fParticleGun = new G4ParticleGun(1);
    fGPS = new G4GeneralParticleSource();

    

}

PrimaryGenerator::~PrimaryGenerator()
{
   // delete fParticleGun;
    delete fGPS;

}
void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    //Create Vertex
    //fParticleGun->GeneratePrimaryVertex(anEvent);
    fGPS->GeneratePrimaryVertex(anEvent);
    
   
    



    ///////////////////Logging Neutrons Starting KE and Primaries Generated///////////////////// 
    G4PrimaryParticle* primary = anEvent->GetPrimaryVertex(0)->GetPrimary(0);
    auto analysisManager = G4AnalysisManager::Instance();
    
    









}