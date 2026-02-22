#include "SensitiveDetector.hh"


SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    fTotalEnergyDeposited = 0.;
}

SensitiveDetector::~SensitiveDetector()
{

}

void SensitiveDetector::Initialize(G4HCofThisEvent *)
{
    
    fTotalEnergyDeposited = 0. ;
   
    fEdepPVT = 0. ;
    
    m_firstHDPEEntry.clear(); 
    fFirstShieldEntry.clear(); 
    edepByProton.clear();
    edepByTriton.clear();
    edepByGamma.clear();

}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent *)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    analysisManager->FillH1(0, fTotalEnergyDeposited);
    if (fEdepPVT > 0){
    analysisManager->FillH1(1, fEdepPVT); 
    }


    G4double totalEdep = 0.0;


    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    

    for (const auto& [eventID, edep] : edepByGamma) {
        //if (edep >= 4.3 * MeV && edep <= 4.5 * MeV) {
            analysisManager->FillNtupleDColumn(0, 0, edep);
            analysisManager->FillNtupleIColumn(0, 1, eventID);
            analysisManager->AddNtupleRow(0);
        
        //}
    }




}






G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
     
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint* postStep = aStep->GetPostStepPoint();
    G4StepStatus stepStatus = preStepPoint->GetStepStatus();
    const G4VProcess* process = postStep->GetProcessDefinedStep();
    G4String volName = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName(); 
    G4Track* track = aStep->GetTrack();
    G4int trackID = track->GetTrackID();
    G4int parentID = track->GetParentID();
    G4int pdgEncoding = track->GetDefinition()->GetPDGEncoding();
    G4double fGlobalTime = preStepPoint->GetGlobalTime();
    G4double deltaT = aStep->GetDeltaTime();
    G4ThreeVector posNeutron = preStepPoint->GetPosition();
    G4ThreeVector momNeutron = preStepPoint->GetMomentum();
    G4double fmomNeutron = momNeutron.mag();
    G4double fenergy = (1 * eV);
    G4int pdg = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
    G4String procName = process->GetProcessName();
    auto secondaries = aStep->GetSecondaryInCurrentStep();
    G4double KE = preStepPoint->GetKineticEnergy();
    G4double KEev = KE / eV;
    G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();

    
    
    G4double energyDeposited = aStep->GetTotalEnergyDeposit();
    
    //Energy Depositied in NaI (gammas)
    if (volName == "PVT"  && energyDeposited > 0) {
        //G4cout << pdg << ": " << energyDeposited << G4endl ; 
        edepByGamma[eventID] += energyDeposited;
    }
    
    //Energy depositied by events
    if (energyDeposited > 0)
    {
        fTotalEnergyDeposited += energyDeposited;
    
    
        if (volName == "PVT" && particleName != "mu-" ) {
            G4cout << particleName << G4endl;

            fEdepPVT += energyDeposited;
        }   
        
        
    }

    

    return true;
}

