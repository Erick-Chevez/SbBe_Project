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
   

}


G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
     
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint* postStep = aStep->GetPostStepPoint();
    G4double fGlobalTime = preStepPoint->GetGlobalTime();
    G4Track* track = aStep->GetTrack();
    G4StepStatus stepStatus = preStepPoint->GetStepStatus();
    const G4VProcess* process = postStep->GetProcessDefinedStep();
    G4String procName = process->GetProcessName();
    G4String volName = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName(); 
    
    G4int trackID = track->GetTrackID();
    G4int parentID = track->GetParentID();
    G4int pdgEncoding = track->GetDefinition()->GetPDGEncoding();
    G4double deltaT = aStep->GetDeltaTime();
    G4ThreeVector posNeutron = preStepPoint->GetPosition();
    G4ThreeVector momNeutron = preStepPoint->GetMomentum();
    G4double fmomNeutron = momNeutron.mag();
    G4double fenergy = (1 * eV);
    G4int pdg = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
    auto secondaries = aStep->GetSecondaryInCurrentStep();
    G4double KE = preStepPoint->GetKineticEnergy();
    G4double KEev = KE / eV;
    G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();

    
    
    G4double energyDeposited = aStep->GetTotalEnergyDeposit();

    //Total Energy Deposited
    /*if (energyDeposited > 0)
    {
        fTotalEnergyDeposited += energyDeposited;
    }
    */
   //Recoil Energy deposits 
   if (procName == "hadElastic") {
   analysisManager->FillH2(0, fGlobalTime/ns, energyDeposited/keV);
   }
    

   return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent *)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();


    analysisManager->FillNtupleIColumn(0, 1, eventID);
    analysisManager->AddNtupleRow(0);
    analysisManager->FillH1(0, fTotalEnergyDeposited);

    G4cout << "Deposited energy: " << fTotalEnergyDeposited << G4endl;



}