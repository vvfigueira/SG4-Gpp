// Includes do C++

#include <math.h>

// Includes da Simulação

#include "Detector.hh"
#include "CampoEletrico.hh"
#include "DetectorSensivel.hh"
#include "DetectorMens.hh"
#include "Constantes.hh"
#include "GarfieldModelo.hh"

// Includes de Geometria

#include "G4NistManager.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4MultiUnion.hh"
#include "G4VisAttributes.hh"
#include "G4Region.hh"

// Include de Unidades

#include "CLHEP/Units/PhysicalConstants.h"

// Includes do Campo

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4ElectroMagneticField.hh"
#include "G4UniformMagField.hh"
#include "G4UniformElectricField.hh"
#include "G4EqMagElectricField.hh"
#include "G4IntegrationDriver.hh"
#include "G4DormandPrince745.hh"
#include "G4ChordFinder.hh"

using namespace CLHEP;

// Construtor inicializando variáveis

Detector::Detector()
    : pressaogas(Dim::pressaogas),
    temperaturagas(Dim::temperaturagas){
        detectormens = new DetectorMens(this);
}

// Aniquilador deletando variáveis criadas pelo construtor

Detector::~Detector(){delete detectormens;}

// Construção do Detector

G4VPhysicalVolume* Detector::Construct()
{   
    DefineMaterials();

    return DefineVolumes();
}

// Construção de Materiais

void Detector::DefineMaterials() {
    
    auto NistManager = G4NistManager::Instance();

    // Adição de materiais prontos

    NistManager->FindOrBuildMaterial("G4_Al");
    NistManager->FindOrBuildMaterial("G4_Au");
    NistManager->FindOrBuildMaterial("G4_Galactic");

    auto Ni = NistManager->FindOrBuildMaterial("G4_Ni");
    auto Cr = NistManager->FindOrBuildMaterial("G4_Cr");

    G4Element* C = NistManager->FindOrBuildElement("C", false);
    G4Element* H = NistManager->FindOrBuildElement("H", false);
    G4Element* O = NistManager->FindOrBuildElement("O", false);
    G4Element* Ne = NistManager->FindOrBuildElement("Ne", false);
    G4Element* Ar = NistManager->FindOrBuildElement("Ar", false);

    // Definição do gás 'CO2'

    DensidadeCO2 = 0.610*mg/cm3;

    G4Material* CO2 = new G4Material("CO2", DensidadeCO2, 2, kStateGas, 
        Dim::temperaturagas, Dim::pressaogas);
    CO2->AddElement(C, 1);
    CO2->AddElement(O, 2);

    // Definição do gás 'CH4'

    DensidadeCH4 = 0.657*mg/cm3;

    G4Material* CH4 = new G4Material("CH4", DensidadeCH4,2,kStateGas, 
        Dim::temperaturagas, Dim::pressaogas);
    CH4->AddElement(C, 1);
    CH4->AddElement(H, 4);

    // Definição do nicromo

    auto Nicromo = new G4Material("Nicromo", Dim::densidadenicr, 2, kStateSolid, Dim::temperaturagas, Dim::pressaogas); 
    Nicromo->AddMaterial(Ni, 77.0*perCent);
    Nicromo->AddMaterial(Cr, 23.0*perCent);

    // Definição da mistura 'Ne+CO2'

    DensidadeGas = 0.816*mg/cm3;

    G4Material* NeCO2_90_10 = new G4Material("NeCO2_90_10", DensidadeGas, 2, 
        kStateGas, Dim::temperaturagas, Dim::pressaogas);
    NeCO2_90_10->AddElement(Ne, 0.9);
    NeCO2_90_10->AddMaterial(CO2, 0.1);

    // Definição da mistura 'Ar+CO2'

    DensidadeGas2 = 1.345*mg/cm3;

    G4Material* ArCO2_70_30 = new G4Material("ArCO2_70_30", DensidadeGas2, 2, 
        kStateGas, Dim::temperaturagas, Dim::pressaogas);
    ArCO2_70_30->AddElement(Ar, 0.7);
    ArCO2_70_30->AddMaterial(CO2,0.3);

    // Definição da mistura 'Ar+CH4'

    DensidadeGas3 = 1.560*mg/cm3;

    G4Material* ArCH4_90_10 = new G4Material("ArCH4_90_10", DensidadeGas3, 2, 
        kStateGas, Dim::temperaturagas, Dim::pressaogas);
    ArCH4_90_10->AddElement(Ar, 0.9);
    ArCH4_90_10->AddMaterial(CH4, 0.1);

    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

// Construção da Geometria

G4VPhysicalVolume* Detector::DefineVolumes(){

    // Definição do Mundo

    auto SMundo = new G4Box("SMundo", Dim::tamanhomundo, Dim::tamanhomundo, Dim::tamanhomundo);
    auto VLMundo = new G4LogicalVolume(SMundo, G4Material::GetMaterial("G4_Galactic"), "VLMundo");
    auto VFMundo = new G4PVPlacement(0, {0, 0, 0}, VLMundo, "Mundo", 0, false, 0);

    // Definição do Gás Ativo
    
    auto SGas = new G4Tubs("SGas", 0, 10.45*cm, Dim::comprimento, 0, twopi);
    auto VLGas = new G4LogicalVolume(SGas, G4Material::GetMaterial("ArCO2_70_30"), "VLGas");
    VFGas = new G4PVPlacement(0, {0, 0, 0}, VLGas, "Gás Ativo", VLMundo, false, 0);
    
    auto SFio = new G4Tubs("SFio", 0, Dim::raio, Dim::comprimento, 0, twopi);

    auto SCatExt = new G4MultiUnion("SCatExt");
    
    // Definição do Cátodo Externo

    for (G4int i = 0; i < Dim::n; i++) {
        G4RotationMatrix MNula = G4RotationMatrix(0, 0, 0);
        G4Tubs* SFio = new G4Tubs("SFio", 0, Dim::raio, Dim::comprimento, 0, twopi);
        G4ThreeVector Pos = G4ThreeVector(Dim::distcatext*cos(i*twopi/60.), Dim::distcatext*sin(i*twopi/60.), 0);
        G4Transform3D Tr = G4Transform3D(MNula, Pos);
        SCatExt->AddNode(*SFio, Tr);
    }

    SCatExt->Voxelize();

    auto VLCatExt = new G4LogicalVolume(SCatExt, G4Material::GetMaterial("Nicromo"), "catodoextLogico");
    VFCatExt = new G4PVPlacement(0, {0, 0, 0}, VLCatExt, "Cátodo Externo", VLGas, false, 0);

    // Definição do Cátodo Interno

    auto SCatInt = new G4MultiUnion("SCatInt");

    for (G4int i = 0; i < Dim::n; i++) {
        G4RotationMatrix MNula = G4RotationMatrix(0, 0, 0);
        G4Tubs* SFio = new G4Tubs("SFio", 0, Dim::raio, Dim::comprimento, 0, twopi);
        G4ThreeVector Pos = G4ThreeVector(Dim::distcatint*cos(i*twopi/60.), Dim::distcatint*sin(i*twopi/60.), 0);
        G4Transform3D Tr = G4Transform3D(MNula, Pos);
        SCatInt->AddNode(*SFio, Tr);
    }

    SCatInt->Voxelize();

    auto VLCatInt = new G4LogicalVolume(SCatInt, G4Material::GetMaterial("Nicromo"), "catodointLogico");
    VFCatInt = new G4PVPlacement(0, {0, 0, 0}, VLCatInt, "Cátodo Interno", VLGas, false, 0);

    // Definição do Cátodo Externo de Aluminio

    auto STubo = new G4Tubs("STubo", Dim::raiointal, Dim::raioextal, Dim::comprimento, 0, twopi); 
    auto VLTubo = new G4LogicalVolume(STubo, G4Material::GetMaterial("G4_Al"), "VLTubo");
    VFTubo = new G4PVPlacement(0, {0, 0, 0}, VLTubo, "Cátodo Alumínio", VLMundo, false, 0);

    // Definição do Ânodo de Ouro

    auto SAnodo = new G4Tubs("SAnodo", 0, Dim::raiofioouro, Dim::comprimento, 0, twopi);
    auto VLAnodo = new G4LogicalVolume(SAnodo, G4Material::GetMaterial("G4_Au"), "VLAnodo");
    VFAnodo = new G4PVPlacement(0, {0,0,0}, VLAnodo, "Ânodo de Ouro", VLGas, false, 0);

    // Atributos Visuais

    VLMundo->SetVisAttributes(G4VisAttributes::Invisible);

    G4VisAttributes* VisAttBlue = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
    G4VisAttributes* VisAttGreen = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));
    G4VisAttributes* VisAttRed = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
    G4VisAttributes* VisAttWhite = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));

    VLTubo->SetVisAttributes(VisAttRed);
    VLCatExt->SetVisAttributes(VisAttGreen);
    VLCatInt->SetVisAttributes(VisAttWhite);
    VLAnodo->SetVisAttributes(VisAttBlue);

    // Regiões de Validade do Modelo Garfieldpp
    
    if(Dim::uGpp == true ){
        G4Region* RGarfield = new G4Region("RGarfield");
        RGarfield->AddRootLogicalVolume(VLGas);

        G4Region* RAnodo = new G4Region("RAnodo");
        RAnodo->AddRootLogicalVolume(VLAnodo);

        auto gpp = new GarfieldModelo("GarfieldModelo", RGarfield);
        gpp->WriteGeometryToGDML(VFGas);
    }

    // Região de Validade do Detector Sensível

    auto DS = new DetectorSensivel("Ânodo de Ouro", GetFieldIntensity());
    VLAnodo->SetSensitiveDetector(DS);
    
    return VFMundo;
}

// Construtor do Campo

void Detector::ConstructSDandField(){

    auto campoEletrico = new Campo();
    auto campoManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    auto campoEq = new G4EqMagElectricField(campoEletrico);
    auto passo = new G4DormandPrince745(campoEq, Dim::variaveis);
    auto integrador = new G4IntegrationDriver<G4DormandPrince745>(Dim::passoMin, passo, Dim::variaveis);
    auto caminho = new G4ChordFinder(integrador);

    campoManager->SetDetectorField(campoEletrico);
    campoManager->SetChordFinder(caminho);
}