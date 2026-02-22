#----------------------------------------------------------------------------
# Setup the project

cmake_minimum_required(VERSION 3.16...3.27)
project(SbBe)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# See the documentation for a guide on how to enable/disable specific components
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

# Project Directories 
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

file(GLOB MACRO_FILES "macros/*.mac")
file(COPY ${MACRO_FILES} DESTINATION ${CMAKE_BINARY_DIR})
#----------------------------------------------------------------------------
# Add the executable, use our local headers, and link it to the Geant4 libraries

add_executable(SbBesim SbBe.cc ${sources})
target_link_libraries(SbBesim ${Geant4_LIBRARIES})
add_custom_target(SbBe DEPENDS SbBesim)

