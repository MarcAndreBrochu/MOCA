# Top-level CMakeList.txt ecrit par Marc-Andre Brochu
# Le 1er avril 2015 (joyeux mois de l'autisme!)
# Pour le projet MOCA, Cegep de l'Outaouais
# ====================================================
# DMAJ: 6 avril 2015
#
# Trois targets:
#  1. MOCA lib
#  2. moca_test (depends de MOCA lib)
#  3. Editor (depends de MOCA lib)
#
# Donc on veut generer 3 targets, et les deux derniers sont dependants de
# target numero 1. Il est donc possible de compiler uniquement la lib mais
# pas de compiler uniquement le test ou l'editor par exemple.

cmake_minimum_required(VERSION 3.1.0)
include(${CMAKE_SOURCE_DIR}/cmake/PrefixList.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/TermColor.cmake)

project(Projet_MOCA)

# Creer les directories pour mettre le output s'ils n'existent pas
# deja. De plus, specifier les output paths des executables et des
# librairies compiles
set(LIB_OUTPUT ${CMAKE_SOURCE_DIR}/lib)
set(BIN_OUTPUT ${CMAKE_SOURCE_DIR}/bin)
set(INC_OUTPUT ${CMAKE_SOURCE_DIR}/include)
file(MAKE_DIRECTORY ${LIB_OUTPUT})
file(MAKE_DIRECTORY ${BIN_OUTPUT})
file(MAKE_DIRECTORY ${INC_OUTPUT})

# Le target 'install' depends de 'all' par defaut, ce qui veut dire que
# si on fait 'make moca' puis 'make install' par exemple, tous les targets
# seront compiles au lieu de juste 'moca' comme on le veut. Il faut donc
# desactiver ce comportement:
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)

# Changer le path ou le linker va aller chercher les libs dynamiques
# sinon a l'execution ca plante parce que c'est impossible de localiser
# MOCA.
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH ${LIB_OUTPUT})
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES ${LIB_OUTPUT} isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH ${LIB_OUTPUT})
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Ajouter les CMakeLists.txt
add_subdirectory(MOCA)
add_subdirectory(test)
add_subdirectory(editor)

message(STATUS "${BoldGreen}Finished parsing top-level CMakeLists.txt${ColorReset}")
