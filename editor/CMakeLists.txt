# editor CMakeList.txt ecrit par Marc-Andre Brochu
# Le 6 avril 2015
# Pour le projet MOCA, Cegep de l'Outaouais
# ====================================================

cmake_minimum_required(VERSION 3.1.0)

set(EDITOR_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/)
set(EDITOR_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/)

include_directories(${EDITOR_INC_DIR}
                    ${INC_OUTPUT})
link_directories(${LIB_OUTPUT})

set(SRCS)

if(SRCS)
    # Ajouter le path des sources aux sources en utilisant une macro,
    # comme ca on est pas oblige de le specifier dans la liste des
    # sources ci-haut
    add_prefix_to_list(SRCS ${EDITOR_SRC_DIR})
    add_executable(editor ${SRCS})
    add_dependencies(editor moca)

    # Linker l'editor avec MOCA et installer
    target_link_libraries(editor LINK_PUBLIC moca)
    install(TARGETS editor DESTINATION ${BIN_OUTPUT} OPTIONAL)
else()
    message(STATUS "${BoldYellow}Warning: no sources for target 'editor'${ColorReset}")
endif()
