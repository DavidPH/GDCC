##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2016 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-bc.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_subdirectory(MgC)
add_subdirectory(ZDACS)

##
## gdcc-bc-lib
##
add_library(gdcc-bc-lib ${GDCC_SHARED_DECL}
   Info.cpp
   Info/moveArg.cpp
   Info/optStmnt.cpp

   ../../inc/BC/Info.hpp
)

target_link_libraries(gdcc-bc-lib gdcc-ir-lib)

GDCC_INSTALL_PART(bc FALSE TRUE)

## EOF

