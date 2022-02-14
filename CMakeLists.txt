
#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.9   FATAL_ERROR)
project(EXAMPLE)
if(NOT TARGET Garfield::Garfield)
  find_package(Garfield REQUIRED)
endif()

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 COMPONENTS ui_all vis_all)
else()
  find_package(Geant4)
endif()

if(NOT Geant4_FOUND)
  message("Geant4 is not found. Example cannot be built.")
  return()
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${CLHEP_INCLUDE_DIR}/CLHEP/Units/)


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/source/*.cc )
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh )

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g")
add_executable(build main.cc ${sources} ${headers})
target_link_libraries(build ${Geant4_LIBRARIES} Garfield::Garfield)
target_include_directories(build PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build EXAMPLE. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(EXAMPLEB1_SCRIPTS
  teste.mac
  vis.mac
  gamma.mac
  teste2.mac
  Macro.mac
  ar_70_co2_30_1000mbar.gas
  run.mac
  physics.mac
  mdt_elx_delta.txt
  )

foreach(_script ${EXAMPLEB1_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
add_custom_target(EXAMPLE DEPENDS build)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS build DESTINATION bin)
