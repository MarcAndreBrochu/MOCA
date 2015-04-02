# Test-level CMakeList.txt ecrit par Marc-Andre Brochu
# Le 1er avril 2015 (joyeux mois de l'autisme!)
# Pour le projet MOCA, Cegep de l'Outaouais
# ====================================================
#
# On veut compiler le test en linkant avec la librairie MOCA
# qui a ete (on l'espere) compilee avant.

cmake_minimum_required(VERSION 3.1.0)

set(TEST_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/)
set(TEST_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/)

include_directories(${TEST_INC_DIR}
                    ${INC_OUTPUT})
link_directories(${LIB_OUTPUT})

set(SRCS
    main.cpp)

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

# Ajouter le path des sources aux sources en utilisant une macro,
# comme ca on est pas oblige de le specifier dans la liste des
# sources ci-haut
add_prefix_to_list(SRCS ${TEST_SRC_DIR})
add_executable(moca_test ${SRCS})

# Linker le test avec MOCA et installer
target_link_libraries(moca_test LINK_PUBLIC moca)
install(TARGETS moca_test DESTINATION ${BIN_OUTPUT})
