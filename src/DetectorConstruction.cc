#include "DetectorConstruction.hh"

DetectorConstruction::DetectorConstruction()
{

}
DetectorConstruction::~DetectorConstruction()
{

}


G4VPhysicalVolume *DetectorConstruction::Construct()
{
  G4bool checkOverlaps = true;

  G4NistManager *nist = G4NistManager::Instance();
  
   
  

  //////////////////World Construction/////////////////////
  
  //Dimension Constants
  G4double xWorld = 4. * m;
  G4double yWorld = 4. * m;
  G4double zWorld = 4. * m;
  
  
  //World Material
  G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");  

  //solid dimensions - logical volumes - Placement
  G4Box *solidWorld = new G4Box("solidWorld", 0.5 * xWorld, 0.5 * yWorld, 0.5 * zWorld);
  G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
  G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);
    
    
  //////////////////Envelope/////////////////////
  
  //Dimensions and Material Constants
  double inch = 2.54 * cm;
  G4double env_sizeXY = 80 * inch, env_sizeZ = 80 * inch;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");



  //solid dimensions - logical volumes 
  auto solidEnv = new G4Box("Envelope", 0.5 * env_sizeXY, 0.5 * env_sizeXY, 0.5 * env_sizeZ);  
  auto logicEnv = new G4LogicalVolume(solidEnv, env_mat, "Envelope");

  //color
  G4VisAttributes* EnvVisAtt = new G4VisAttributes(G4Colour(.25, .25, .25, .25));
  EnvVisAtt->SetForceSolid(true);
  logicEnv->SetVisAttributes(EnvVisAtt);

  //placement
  new G4PVPlacement(nullptr, G4ThreeVector(), logicEnv, "Envelope", logicWorld, false, 0, checkOverlaps);



  //////////////////PVT/////////////////////

  //PVT Constants
  G4double px_PVT = 27 * inch;
  G4double py_PVT = 26 * inch;
  G4double pz_PVT = (1.14 ) * inch;


  //PVT Material
  G4Element* C = new G4Element("Carbon", "C", 6., 12.01*g/mole);
  G4Element* H = new G4Element("Hydrogen", "H", 1., 1.008*g/mole);
  G4Material* PVT = new G4Material("PVT", 1.023*g/cm3, 2);
  PVT->AddElement(C, 9);
  PVT->AddElement(H, 10);

  //Solid - Logic - Placement
  G4Box* solidPVT = new G4Box("Box", px_PVT * 0.5, py_PVT * 0.5, pz_PVT * 0.5);
  logicPVT = new G4LogicalVolume(solidPVT, PVT, "logicPVT");
  G4VPhysicalVolume* PhysPVT = new G4PVPlacement(nullptr,G4ThreeVector(0 , 0, 0), logicPVT, "PVT", logicEnv, false, 0, checkOverlaps);
  


  ///////////////////Colors :>///////////////////// 
  G4VisAttributes* Magenta = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0, 0.5)); //Magenta
  G4VisAttributes* Orange = new G4VisAttributes(G4Color(1.0, 0.55, 0.0, 0.5)); //Orange
  G4VisAttributes* Yellow = new G4VisAttributes(G4Color(1.0, 1.0, 0.0, 0.5)); //Yellow
  G4VisAttributes* LightBlue = new G4VisAttributes(G4Color(0.4, 0.7, 1.0, 0.75)); //LightBlue
  G4VisAttributes* DarkGray = new G4VisAttributes(G4Color(0.5, 0.5, 0.5, 0.5)); //DarkGray
  G4VisAttributes* SlateBlue = new G4VisAttributes(G4Color(0.42, 0.35, 0.8, 0.75)); //SlateBlue
  G4VisAttributes* White = new G4VisAttributes(G4Color(1.0, 1.0, 1.0, 0.5)); //White
  G4VisAttributes* Black = new G4VisAttributes(G4Color(0.0, 0.0, 0.0, 0.75)); //White
  Magenta->SetForceSolid(true);
  Orange->SetForceSolid(true);
  Yellow->SetForceSolid(true);
  LightBlue->SetForceSolid(true);
  DarkGray->SetForceSolid(true);
  SlateBlue->SetForceSolid(true);
  White->SetForceSolid(true);
  Black->SetForceSolid(true);


  //PVT
  logicPVT->SetVisAttributes(Yellow);

  return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
    SensitiveDetector *sensDet = new SensitiveDetector("SensitiveDetector");
    logicPVT->SetSensitiveDetector(sensDet); //Black
    
    G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
}
    