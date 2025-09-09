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
    G4Material* H2O = new G4Material("H2O", 1.0 * g / cm3, ncomponents = 2);
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


    //Air
    G4Material* Air = new G4Material("Air", 1.7903 * mg / cm3, ncomponents = 2);
    Air->AddElement(N, 70. * perCent);
    Air->AddElement(O, 30. * perCent);
    

    G4double watervapourDensity = (10.38 * 1e-6); // Convert mg/m3 to g/cm³

    G4double massFractionWater = 0.015;

    // Define Humid Air
    G4Material* HumidAir = new G4Material("HumidAir", 0.001204 * g / cm3, 3, kStateGas, 293 * kelvin, 1 * atmosphere);
    HumidAir->AddElement(N, 0.688);  // 70% N₂, adjusted down
    HumidAir->AddElement(O, 0.305);  // 30% O₂, adjusted down
    HumidAir->AddMaterial(H2O, 0.007);  // ~0.7% H₂O by mass

    //G4Material* HumidAir = new G4Material("HumidAir", (0.0017903 + watervapourDensity) * g / cm3, 3);
    //HumidAir->AddElement(N, 0.78 * (1.0 - massFractionWater));  // 78% Nitrogen
    //HumidAir->AddElement(O, 0.21 * (1.0 - massFractionWater));  // 21% Oxygen
    //HumidAir->AddMaterial(H2O, massFractionWater);              // Water vapor 


    //Set World Material
    worldMaterial = HumidAir;

    G4Material* Polyethylene = new G4Material("Polyethylene", .94 * g / cm3, 2);
    Polyethylene->AddElement(C, natoms = 2);
    Polyethylene->AddElement(Hnew, natoms = 4);


    G4Material* ChamberAluminum = new G4Material("ChamberAluminum", 2.7 * g / cm3, 1);
    ChamberAluminum->AddElement(Al, natoms = 1);



    G4Material* acrylicLid = new G4Material("Acrylic", 2 * g / cm3, 3);
    acrylicLid->AddElement(C, natoms = 5);
    acrylicLid->AddElement(Hnew, natoms = 8);
    acrylicLid->AddElement(O, natoms = 2);
    //Building world first
    G4Sphere* solidWorld =
        new G4Sphere("solidWorld", 0., 500 * cm, 0 * deg, 360 * deg, 0 * deg, 180 * deg); // 150 CM original
    G4LogicalVolume* logicWorld =
        new G4LogicalVolume(solidWorld, worldMaterial, "World");
    G4VPhysicalVolume* physWorld =
        new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
    G4cout << "BDS World Geometry Initialized";


    //Dimensions for geometry building

    //Measurements for Detectors
    G4double InnerRadius = 6.38 * mm;
    G4double InnerLength = 50.8 * mm;
    G4double OuterThickness = 1.52 * mm;
    G4double OuterRadius = InnerRadius + OuterThickness;
    G4double OuterLength = InnerLength + 2 * OuterThickness;

    //Measurements for Chamber
    G4double cInnerRadius = 16.19 * cm;
    G4double cOuterRadius = 16.44 * cm;
    G4double cHeight = 29.21 * cm;
    G4double lidHeight = 2.43 * cm;
    G4double lidWidth = cOuterRadius + 1 * cm; //Need to take this measurement


    //Pb castle measurements (not to scale, please edit)
    G4double castleThickness = 10.4 * cm;
    G4double innerCastleHeight = 23.2 * cm;
    G4double innerCastleBase = 25.25 * cm;
    //G4double innercastleSize = 18 * cm;
    G4double innerCastleWidth = 20.85 * cm;
    G4double outerCastleHeight = innerCastleHeight + castleThickness;
    G4double outerCastleBase = innerCastleBase + castleThickness;

    //G4double outerCastleSize = innerCastleSize + 2 * castleThickness; // Size of the outer volume
    G4double innerPbBasetoChamber = innerCastleHeight - (cHeight / 2) - cOuterRadius + cInnerRadius; //innerCastleSIze


    //Rotations and Positions
    G4RotationMatrix* NoRot = new G4RotationMatrix(0 * deg, 0 * deg, 0 * deg);
    G4ThreeVector NoTrans(0, 0, 0);
    G4ThreeVector InnerTrans(0, 0, -InnerLength / 2);
    G4ThreeVector OuterTrans(0, 0, -OuterLength / 2);
    G4ThreeVector ChamberToBase(0, 0, -cHeight / 2);
    G4ThreeVector ChamberToTop(0, 0, cHeight / 2 + lidHeight / 2 + 0.1 * mm);//we dont want overlap for lid and chamber since we wont g4union them
    G4ThreeVector PbCastle(0, 0, innerPbBasetoChamber); // Position the Lead castle up near the Chamber Base




    // Inside of BDS
    G4Tubs*
        sInnerTub = new G4Tubs("Inner Cylinder",                                //name
            0., InnerRadius, 0.5 * InnerLength, 0., twopi);    //dimensions
    G4Sphere*
        sInnerSphere = new G4Sphere("Inner Sphere", 0., InnerRadius, 0 * deg,
            360 * deg, 0 * deg, 180 * deg);
    G4VSolid*
        sInnerBDS = new G4UnionSolid("InnerBDS Solid", sInnerTub, sInnerSphere, NoRot, InnerTrans);

    //G4LogicalVolume*
    logicInner = new G4LogicalVolume(sInnerBDS,     //shape
        NewGel,                 //material
        "Inner BDS Logic");     //name

    for (G4int i = 0; i < 2; i++) //Placing 144 Detector inners, we place them seperate from the outer since they are sensitive detectors
    {
        for (G4int j = 0; j < 6; j++)
        {
            for (G4int k = 0; k < 12; k++)
            {
                new G4PVPlacement(0,                        //no rotation
                    G4ThreeVector(-5.72 * cm + (2.29 * j) * cm, -11.57 * cm + (2.29 * k) * cm, -InnerLength / 2 - 7.5 * cm + (InnerLength / 2 + 6 * cm) * i),//Centered at (0,0,0)
                    logicInner,                        //logical volume
                    "Inner BDS Logic",      //name
                    logicWorld,                      //mother  volume
                    false,                          //no boolean operation
                    i + j * 100 + k * 1000,                 //copy number, returns a unique number every iteration
                    true);
            }
        }
    }
   


    //// Create a small sphere to mark the source position
    //auto markerShape = new G4Sphere("SourceMarker", 0, 1 * cm, 0, 360 * deg, 0, 180 * deg);
    //auto markerLogic = new G4LogicalVolume(markerShape, Air, "MarkerLV");

    //// Place it at the same position as your GPS source (example: 0 0 90 cm)
    //new G4PVPlacement(0, G4ThreeVector(0, 0, 90 * cm),
    //    markerLogic, "Marker", logicWorld, false, 0);


    //new G4PVPlacement(0,                        //no rotation, j= 2, k=5, i=1
    //    G4ThreeVector(-5.72 * cm + (2.29 * 2) * cm, -11.57 * cm + (2.29 * 5) * cm, -InnerLength / 2 - 7.5 * cm + (InnerLength / 2 + 6 * cm) * 1),//Centered at (0,0,0)
    //    logicInner,                        //logical volume
    //    "Inner BDS Logic",      //name
    //    logicWorld,                      //mother  volume
    //    false,                          //no boolean operation
    //    1 + 2 * 100 + 5 * 1000,                 //copy number, returns a unique number every iteration
    //    true);


    // Outside of BDS
    G4Tubs*
        sOuterTub = new G4Tubs("Outer Cylinder",                            //name
            0., OuterRadius, 0.5 * OuterLength, 0., twopi);  //dimensions
    G4Sphere*
        sOuterSphere = new G4Sphere("Outer Sphere", 0., OuterRadius, 0 * deg,
            360 * deg, 0 * deg, 180 * deg);
    G4VSolid*
        NoCavBDS = new G4UnionSolid("No Cavity BDS", sOuterTub, sOuterSphere, NoRot, OuterTrans);

    G4VSolid*
        sOuterBDS = new G4SubtractionSolid("OuterBDS", NoCavBDS, sInnerBDS, NoRot, NoTrans);

    G4LogicalVolume*
        logicOuter = new G4LogicalVolume(sOuterBDS,            //shape
            Polyethylene,               //material
            "OuterBDSLogic");   //name

    for (G4int i = 0; i < 2; i++) //Placing 144 Detector outers
    {
        for (G4int j = 0; j < 6; j++)
        {
            for (G4int k = 0; k < 12; k++)
            {
                new G4PVPlacement(0,                        //no rotation
                    G4ThreeVector(-5.72 * cm + (2.29 * j) * cm, -11.57 * cm + (2.29 * k) * cm, -InnerLength / 2 - 7.5 * cm + (InnerLength / 2 + 6 * cm) * i),//Centered at (0,0,0)
                    logicOuter,                        //logical volume
                    "Outer BDS Logic",      //name
                    logicWorld,                      //mother  volume
                    false,                          //no boolean operation
                    i + j * 100 + k * 1000,                 //copy number, returns a unique number every iteration
                    true);
            }
        }
    }
    G4cout << "BDS Inner and Outer Layers Constructed";

    //Chamber Construction  
    G4Tubs*
        solidChamberWalls = new G4Tubs("Solid Chamber Walls", //name
            cInnerRadius, cOuterRadius, cHeight / 2, 0., twopi); //dimensions  
    G4Tubs*
        solidChamberBase = new G4Tubs("Solid Chamber Base",
            0., cOuterRadius, cOuterRadius - cInnerRadius, 0., twopi);
    G4VSolid*
        solidChamber = new G4UnionSolid("Solid Chamber", //walls + base = aluminum part of chamber 
            solidChamberWalls, solidChamberBase, NoRot, ChamberToBase);

    G4LogicalVolume*
        logicChamber = new G4LogicalVolume(solidChamber,            //shape
            ChamberAluminum,               //material
            "Logic Chamber");   //name

    new G4PVPlacement(0,                        //Placing Chamber around detectors
        NoTrans,                //at (0,0,0)
        logicChamber,                        //logical volume
        "Logic Chamber",      //name
        logicWorld,                      //mother  volume
        false,                          //no boolean operation
        0,                 //no copies
        true);

    //Chamber Lid Construction
    G4Tubs*
        solidChamberLid = new G4Tubs("Solid Chamber Lid", //name
            0., lidWidth, lidHeight / 2, 0., twopi); //dimensions 

    G4LogicalVolume*
        logicChamberLid = new G4LogicalVolume(solidChamberLid,            //shape
            acrylicLid,               //material
            "Logic Chamber Lid");   //name

    new G4PVPlacement(0,                        //Placing Chamber around detectors
        ChamberToTop,                //Placing lid on chamber
        logicChamberLid,                        //logical volume
        "Logic Chamber Lid",      //name
        logicWorld,                      //mother  volume
        false,                          //no boolean operation
        0,                 //no copies
        true);


    //Pb castle construction
    // Define the inner and outer boxes
    G4Box* innerLeadCastleBox = new G4Box("Inner Lead Castle Box", innerCastleBase, innerCastleBase, innerCastleHeight);
    G4Box* outerLeadCastleBox = new G4Box("Outer Lead Castle Box", outerCastleBase, outerCastleBase, outerCastleHeight);


    //Subtract the inner volume from the outer volume, to create a castle with thickness with the differenc of their size
    G4SubtractionSolid* LeadCastleSolid = new G4SubtractionSolid("Lead Castle Solid", outerLeadCastleBox, innerLeadCastleBox);


    // Define Pb logical volumes
    G4LogicalVolume* LeadCastle = new G4LogicalVolume(LeadCastleSolid, lead, "Pb Castle");


    // Position the Pb castle
 /* new G4PVPlacement(0,                         // No rotation
        PbCastle,  // Position at origin
        LeadCastle,           // Outer logical volume
        "Pb castle",       // Name
        logicWorld,              // Mother volume
        false,                   // No boolean operation
        0,                       // No copies
        true);
 */  
 G4cout << "BDS Chamber System Constructed";

   

    //Setting sensitive detectors as the logical volumes of the inner BDS
    SensitiveDetector* sensDet =
        new SensitiveDetector("SensitiveDetector");
    //Set the sensitive detector logicWorld, logicInner
    logicInner->SetSensitiveDetector(sensDet);
    G4cout << " Sensitive Detector Set";

    return physWorld;
}

