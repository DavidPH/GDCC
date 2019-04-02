##-----------------------------------------------------------------------------
##
## Copyright (C) 2016-2019 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-ar-wad.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Variables                                                                  |
##

set(GDCC_AR_Wad_H
   Lump.hpp
   LumpInfo.hpp
   Types.hpp
   Wad.hpp
   WadHeader.hpp
)


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-ar-wad-lib
##
add_library(gdcc-ar-wad-lib ${GDCC_SHARED_DECL}
   ${GDCC_AR_Wad_H}
   Lump.cpp
   LumpInfo.cpp
   Wad.cpp
   Wad/AddLump.cpp
   WadHeader.cpp

)

target_link_libraries(gdcc-ar-wad-lib gdcc-ar-lib)

##
## gdcc-ar-wad
##
add_executable(gdcc-ar-wad
   main_wad.cpp
)

target_link_libraries(gdcc-ar-wad gdcc-ar-wad-lib)

GDCC_INSTALL_PART(ar-wad AR_Wad AR/Wad TRUE TRUE)

## EOF

