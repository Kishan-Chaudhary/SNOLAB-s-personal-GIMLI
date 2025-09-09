#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(SnolabBDS)

#Setup UI and Vis
find_package(Geant4 REQUIRED ui_all vis_all)

#Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(slbds slbds.cc ${sources} ${headers})
target_link_libraries(slbds ${Geant4_LIBRARIES} )

add_custom_target(SnolabBDS DEPENDS slbds)
#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build NeutronSource. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(slbds_SCRIPTS
    U-235.mac
    U-238.mac
    Th-232.mac
    FluxNeutrons.mac
    Spectrum.dat
  )
#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS slbds DESTINATION bin)

# Install macro files to the build directory
foreach(script ${slbds_SCRIPTS})
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${script}
            DESTINATION ${CMAKE_BINARY_DIR})
endforeach()