# SG4-Gpp
## Sobre
  Este projeto está sendo desenvolvido como tentativa de melhorar a qualidade das simulações iniciadas em [SG4](https://github.com/vvfigueira/SG4-Gpp), agora além de utilizar as bibliotecas [Geant4](https://geant4.web.cern.ch/), também foram implementadas as bibliotecas [Garfield++](https://garfieldpp.web.cern.ch/garfieldpp/), as quais são especializadas para simulação de detectores gasosos.
## Requisitos
  Uma instalação de Garfield++, Geant4 e todas suas dependências é suficiente para a compilação do projeto
## Compilação
  Caso possua uma instalação das bibliotecas CLHEP fora do diretório de instalação do Geant4, é necessário definir uma variável de ambiente `CLHEP_INCLUDE_DIR=[CLHEP_install_dir]/include/`, e também exportar as bibliotecas `export LD_LIBRARY_PATH=[CLHEP_install_dir]/lib/:${LD_LIBRARY_PATH}`.
  A compilação segue o método padrão do `cmake`, basta executar dentro da pasta desejada para a construção do projeto `cmake [path_to_CMakeLists.txt]` seguido de `make`, isto irá criar o arquivo binário `build` e copiar os arquivos de script `run.mac` e `vis.mac`.
## Uso
  A aplicação possui dois modos de uso, com janela gráfica, e somente batch. Para a simulação utilizar as bibliotecas Garfield++ é necessário rodar em modo batch, executando `./build -m run.mac`. Caso seja executado apenas `./build` o projeto será inicializado em modo de janela gráfica que utiliza apenas o Geant4 para a simulação.
## Características
  Atualmente o projeto está definido como um cilindro de alumínio completo de uma mistura de gás Argônio(70%) com CO2(30%), 1 atm de pressão e 300 K de temperatura, em seu interior, à distâncias de 96 mm e 20 mm do centro do detector estâo posicionados respectivamente os cátodos externo e interno, sendo ambos compostos de 60 fios distribuídos uniformemente pela circunferência. No centro temos o ânodo de ouro.
