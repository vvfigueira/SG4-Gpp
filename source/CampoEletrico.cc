#include "math.h"

#include "CampoEletrico.hh"
#include "Constantes.hh"

#include "G4SystemOfUnits.hh"
#include "globals.hh"

using namespace CLHEP;

// Definindo a Classe Campo como Campo Elétrico

Campo::Campo(G4double intcamp)
    /*: Intensidade1(223.184*volt), Intensidade2(465.38*volt), Intensidade3(243.976*volt)*/{

}

Campo::~Campo(){
    
}

G4bool Campo::DoesFieldChangeEnergy() const{
    return true;
}

// Definição da Classe Campo

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