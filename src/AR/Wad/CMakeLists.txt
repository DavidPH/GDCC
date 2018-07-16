##-----------------------------------------------------------------------------
##
## Copyright (C) 2016 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-ar-wad.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-ar-wad-lib
##
add_library(gdcc-ar-wad-lib ${GDCC_SHARED_DECL}
   Lump.cpp
   LumpInfo.cpp
   Wad.cpp
   Wad/AddLump.cpp
   WadHeader.cpp

   ../../../inc/AR/Wad/Lump.hpp
   ../../../inc/AR/Wad/LumpInfo.hpp
   ../../../inc/AR/Wad/Types.hpp
   ../../../inc/AR/Wad/Wad.hpp
   ../../../inc/AR/Wad/WadHeader.hpp
)

target_link_libraries(gdcc-ar-wad-lib gdcc-ar-lib)

##
## gdcc-ar-wad
##
add_executable(gdcc-ar-wad
   main_wad.cpp
)

target_link_libraries(gdcc-ar-wad gdcc-ar-wad-lib)

GDCC_INSTALL_PART(ar-wad TRUE TRUE)

## EOF

