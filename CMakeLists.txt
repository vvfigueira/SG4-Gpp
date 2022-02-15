
#----------------------------------------------------------------------------
# Definição do projeto
#

cmake_minimum_required(VERSION 3.9   FATAL_ERROR)
project(G4Gpp)
if(NOT TARGET Garfield::Garfield)
  find_package(Garfield REQUIRED)
endif()

#----------------------------------------------------------------------------
# Encontra a instalação do Geant4 e ativa todos seus drives gráficos
# Pode definir-se WITH_GEANT4_UIVIS como OFF para executar apenas em batch
#

option(WITH_GEANT4_UIVIS "Constrói o projeto com Geant4 UI e drives de visualização" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 COMPONENTS ui_all vis_all)
else()
  find_package(Geant4)
endif()

if(NOT Geant4_FOUND)
  message("Geant4 não encontrado. G4Gpp não pode ser compilado.")
  return()
endif()

#----------------------------------------------------------------------------
# Carrega bibliotecas do Geant4 e seus compiladores
# 

include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Inclusão das bibliotecas CLHEP, normalmente acompanham a instalação do G4
#

include_directories(${CLHEP_INCLUDE_DIR}/CLHEP/Units/)


#----------------------------------------------------------------------------
# Bibliotecas e códigos fonte do projeto
# 

file(GLOB sources ${PROJECT_SOURCE_DIR}/source/*.cc )
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh )

#----------------------------------------------------------------------------
# Constrói o executável 'build'
#

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g")
add_executable(build main.cc ${sources} ${headers})
target_link_libraries(build ${Geant4_LIBRARIES} Garfield::Garfield)
target_include_directories(build PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Copia arquivos importantes para o diretório de utilização de '~$ cmake ..'
# 

set(G4Gpp_scripts
  vis.mac
  ar_70_co2_30_1000mbar.gas
  run.mac
  )

foreach(_script ${G4Gpp_scripts})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Para uso interno do Geant4
# 

add_custom_target(G4Gpp DEPENDS build)

#----------------------------------------------------------------------------
# Instala o executável no 'bin' da definição de CMAKE_INSTALL_PREFIX
#

install(TARGETS build DESTINATION bin)
