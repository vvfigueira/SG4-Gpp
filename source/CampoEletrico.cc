// Includes do C++

#include "math.h"

// Includes da Simulação

#include "CampoEletrico.hh"
#include "Constantes.hh"

// Includes do Geant4

#include "G4SystemOfUnits.hh"
#include "globals.hh"

using namespace CLHEP;

// Construtor

Campo::Campo(){}

// Aniquilador

Campo::~Campo(){}

// Definido como 'true', campos elétricos

G4bool Campo::DoesFieldChangeEnergy() const{
    return true;
}

// Definição dos valores do campo

void Campo::GetFieldValue(const G4double Point[4], G4double field[6]) const{

    field[0]=0.*tesla;
    field[1]=0.*tesla;
    field[2]=0.*tesla;
    field[5]=0.*volt/mm;

    if(sqrt( Point[0]*Point[0] + Point[1]*Point[1]) <= (Dim::raioextal + 1*mm) && sqrt( Point[0]*Point[0] + Point[1]*Point[1]  ) > Dim::distcatext ){  
        field[3]=Dim::Intensidade1*(Point[0]/(Point[0]*Point[0]+Point[1]*Point[1]));
        field[4]=Dim::Intensidade1*(Point[1]/(Point[0]*Point[0]+Point[1]*Point[1]));
    }else if (sqrt( Point[0]*Point[0] + Point[1]*Point[1]  ) <= Dim::distcatext && sqrt( Point[0]*Point[0] + Point[1]*Point[1]  ) >= Dim::distcatint){
        field[3]=Dim::Intensidade2*(Point[0]/(Point[0]*Point[0]+Point[1]*Point[1]));
        field[4]=Dim::Intensidade2*(Point[1]/(Point[0]*Point[0]+Point[1]*Point[1]));
    }else if (sqrt( Point[0]*Point[0] + Point[1]*Point[1]  ) <= Dim::distcatint){
        field[3]=Dim::Intensidade3*(Point[0]/(Point[0]*Point[0]+Point[1]*Point[1]));
        field[4]=Dim::Intensidade3*(Point[1]/(Point[0]*Point[0]+Point[1]*Point[1]));
    }else{
        field[3]=0.*volt/mm;
        field[4]=0.*volt/mm;
    };
}