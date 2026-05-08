
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>


#include "G4RunManagerFactory.hh"
//#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4SteppingVerbose.hh"
#include "G4UIExecutive.hh"
#include "G4VisManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "Randomize.hh"

#include "ProPhysicsList.hh"
#include "DetectorConstruction.hh"
#include "ActionInit.hh"
#include "MyRunAction.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  G4UIExecutive* ui = nullptr;
  
  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
  }

  const auto seed = static_cast<long>(
      std::chrono::high_resolution_clock::now().time_since_epoch().count());
  G4Random::setTheSeed(seed);
  G4cout << "RNG seed: " << seed << G4endl;

  auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
  unsigned int requestedThreads = std::thread::hardware_concurrency();
  if (requestedThreads == 0) {
    requestedThreads = 12;
  }

  if (const char* envThreads = std::getenv("SBBE_THREADS")) {
    const long parsed = std::strtol(envThreads, nullptr, 10);
    if (parsed > 0) {
      requestedThreads = static_cast<unsigned int>(parsed);
    }
  }

  if (argc >= 3) {
    const long parsed = std::strtol(argv[2], nullptr, 10);
    if (parsed > 0) {
      requestedThreads = static_cast<unsigned int>(parsed);
    }
  }

  runManager->SetNumberOfThreads(static_cast<G4int>(requestedThreads));
  G4cout << "THREADS: " << runManager->GetNumberOfThreads() << G4endl;
  
  // Physics List
  runManager->SetUserInitialization(new ProPhysicsList());

  //Detector Contruction
  runManager->SetUserInitialization(new DetectorConstruction());
  
// User action initialization
  runManager->SetUserInitialization(new ActionInit());

  // Initialize visualization with the default graphics system
  auto visManager = new G4VisExecutive(argc, argv);
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  auto UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if (!ui) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
    // Manual merging is preferred; keep per-run ROOT files and avoid automatic hadd.
    // RunAction::MergeCompletedRunFiles();
  }
  else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
