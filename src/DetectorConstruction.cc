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
  G4double xWorld = 1. * m;
  G4double yWorld = 1. * m;
  G4double zWorld = 1. * m;
  
  
  //World Material
  G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");  

  //solid dimensions - logical volumes - Placement
  G4Box *solidWorld = new G4Box("solidWorld", 0.5 * xWorld, 0.5 * yWorld, 0.5 * zWorld);
  G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
  G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);
    
    
  //////////////////Envelope/////////////////////
  
  //Dimensions and Material Constants
  double inch = 2.54 * cm;
  G4double env_sizeXY = 50 * cm, env_sizeZ = 50 * cm;
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



  //////////////////Xenon Cylinder/////////////////////

  // Cylinder dimensions
  G4double rIn_LXe  = 0.0*cm;          // solid fill (no hole)
  G4double rOut_LXe = 1.5*cm;          // 3 cm inner diameter -> radius 1.5 cm
  G4double h_LXe    = 1.5875*cm;       // height
  G4double hz_LXe   = 0.5*h_LXe;

  // LXe Material (80% Xe, 20% H)  ---- by MASS FRACTION
  G4Element* elXe = new G4Element("Xenon",    "Xe", 54., 131.293*g/mole);
  G4Element* elH  = new G4Element("Hydrogen", "H",   1.,   1.008*g/mole);

  // Pure liquid xenon is ~2.9 g/cm3 near boiling; mixture will differ.
  G4double density_LXeMix = 2.9*g/cm3;

  G4Material* LXe = new G4Material("LXe", density_LXeMix, 2);
  LXe->AddElement(elXe, 0.80);   // 80% by mass
  LXe->AddElement(elH,  0.20);   // 20% by mass

  // Solid - Logic - Placement
  G4Tubs* solidLXe = new G4Tubs("solidLXe", rIn_LXe, rOut_LXe, hz_LXe, 0.*deg, 360.*deg);
  logicLXe = new G4LogicalVolume(solidLXe, LXe, "logicLXe");

  // Place inside your environment volume 
  G4VPhysicalVolume* physLXe = new G4PVPlacement(nullptr, G4ThreeVector(0,0,0), logicLXe, "LXe", logicEnv, false, 0, checkOverlaps);
  


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
  logicLXe->SetVisAttributes(Yellow);

  return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
    SensitiveDetector *sensDet = new SensitiveDetector("SensitiveDetector");
    logicLXe->SetSensitiveDetector(sensDet); //Black
    
    G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
}
    