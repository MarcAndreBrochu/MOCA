# PrefixList.cmake ecrit par Marc-Andre Brochu
# Le 1er avril 2015 (joyeux mois de l'autisme!)
# Pour le projet MOCA, Cegep de l'Outaouais
# ====================================================
#
# But: ajouter un prefix a tous les elements d'une liste

macro(add_prefix_to_list list_name prefix)

    # Creer une liste temporaire qui va contenir les
    # nouvelles valeurs
    set(${list_name}_tmp)

    foreach(l ${${list_name}})
        list(APPEND "${list_name}_tmp" ${prefix}${l})
    endforeach()

    # Remplacer la liste par liste_tmp
    set(${list_name} ${${list_name}_tmp})
    unset(${list_name}_tmp)

endmacro(add_prefix_to_list)
