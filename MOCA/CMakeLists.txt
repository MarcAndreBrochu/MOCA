# MOCA CMakeList.txt ecrit par Marc-Andre Brochu
# Le 1er avril 2015 (joyeux mois de l'autisme!)
# Pour le projet MOCA, Cegep de l'Outaouais
# ====================================================
# DMAJ: 15 avril 2015

cmake_minimum_required(VERSION 3.1.0)

option(BUILD_SHARED_LIBS "Builds MOCA as a shared library (default ON)" ON)

find_package(Armadillo REQUIRED)
if(${ARMADILLO_FOUND})
    message(STATUS "${BoldGreen}Found library Armadillo${ColorReset}")
else()
    message(FATAL "${BoldRed}Cannot find library Armadillo${ColorReset}")
endif()

# Pour que cmake trouve les sources et les headers, il faut les
# specifier ici.
set(MOCA_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/)
set(MOCA_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/MOCA/)
set(MOCA_INC_INSTALL ${CMAKE_CURRENT_SOURCE_DIR}/include/)

include_directories(${MOCA_INC_DIR})
include_directories(${ARMADILLO_INCLUDE_DIRS})

# Lister toutes les sources qui vont etre compilees ici.
# Il serait possible d'utiliser la commande GLOB afin de ne
# pas avoir a le faire mais cette methode vient avec certains
# problemes qui ne sont pas souhaitables dans notre cas, tel
# des problemes avec git rebase.
set(SRCS
    AbstractBody.cpp
    Ball.cpp
    Box.cpp
    CollisionHandler.cpp
    Integrator.cpp
    Particle.cpp
    Solid.cpp
    World.cpp)

# Lister les headers uniquement pour que QtCreator les liste
set(HEADERS
    AbstractBody.h
    Ball.h
    Box.h
    CollisionHandler.h
    Integrator.h
    Particle.h
    Solid.h
    World.h
    MOCA.h)

# Ajouter le path en prefixe aux sources en utilisant une macro,
# comme ca on est pas oblige de le specifier dans la liste des
# sources ci-haut et ca evite la redondance
add_prefix_to_list(SRCS ${MOCA_SRC_DIR})
add_prefix_to_list(HEADERS ${MOCA_INC_DIR})
add_library(moca ${SRCS} ${HEADERS})

# Pour que les librairies et les executables qui linkent avec MOCA
# puissent trouver les headers lors de la compilation
target_include_directories(moca PUBLIC ${MOCA_INC_INSTALL})
target_link_libraries(moca ${ARMADILLO_LIBRARIES})

# Installe la librairie et les headers aux bons endroit
install(TARGETS moca DESTINATION ${LIB_OUTPUT})
install(DIRECTORY ${MOCA_INC_INSTALL} DESTINATION ${INC_OUTPUT} FILES_MATCHING PATTERN "*.h")
