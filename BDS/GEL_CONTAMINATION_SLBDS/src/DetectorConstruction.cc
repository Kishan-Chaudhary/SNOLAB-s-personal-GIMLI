#include "DetectorConstruction.hh"
#include "G4NistManager.hh"
DetectorConstruction::DetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    G4int ncomponents, natoms;

    // Isotopes
    G4Isotope* H11 = new G4Isotope("Protium", 1, 1, 1.007825 * g / mole);
    G4Isotope* H22 = new G4Isotope("Deuterium", 1, 2, 2.014 * g / mole);
    G4Isotope* IU238 = new G4Isotope("IU238", 92, 238, 238.05 * g / mole);
    G4Isotope* IU235 = new G4Isotope("IU235", 92, 235, 235.04 * g / mole);


    // New elements from isotopes 
    G4Element* Hnew = new G4Element("Hydrogen", "Hnew", 2);
    Hnew->AddIsotope(H11, 99.985 * perCent);
    Hnew->AddIsotope(H22, 0.015 * perCent);

    G4Element* U238 = new G4Element("Uranium238", "U238", 1);
    U238->AddIsotope(IU238, 100 * perCent);

    G4Element* U235 = new G4Element("Uranium235", "U235", 1);
    U235->AddIsotope(IU235, 100 * perCent);


    //G4Element* H = new G4Element("Hydrogen", "H", 1., 1.01 * g / mole); (Using Hnew)
    G4Element* N = new G4Element("Nitrogen", "N", 7., 14.01 * g / mole);
    G4Element* O = new G4Element("Oxygen", "O", 8., 16.00 * g / mole);
    G4Element* C = new G4Element("Carbon", "C", 6, 12.01 * g / mole);
    G4Element* Al = new G4Element("Aluminum", "Al", 13, 26.98 * g / mole);
    G4Element* Na = new G4Element("Sodium", "Na", 11, 22.99 * g / mole);
    G4Element* Cl = new G4Element("Chlorine", "Cl", 17, 35.45 * g / mole);
    G4Element* Cs = new G4Element("Cesium", "Cs", 55, 132.91 * g / mole);
    G4Element* Th = new G4Element("Thorium", "Th", 90., 232.04 * g / mole);
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* lead = nist->FindOrBuildMaterial("G4_Pb"); //Used nist for Pb. It include isotopes, create a new material for different isotope abundance if required

    // H2O
    G4Material* H2O = new G4Material("H2O", 3.05 * g / cm3, ncomponents = 2);
    H2O->AddElement(Hnew, natoms = 2);
    H2O->AddElement(O, natoms = 1);



    //Gel material
    G4Material* Gel = new G4Material("Gel", 1.57 * g / cm3, ncomponents = 7);
    Gel->AddElement(Cs, 38.7 * perCent);
    Gel->AddElement(Cl, 10.3 * perCent);
    Gel->AddElement(C, 1.8 * perCent);
    Gel->AddElement(Hnew, 5.5 * perCent);
    Gel->AddElement(O, 42.9 * perCent);
    Gel->AddElement(N, 0.6 * perCent);
    Gel->AddElement(Na, 0.2 * perCent);



    //NewGel, includes U and Th contaminations 
    const double ppb = 1e-9; // Define ppb as 10^-9
    G4Material* NewGel = new G4Material("NewGel", 1.57 * g / cm3, ncomponents = 4);
    NewGel->AddMaterial(Gel, 0.99999998804); // 0.99999998804 
    NewGel->AddElement(U238, 5.99 * ppb); //5.99 * ppb
    NewGel->AddElement(U235, 3.00 * ppb); //3.00 * ppb
    NewGel->AddElement(Th, 1.97 * ppb); //1.97 * ppb


    //Set the NewGel for the world material
    worldMaterial = NewGel;

    
    //Building world first
    G4Sphere* solidWorld =
        new G4Sphere("solidWorld", 0., 1.19 * cm, 0 * deg, 360 * deg, 0 * deg, 180 * deg); // Original Sphere radius 150 CM original. The Radius 1.19  cm is for the volume of gel in the BDS according to the InnerBDS volume
    G4LogicalVolume* logicWorld =
        new G4LogicalVolume(solidWorld, worldMaterial, "World");
    G4VPhysicalVolume* physWorld =
        new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
    G4cout << "BDS World Geometry Initialized";




    //Setting sensitive detectors as the logical volumes of the inner BDS
    SensitiveDetector* sensDet =
        new SensitiveDetector("SensitiveDetector");
    //Set the NewGel world Sphere as the sensitive detector
    logicWorld->SetSensitiveDetector(sensDet);
    G4cout << " Sensitive Detector Set";

    return physWorld;
}

