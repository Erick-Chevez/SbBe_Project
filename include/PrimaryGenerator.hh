#ifndef PRIMARYGENERATOR_HH
#define PRIMARYGENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4GeneralParticleSource.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4Colour.hh"
#include "Randomize.hh"              // for G4UniformRand()
#include "G4PhysicalConstants.hh"    // (optional, but common)
#include "G4ThreeVector.hh"          // for G4ThreeVector
#include "G4RandomDirection.hh"

#include "CLHEP/Random/Random.h"
#include "CLHEP/Units/PhysicalConstants.h"



class PrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public: 
    PrimaryGenerator();
    ~PrimaryGenerator();

    virtual void GeneratePrimaries(G4Event *);

private:
    //G4ParticleGun *fParticleGun;
    G4GeneralParticleSource* fGPS;


};

#endif 