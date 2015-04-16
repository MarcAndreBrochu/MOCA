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

# On veut que cmake puisse executer MOC quand il le faut automatiquement.
# De plus, on veut inclure Qt5 en tant que librairie pour le test et
# pour l'editeur
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

# Sans le flag -fPIE, les executables ne compilent pas
set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")

link_directories(${LIB_OUTPUT})
include_directories(
    ${TEST_INC_DIR}
    ${INC_OUTPUT})

set(SRCS
    main.cpp)

# Lister les headers uniquement pour que QtCreator les liste
set(HEADERS
    )

if(SRCS)
    # Ajouter le path des sources aux sources en utilisant une macro,
    # comme ca on est pas oblige de le specifier dans la liste des
    # sources ci-haut
    add_prefix_to_list(SRCS ${TEST_SRC_DIR})
    add_prefix_to_list(HEADERS ${TEST_INC_DIR})
    add_executable(moca_test ${SRCS} ${HEADERS})
    add_dependencies(moca_test moca)

    # Linker le test avec les librairies necessaires et installer
    target_link_libraries(moca_test LINK_PUBLIC Qt5::Core Qt5::Gui moca
                          ${Qt5Gui_EGL_LIBRARIES} ${Qt5Gui_OPENGL_LIBRARIES})

    install(TARGETS moca_test DESTINATION ${BIN_OUTPUT} OPTIONAL)
else()
    message(STATUS "${BoldYellow}Warning: no sources for target 'moca_test'${ColorReset}")
endif()
