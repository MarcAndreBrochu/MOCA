# Test-level CMakeList.txt ecrit par Marc-Andre Brochu
# Le 1er avril 2015 (joyeux mois de l'autisme!)
# Pour le projet MOCA, Cegep de l'Outaouais
# ====================================================
# DMAJ: 30 avril 2015
#
# On veut compiler le test en linkant avec la librairie MOCA
# qui a ete (on l'espere) compilee avant.

cmake_minimum_required(VERSION 3.1.0)

set(TEST_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/)
set(TEST_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/)
set(TEST_RES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources/)

# On veut que cmake puisse executer MOC quand il le faut automatiquement.
# De plus, on veut inclure Qt5 en tant que librairie pour le test et
# pour l'editeur
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Widgets)
find_package(OpenGL)

# Sans le flag -fPIE, les executables ne compilent pas
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")

link_directories(${LIB_OUTPUT})
include_directories(
    ${TEST_INC_DIR}
    ${INC_OUTPUT})

set(SRCS
    GLBox.cpp
    GLSphere.cpp
    GLWidget.cpp
    main.cpp)

# Lister les headers uniquement pour que QtCreator les liste
set(HEADERS
    GLDrawable.h
    GLBox.h
    GLSphere.h
    GLWidget.h)

if(SRCS)
    # Ajouter le path des sources aux sources en utilisant une macro,
    # comme ca on est pas oblige de le specifier dans la liste des
    # sources ci-haut
    add_prefix_to_list(SRCS ${TEST_SRC_DIR})
    add_prefix_to_list(HEADERS ${TEST_INC_DIR})
    add_prefix_to_list(MOCA_TEST_RESOURCES ${TEST_RES_DIR})

    qt5_add_resources(RESOURCE_ADDED ${CMAKE_CURRENT_SOURCE_DIR}/resources/qtres.qrc)

    add_executable(moca_test ${SRCS} ${RESOURCE_ADDED} ${HEADERS})
    add_dependencies(moca_test moca)

    qt5_use_modules(moca_test OpenGL)
    target_link_libraries(moca_test Qt5::Widgets moca)

#    add_custom_command(TARGET moca_test PRE_BUILD
#                       COMMAND ${CMAKE_COMMAND} -E copy
#                       ${CMAKE_CURRENT_SOURCE_DIR}/resources/scene.json ${CMAKE_SOURCE_DIR}/bin/scene.json)

    install(TARGETS moca_test DESTINATION ${BIN_OUTPUT} OPTIONAL)
else()
    message(STATUS "${BoldYellow}Warning: no sources for target 'moca_test'${ColorReset}")
endif()
