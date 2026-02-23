#include "MyRunAction.hh"

RunAction::RunAction()
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetNtupleMerging(true);


    //0 Histo1D
    analysisManager->CreateH1("Edep", "Energy deposit", 100, 0., 6 * MeV) ; 
    analysisManager->SetH1XAxisTitle(0, "MeV");
    analysisManager->SetH1YAxisTitle(0, "Counts");

    //0 Histo2D
    analysisManager->CreateH2("EdepVsToF", "Energy Deposited vs Time of Flight", 10000, 0., 500, 10000, 0, 100) ; 
    analysisManager->SetH2XAxisTitle(0, "ToF(ns)");
    analysisManager->SetH2YAxisTitle(0, "Edep (keV)");
    


    //0 ntuple Gammas
    analysisManager->CreateNtuple("GammaEdep", "Gamma energy deposits in NaI");
    analysisManager->CreateNtupleDColumn("fGlobalTime");
    analysisManager->CreateNtupleIColumn("EventId");
    analysisManager->FinishNtuple(0);

   

}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run *run)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    

    
    G4int runID = run->GetRunID();

    std::stringstream strRunID;
    strRunID << runID;

    analysisManager->OpenFile("output" + strRunID.str() + ".root");
    

}


void RunAction::EndOfRunAction(const G4Run *run)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    
    analysisManager->Write();

    analysisManager->CloseFile();

    G4int runId = run->GetRunID();

    G4cout << "Finishing run " << runId << G4endl;

}