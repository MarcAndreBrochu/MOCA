# Test-level CMakeList.txt ecrit par Marc-Andre Brochu
# Le 1er avril 2015 (joyeux mois de l'autisme!)
# Pour le projet MOCA, Cegep de l'Outaouais
# ====================================================
# DMAJ: 6 avril 2015
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

if(SRCS)
    # Ajouter le path des sources aux sources en utilisant une macro,
    # comme ca on est pas oblige de le specifier dans la liste des
    # sources ci-haut
    add_prefix_to_list(SRCS ${TEST_SRC_DIR})
    add_executable(moca_test ${SRCS})
    add_dependencies(moca_test moca)

    # Linker le test avec MOCA et installer
    target_link_libraries(moca_test LINK_PUBLIC moca)
    install(TARGETS moca_test DESTINATION ${BIN_OUTPUT} OPTIONAL)
else()
    message(STATUS "${BoldYellow}Warning: no sources for target 'moca_test'${ColorReset}")
endif()
