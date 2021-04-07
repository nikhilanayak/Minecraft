if(TARGET raylib::raylib AND NOT TARGET raylib)
    add_library(raylib INTERFACE IMPORTED)
    set_property(TARGET raylib PROPERTY INTERFACE_LINK_LIBRARIES raylib::raylib)
endif()
