##-----------------------------------------------------------------------------
##
## Copyright (C) 2016 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-ar.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_subdirectory(Wad)

##
## gdcc-ar-lib
##
add_library(gdcc-ar-lib ${GDCC_SHARED_DECL}
   BinaryIO.cpp
   Exception.cpp

   ../../inc/AR/BinaryIO.hpp
   ../../inc/AR/Exception.hpp
)

target_link_libraries(gdcc-ar-lib gdcc-core-lib)

GDCC_INSTALL_PART(ar FALSE TRUE)

## EOF

