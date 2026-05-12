#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4GenericMessenger.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SDManager.hh"

#include <cmath>

DetectorConstruction::DetectorConstruction()
{
  DefineCommands();
}

DetectorConstruction::~DetectorConstruction()
{
  delete fMessenger;
}

void DetectorConstruction::DefineCommands()
{
  fMessenger = new G4GenericMessenger(this, "/SbBe/det/", "Detector control");

  auto& distanceCmd = fMessenger->DeclareMethodWithUnit(
      "setEJ309Distance", "cm", &DetectorConstruction::SetEJ309Distance,
      "Set the EJ309 center distance from the LXe target.");
  distanceCmd.SetParameterName("distance", false);
  distanceCmd.SetRange("distance > 0.");

  auto& angleCmd = fMessenger->DeclareMethodWithUnit(
      "setEJ309Angle", "deg", &DetectorConstruction::SetEJ309Angle,
      "Set the EJ309 polar angle relative to the +x beam axis.");
  angleCmd.SetParameterName("angle", false);
  angleCmd.SetRange("angle >= 0. && angle <= 180.");
}

void DetectorConstruction::SetEJ309Distance(G4double value)
{
  fScintDistance = value / cm;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetEJ309Angle(G4double value)
{
  fScintAngleDeg = value / deg;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
  G4bool checkOverlaps = true;

  G4NistManager *nist = G4NistManager::Instance();

  //////////////////World Construction/////////////////////

  G4double xWorld = 2.5 * m;
  G4double yWorld = 2.5 * m;
  G4double zWorld = 2.5 * m;

  G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");

  G4Box *solidWorld = new G4Box("solidWorld", 0.5 * xWorld, 0.5 * yWorld, 0.5 * zWorld);
  G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
  G4VPhysicalVolume *physWorld =
      new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);

  //////////////////Envelope/////////////////////

  double inch = 2.54 * cm;
  G4double env_sizeXY = 400 * cm, env_sizeZ = 400 * cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");

  auto solidEnv = new G4Box("Envelope", 0.5 * env_sizeXY, 0.5 * env_sizeXY, 0.5 * env_sizeZ);
  auto logicEnv = new G4LogicalVolume(solidEnv, env_mat, "Envelope");

  G4VisAttributes* EnvVisAtt = new G4VisAttributes(G4Colour(.25, .25, .25, .25));
  EnvVisAtt->SetForceSolid(true);
  logicEnv->SetVisAttributes(EnvVisAtt);

  new G4PVPlacement(nullptr, G4ThreeVector(), logicEnv, "Envelope", logicWorld, false, 0, checkOverlaps);

  //////////////////Xenon Cylinder/////////////////////

  G4double rIn_LXe  = 0.0 * cm;
  G4double rOut_LXe = 1.5 * cm;
  G4double h_LXe    = 1.5875 * cm;
  G4double hz_LXe   = 0.5 * h_LXe;

  G4Material* LXe = nist->FindOrBuildMaterial("G4_lXe");
  G4Material* H2Approx = nist->FindOrBuildMaterial("G4_H");
  G4Material* HeApprox = nist->FindOrBuildMaterial("G4_He");

  const G4double density = LXe->GetDensity();

  // Toggle the active dopant by commenting one block and uncommenting the other.
//   G4Material* activeDopant = HeApprox;
//   G4double activeDopantMassFraction = 9.79e-4;
//   G4String dopedMaterialName = "LXe_He";

  
  G4Material* activeDopant = H2Approx;
  G4double activeDopantMassFraction = 9.79e-4;
  G4String dopedMaterialName = "LXe_H2";
  

  G4Material* dopedLXe = new G4Material(dopedMaterialName, density, 2);
  dopedLXe->AddMaterial(LXe, 1. - activeDopantMassFraction);
  dopedLXe->AddMaterial(activeDopant, activeDopantMassFraction);

  G4Tubs* solidLXe = new G4Tubs("solidLXe", rIn_LXe, rOut_LXe, hz_LXe, 0. * deg, 360. * deg);
  logicLXe = new G4LogicalVolume(solidLXe, dopedLXe, "logicLXe");

  G4VPhysicalVolume* physLXe =
      new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), logicLXe, "LXe", logicEnv, false, 0, checkOverlaps);

  ///////////////////Lead shielding around source/////////////////////

  G4Material* leadMat = nist->FindOrBuildMaterial("G4_Pb");
  G4Material* airMat = nist->FindOrBuildMaterial("G4_AIR");

  const G4ThreeVector sourcePosition(-50.0 * cm, 0., 0.);
  const G4double leadBrickHalfX = 2.5 * cm;
  const G4double leadBrickHalfY = 5.0 * cm;
  const G4double leadBrickHalfZ = 4.0 * cm;
  const G4double sourceHoleRadius = 0.6 * cm;
  const G4double sourceHoleDepth = 3.0 * cm;
  const G4double leadCoverThickness = 1.0 * cm;
  const G4double leadCoverHalfY = 2.2 * cm;
  const G4double leadCoverHalfZ = 2.6 * cm;

  G4Box* solidLeadBrick = new G4Box("solidLeadBrick",
                                    leadBrickHalfX,
                                    leadBrickHalfY,
                                    leadBrickHalfZ);

  auto sourceHoleRot = new G4RotationMatrix();
  sourceHoleRot->rotateY(90. * deg);

  G4Tubs* solidSourceHole = new G4Tubs("solidSourceHole",
                                       0.,
                                       sourceHoleRadius,
                                       0.5 * sourceHoleDepth,
                                       0. * deg,
                                       360. * deg);

  // The source remains at sourcePosition. The hole enters from the +x face
  // and reaches slightly past the source; a separate lead cover closes it.
  G4SubtractionSolid* solidLeadBrickWithHole =
      new G4SubtractionSolid("solidLeadBrickWithHole",
                             solidLeadBrick,
                             solidSourceHole,
                             sourceHoleRot,
                             G4ThreeVector(leadBrickHalfX - 0.5 * sourceHoleDepth, 0., 0.));

  logicLead = new G4LogicalVolume(solidLeadBrickWithHole, leadMat, "logicLeadBrickWithHole");

  new G4PVPlacement(nullptr,
                    sourcePosition,
                    logicLead, "LeadBrickWithSourceHole", logicEnv, false, 0, checkOverlaps);

  G4Tubs* solidSourceCavity = new G4Tubs("solidSourceCavity",
                                         0.,
                                         sourceHoleRadius,
                                         0.5 * sourceHoleDepth,
                                         0. * deg,
                                         360. * deg);
  G4LogicalVolume* logicSourceCavity =
      new G4LogicalVolume(solidSourceCavity, airMat, "logicSourceCavity");

  new G4PVPlacement(sourceHoleRot,
                    sourcePosition + G4ThreeVector(leadBrickHalfX - 0.5 * sourceHoleDepth, 0., 0.),
                    logicSourceCavity, "SourceAirHole", logicEnv, false, 0, checkOverlaps);

  G4Box* solidLeadCover = new G4Box("solidLeadCover",
                                    0.5 * leadCoverThickness,
                                    leadCoverHalfY,
                                    leadCoverHalfZ);
  G4LogicalVolume* logicLeadCover = new G4LogicalVolume(solidLeadCover, leadMat, "logicLeadCover");

  new G4PVPlacement(nullptr,
                    sourcePosition + G4ThreeVector(leadBrickHalfX + 0.5 * leadCoverThickness, 0., 0.),
                    logicLeadCover, "LeadSourceCover", logicEnv, false, 1, checkOverlaps);

  ///////////////////EJ309 cylinder/////////////////////

  G4double rIn_Reflect  = 0.0 * inch;
  G4double rOut_Reflect = 3.0 * inch;
  G4double h_Reflect    = 3.0 * inch;
  G4double hz_Reflect   = 0.5 * h_Reflect;

  G4Material* EJ309 = new G4Material("EJ309", 0.964 * g / cm3, 2);

  G4Element* elH = nist->FindOrBuildElement("H");
  G4Element* elC = nist->FindOrBuildElement("C");

  EJ309->AddElement(elC, 0.906);
  EJ309->AddElement(elH, 0.094);

  // Match the slide geometry: the EJ309 is placed downward from the beam axis (+x).
  const G4double scintDistance = fScintDistance * cm;
  const G4double scintAngleFromBeam = fScintAngleDeg * deg;
  const G4ThreeVector scintPosition(scintDistance * std::cos(scintAngleFromBeam),
                                    0.,
                                    -scintDistance * std::sin(scintAngleFromBeam));

  auto rot = new G4RotationMatrix();
  rot->rotateY(90. * deg + scintAngleFromBeam);

  G4Tubs* solidReflect = new G4Tubs("solidReflect", rIn_Reflect, rOut_Reflect, hz_Reflect, 0. * deg, 360. * deg);
  logicReflect = new G4LogicalVolume(solidReflect, EJ309, "logicReflect");
  G4VPhysicalVolume* physReflect =
      new G4PVPlacement(rot, scintPosition, logicReflect, "EJ309", logicEnv, false, 0, checkOverlaps);

  ///////////////////Colors/////////////////////

  G4VisAttributes* Yellow    = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.5));
  G4VisAttributes* SlateBlue = new G4VisAttributes(G4Colour(0.42, 0.35, 0.8, 0.75));
  G4VisAttributes* LeadGray  = new G4VisAttributes(G4Colour(0.35, 0.35, 0.35, 0.8));

  Yellow->SetForceSolid(true);
  SlateBlue->SetForceSolid(true);
  LeadGray->SetForceSolid(true);

  logicLXe->SetVisAttributes(Yellow);
  logicReflect->SetVisAttributes(SlateBlue);
  logicLead->SetVisAttributes(LeadGray);
  logicLeadCover->SetVisAttributes(LeadGray);

  return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
    auto* sdManager = G4SDManager::GetSDMpointer();
    auto* sensDet =
        static_cast<SensitiveDetector*>(sdManager->FindSensitiveDetector("SensitiveDetector", false));

    if (!sensDet) {
        sensDet = new SensitiveDetector("SensitiveDetector");
        sdManager->AddNewDetector(sensDet);
    }

    logicLXe->SetSensitiveDetector(sensDet);
    logicReflect->SetSensitiveDetector(sensDet);
}
