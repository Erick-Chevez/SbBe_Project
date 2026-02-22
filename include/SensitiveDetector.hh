#ifndef SENSITIVEDETECTOR_HH
#define SENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"

#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include <set>                      
#include <unordered_set>
#include <unordered_map>   
class SensitiveDetector : public G4VSensitiveDetector
{
public:
    SensitiveDetector(G4String);
    ~SensitiveDetector();

private:
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
    static std::set<G4int> countedTracks;

    virtual void Initialize(G4HCofThisEvent*) override;
    virtual void EndOfEvent(G4HCofThisEvent *) override;

    G4double fTotalEnergyDeposited;
    G4double fEdepPVT;
    std::map<G4int, G4double> entryTimes;
    std::unordered_set<G4int> m_firstHDPEEntry;
    std::unordered_map<G4int, G4double> fFirstShieldEntry; 
    std::map<G4int, G4double> edepByProton;
    std::map<G4int, G4double> edepByTriton;
    std::map<G4int, G4double> edepByGamma;
};

#endif