# Top-level CMakeList.txt ecrit par Marc-Andre Brochu
# Le 1er avril 2015 (joyeux mois de l'autisme!)
# Pour le projet MOCA, Cegep de l'Outaouais
# ====================================================
#
# Ce qu'on veut que cmake genere comme processus:
# 1. Creer une librairie physique a partir du code dans /MOCA
#    La librairie va etre placee dans /lib et les headers dans
#    /include/MOCA
# 2. SI VOULU, compiler les tests et linker avec la librairie
#    Les tests comprennent l'affichage OpenGL de debug. Il vont
#    etre places dans /bin/tests
# 3. SI VOULU, compiler l'editor et linker avec la librairie. Il
#    va etre place dans /bin/editor

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

# Execute immediatement le CMakeList.txt situe dans le dossier ./engine
# avant de continuer. Donc ca va compiler et linker la librairie du moteur
# physique et on va pouvoir la linker avec les tests et l'editor
add_subdirectory(MOCA)

# Maintenant que la librairie est compilee et ajoutee, il est possible de
# compiler le test ainsi que l'editor
add_subdirectory(test)
#add_subdirectory(editor)

message(STATUS "${BoldGreen}Please run 'make', 'make install' and optionally 'make clean'${ColorReset}")
