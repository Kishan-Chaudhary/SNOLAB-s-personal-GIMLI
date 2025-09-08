#include "G4NistManager.hh"
#include "DetectorConstruction.hh"
#include "G4Tubs.hh"
#include "NCDGeometry.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"         // Include SD Manager
#include "G4Element.hh"
#include "G4Material.hh"


// Constructor
DetectorConstruction::DetectorConstruction()
    : fHe3TubeL(He3TubeL205cm),             // Length of the He3 Tube
    fHe3TubeRadius(He3NickelIR),          // Inside radius of the Nickel wall
    fHe3TubeThickness(He3NickelTh),       // Thickness of the Nickel wall
    fSteelCapThickness(steelCapFaceThickness), // Thickness of the Steel Cap Face
    fHe3VolumeMiddlePointOffset(He3VolumeMiddlePointOffset),  // Offset for He3 Volume's Middle Point
    fHe3AnodeDiameter(He3AnodeDiameter),
    fHe3AnodeProtrustion(He3AnodeProtrusion),
    lNickelTube(nullptr),
    lHe3CuTube(nullptr),
    lHe3GasTube(nullptr),
    lBackSteelCap(nullptr),
    lBackSteelCapFace(nullptr),
    lFrontSteelCap(nullptr),
    lFrontSteelCapFace(nullptr),
    lAnodeWire(nullptr),
   /* lNickelTube2(nullptr),
    lHe3CuTube2(nullptr),
    lHe3GasTube2(nullptr),
    lBackSteelCap2(nullptr),
    lFrontSteelCap2(nullptr),
    lAnodeWire2(nullptr),*/
    worldMaterial(nullptr)
{
   
}

// Destructor
DetectorConstruction::~DetectorConstruction() {
    // Destructor implementation
}

// Construct the detector's physical volume
G4VPhysicalVolume* DetectorConstruction::Construct() {
    // Define materials
    G4double atomicMass = 3.016 * g / mole;
    G4Isotope* he3 = new G4Isotope("he3", 2, 3, atomicMass);
    G4Element* He3 = new G4Element("He3", "He3", 1);
    He3->AddIsotope(he3, 100. * perCent);
    /*G4Element* He = new G4Element("Helium", "He", 2., 4.002602 * g / mole);*/
    G4Material* He3Gas = new G4Material("He3", 0.0001786 * g / cm3, 1,kStateGas, 293*kelvin,2.125*atmosphere); //original density 0.0001786 new G4Material("matXenon1",density1, ncomponents=1, // kStateGas,temperature1, pressure1);
    He3Gas->AddElement(He3, 1);



    G4Element* Ni = new G4Element("Nickel", "Ni", 28., 58.6934 * g / mole);
    G4Material* nickelMaterial = new G4Material("Nickel", 8.90 * g / cm3, 1);
    nickelMaterial->AddElement(Ni, 1);

    G4Element* Fe = new G4Element("Iron", "Fe", 26., 55.845 * g / mole);
    G4Element* Cr = new G4Element("Chromium", "Cr", 24., 52.00 * g / mole);
    G4Material* stainlessSteelMaterial = new G4Material("G4_STAINLESS_STEEL", 8.00 * g / cm3, 3);
    stainlessSteelMaterial->AddElement(Fe, 0.74);
    stainlessSteelMaterial->AddElement(Cr, 0.18);
    stainlessSteelMaterial->AddElement(Ni, 0.08);

    G4Element* N = new G4Element("Nitrogen", "N", 7., 14.01 * g / mole);
    G4Element* O = new G4Element("Oxygen", "O", 8., 16.00 * g / mole);
    G4double density = 1.29 * mg / cm3;
    G4int ncomponents = 2;
    G4Material* airMaterial = new G4Material("Air", density, ncomponents, kStateGas, 293.15 * kelvin, 1.5 * atmosphere);
    airMaterial->AddElement(N, 0.79);
    airMaterial->AddElement(O, 0.21);

    // New elements from isotopes 
    G4Isotope* H11 = new G4Isotope("Protium", 1, 1, 1.007825 * g / mole);
    G4Isotope* H22 = new G4Isotope("Deuterium", 1, 2, 2.014 * g / mole);
    G4Element* Hnew = new G4Element("Hydrogen", "Hnew", 2);
    Hnew->AddIsotope(H11, 99.985 * perCent);
    Hnew->AddIsotope(H22, 0.015 * perCent);

    // H2O
    G4Material* H2O = new G4Material("H2O", 1.0 * g / cm3, 2);
    H2O->AddElement(Hnew, 2);
    H2O->AddElement(O, 1);


    //HumidAir
	//G4double watervapourDensity = (10.38 * 1e-6); // Convert mg/m3 to g/cm³
 //   G4double massFractionWater = 0.015;
 //   G4Material* HumidAir = new G4Material("HumidAir", (0.0017903 + watervapourDensity) * g / cm3, 3);
 //   HumidAir->AddElement(N, 0.78 * (1.0 - massFractionWater));  // 78% Nitrogen
 //   HumidAir->AddElement(O, 0.21 * (1.0 - massFractionWater));  // 21% Oxygen
 //   HumidAir->AddMaterial(H2O, massFractionWater);              // Water vapor 

	//PolyEthylene
    G4Element* C = new G4Element("Carbon", "C", 6., 12.011 * g / mole);
    G4Element* H = new G4Element("Hydrogen", "H", 1., 1.008 * g / mole);
    G4Material* PolyEthylene = new G4Material("PolyEthylene", 0.930 * g / cm3, 2);
    PolyEthylene->AddElement(C, 2);
    PolyEthylene->AddElement(H, 4);

    //Set World Material
    worldMaterial = airMaterial;
    // Define world size and radius
    G4double outerRadiusOfDetector = fHe3TubeRadius + fHe3TubeThickness; //shifting the setup below
    //G4double worldRadius = 10 * outerRadiusOfDetector;   //world volume radius being 10 times the radius of the tube
    //G4double worldSize = fHe3TubeL;  // World length

    
  /*  G4Tubs* solidWorld = new G4Tubs("World", 0, worldRadius, worldSize / 2, 0., 360. * deg);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, airMaterial, "World");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0);*/

    // Create the world volume
	G4Box* solidWorld =
		new G4Box("solidWorld", 150 * cm, 150 * cm, 450 * cm); //height, widht,length for room


    //G4Sphere* solidWorld =
    //    new G4Sphere("solidWorld", 0., 250 * cm, 0 * deg, 360 * deg, 0 * deg, 180 * deg); G4LogicalVolume* 
    logicWorld =
        new G4LogicalVolume(solidWorld, worldMaterial, "World");
    G4VPhysicalVolume* physWorld =
        new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

    //Shifting the setup
    G4double shiftSide = 70 * cm;
    // He3 Gas Tube (inner tube) - First NCD
    G4Tubs* solidHe3Tube1 = new G4Tubs("He3Tube1", 0, fHe3TubeRadius, fHe3TubeL / 2., 0., 360. * deg);
    lHe3CuTube = new G4LogicalVolume(solidHe3Tube1, He3Gas, "He3CuTube");
    new G4PVPlacement(0, G4ThreeVector(outerRadiusOfDetector + shiftSide, -outerRadiusOfDetector - shiftSide, 0.), lHe3CuTube, "He3CuTube", logicWorld, false, 0); // 

    // Nickel Tube (outer tube) - First NCD
    G4Tubs* solidNickelTube1 = new G4Tubs("NickelTube1", fHe3TubeRadius, fHe3TubeRadius + fHe3TubeThickness, fHe3TubeL / 2., 0., 360. * deg);
    lNickelTube = new G4LogicalVolume(solidNickelTube1, nickelMaterial, "NickelTube1");
    new G4PVPlacement(0, G4ThreeVector(outerRadiusOfDetector + shiftSide, -outerRadiusOfDetector - shiftSide, 0.), lNickelTube, "NickelTube1", logicWorld, false, 0);

    // Steel Caps (front and back) - First NCD
    G4Tubs* solidFrontSteelCap1 = new G4Tubs("FrontSteelCap1", 0, fHe3TubeRadius + fHe3TubeThickness, fSteelCapThickness / 2., 0., 360. * deg);
    lFrontSteelCap = new G4LogicalVolume(solidFrontSteelCap1, stainlessSteelMaterial, "FrontSteelCap");
    new G4PVPlacement(0, G4ThreeVector(outerRadiusOfDetector + shiftSide, -outerRadiusOfDetector - shiftSide, -(fHe3TubeL / 2. + fSteelCapThickness / 2.)), lFrontSteelCap, "FrontSteelCap1", logicWorld, false, 0);

    G4Tubs* solidBackSteelCap1 = new G4Tubs("BackSteelCap1", 0, fHe3TubeRadius + fHe3TubeThickness, fSteelCapThickness / 2., 0., 360. * deg);
    lBackSteelCap = new G4LogicalVolume(solidBackSteelCap1, stainlessSteelMaterial, "BackSteelCap");
    new G4PVPlacement(0, G4ThreeVector(outerRadiusOfDetector + shiftSide, -outerRadiusOfDetector - shiftSide, fHe3TubeL / 2. + fSteelCapThickness / 2.), lBackSteelCap, "BackSteelCap1", logicWorld, false, 0);

    // Anode Wire - First NCD
    G4double anodeWireLength = fHe3TubeL + 2.0 * (fHe3AnodeProtrustion - fSteelCapThickness);
    G4Tubs* solidAnodeWire1 = new G4Tubs("AnodeWire1", 0, fHe3AnodeDiameter / 2., anodeWireLength / 2., 0., 360. * deg);
    lAnodeWire = new G4LogicalVolume(solidAnodeWire1, stainlessSteelMaterial, "AnodeWire");
    new G4PVPlacement(0, G4ThreeVector(outerRadiusOfDetector + shiftSide, -outerRadiusOfDetector - shiftSide, 0.), lAnodeWire, "AnodeWire1", logicWorld, false, 0);


    // He3 Gas Tube (inner tube) - Second NCD (Placed in opposite corner)
/*G4Tubs* solidHe3Tube2 = new G4Tubs("He3Tube2", 0, fHe3TubeRadius, fHe3TubeL / 2., 0., 360. * deg);
lHe3CuTube2 = new G4LogicalVolume(solidHe3Tube, He3Gas, "He3CuTube2");*/
// Placing the second NCD in the opposite corner, using lHe3CuTube from previous NCD
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector + shiftSide, -outerRadiusOfDetector - shiftSide, 0.), lHe3CuTube, "He3CuTube2", logicWorld, false, 0);

    // Nickel Tube (outer tube) - Second NCD
    /*G4Tubs* solidNickelTube2 = new G4Tubs("NickelTube2", fHe3TubeRadius, fHe3TubeRadius + fHe3TubeThickness, fHe3TubeL / 2., 0., 360. * deg);
    lNickelTube2 = new G4LogicalVolume(solidNickelTube1, nickelMaterial, "NickelTube2");*/
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector + shiftSide, -outerRadiusOfDetector - shiftSide, 0.), lNickelTube, "NickelTube2", logicWorld, false, 0);

    // Steel Caps (front and back) - Second NCD
 /*   G4Tubs* solidFrontSteelCap2 = new G4Tubs("FrontSteelCap2", 0, fHe3TubeRadius + fHe3TubeThickness, fSteelCapThickness / 2., 0., 360. * deg);
    lFrontSteelCap2 = new G4LogicalVolume(solidFrontSteelCap1, stainlessSteelMaterial, "FrontSteelCap");*/
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector + shiftSide, -outerRadiusOfDetector - shiftSide, -(fHe3TubeL / 2. + fSteelCapThickness / 2.)), lFrontSteelCap, "FrontSteelCap2", logicWorld, false, 0);

    /* G4Tubs* solidBackSteelCap2 = new G4Tubs("BackSteelCap2", 0, fHe3TubeRadius + fHe3TubeThickness, fSteelCapThickness / 2., 0., 360. * deg);
     lBackSteelCap2 = new G4LogicalVolume(solidBackSteelCap1, stainlessSteelMaterial, "BackSteelCap");*/
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector + shiftSide, -outerRadiusOfDetector - shiftSide, fHe3TubeL / 2. + fSteelCapThickness / 2.), lBackSteelCap, "BackSteelCap2", logicWorld, false, 0);

    // Anode Wire - Second NCD
    /*G4Tubs* solidAnodeWire2 = new G4Tubs("AnodeWire2", 0, fHe3AnodeDiameter / 2., anodeWireLength / 2., 0., 360. * deg);
    lAnodeWire2 = new G4LogicalVolume(solidAnodeWire1, stainlessSteelMaterial, "AnodeWire");*/
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector + shiftSide, -outerRadiusOfDetector - shiftSide, 0.), lAnodeWire, "AnodeWire2", logicWorld, false, 0);


    // He3 Gas Tube (inner tube)- Third NCD
    //G4Tubs* solidHe3Tube3 = new G4Tubs("He3Tube3", 0, fHe3TubeRadius, fHe3TubeL / 2., 0., 360. * deg);
    //lHe3CuTube3 = new G4LogicalVolume(solidHe3Tube3, He3Gas, "He3CuTube3");
    // Placing the third NCD in the opposite corner, using lHe3CuTube from previous NCD
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector + shiftSide, outerRadiusOfDetector - shiftSide, 0.), lHe3CuTube, "He3CuTube3", logicWorld, false, 0);

    // Nickel Tube (outer tube) - Third NCD
    /*G4Tubs* solidNickelTube3 = new G4Tubs("NickelTube3", fHe3TubeRadius, fHe3TubeRadius + fHe3TubeThickness, fHe3TubeL / 2., 0., 360. * deg);
    lNickelTube3 = new G4LogicalVolume(solidNickelTube1, nickelMaterial, "NickelTube2");*/
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector + shiftSide, outerRadiusOfDetector - shiftSide, 0.), lNickelTube, "NickelTube3", logicWorld, false, 0);

    // Steel Caps (front and back) - Third NCD
  /*  G4Tubs* solidFrontSteelCap3 = new G4Tubs("FrontSteelCap3", 0, fHe3TubeRadius + fHe3TubeThickness, fSteelCapThickness / 2., 0., 360. * deg);
    lFrontSteelCap3 = new G4LogicalVolume(solidFrontSteelCap1, stainlessSteelMaterial, "FrontSteelCap");*/
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector + shiftSide, outerRadiusOfDetector - shiftSide, -(fHe3TubeL / 2. + fSteelCapThickness / 2.)), lFrontSteelCap, "FrontSteelCap3", logicWorld, false, 0);

    /* G4Tubs* solidBackSteelCap3 = new G4Tubs("BackSteelCap3", 0, fHe3TubeRadius + fHe3TubeThickness, fSteelCapThickness / 2., 0., 360. * deg);
     lBackSteelCap3 = new G4LogicalVolume(solidBackSteelCap1, stainlessSteelMaterial, "BackSteelCap");*/
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector + shiftSide, outerRadiusOfDetector - shiftSide, fHe3TubeL / 2. + fSteelCapThickness / 2.), lBackSteelCap, "BackSteelCap3", logicWorld, false, 0);

    // Anode Wire - Third NCD
   /* G4Tubs* solidAnodeWire3 = new G4Tubs("AnodeWire3", 0, fHe3AnodeDiameter / 2., anodeWireLength / 2., 0., 360. * deg);
    lAnodeWire3 = new G4LogicalVolume(solidAnodeWire1, stainlessSteelMaterial, "AnodeWire");*/
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector + shiftSide, outerRadiusOfDetector - shiftSide, 0.), lAnodeWire, "AnodeWire3", logicWorld, false, 0);
    
    
    //// Visualization attributes for World Volume
    G4VisAttributes* worldVisAtt = new G4VisAttributes();
    worldVisAtt->SetVisibility(true);  // Set visibility to true/false
    logicWorld->SetVisAttributes(worldVisAtt);

    // Visualization attributes for NCD1
    G4VisAttributes* he3TubeVisAtt = new G4VisAttributes(G4Color::Gray());  // Set to gray
    he3TubeVisAtt->SetVisibility(true);
    he3TubeVisAtt->SetForceSolid(true);  // Set to solid
    lHe3CuTube->SetVisAttributes(he3TubeVisAtt);

    G4VisAttributes* nickelTubeVisAtt = new G4VisAttributes(G4Color::Gray());  // Set to gray
    nickelTubeVisAtt->SetVisibility(true);
    nickelTubeVisAtt->SetForceSolid(true);  // Set to solid
    lNickelTube->SetVisAttributes(nickelTubeVisAtt);

    G4VisAttributes* steelVisAtt = new G4VisAttributes(G4Color::Gray());
    steelVisAtt->SetVisibility(true);
    lFrontSteelCap->SetVisAttributes(steelVisAtt);
    lBackSteelCap->SetVisAttributes(steelVisAtt);

    G4VisAttributes* wireVisAtt = new G4VisAttributes(G4Color::Green());
    wireVisAtt->SetVisibility(true);
    lAnodeWire->SetVisAttributes(wireVisAtt);



    
   
    return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
    //Setting sensitive detectors as the logical volumes of the inner BDS
    SensitiveDetector* sensDet =
        new SensitiveDetector("SensitiveDetector");
    //Set the sensitive detector
    lHe3CuTube->SetSensitiveDetector(sensDet); //logicWorld, lHe3CuTube
    G4cout << " Sensitive Detector Set";

       // Print message to confirm
    G4cout << "Sensitive Detectors set for both He3Tube." << G4endl;

}
