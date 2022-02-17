#ifndef CampoEletrico_h
#define CampoEletrico_h 1

// Includes do Geant4

#include "G4ElectroMagneticField.hh"

// Criação da classe 'Campo' derivada da classe G4ElectroMagneticField do Geant4

class Campo : public G4ElectroMagneticField {

    public:
        
        // Construtor e aniquilador da classe
        
        Campo();
        ~Campo();

        // Método que obtém os valores do campo

        virtual void  GetFieldValue(const G4double Point[4], G4double *Bfield ) const override;

        // Deve ser 'true' para campos elétricos e 'false' para campos magnéticos

        virtual G4bool DoesFieldChangeEnergy() const override;

};
#endif