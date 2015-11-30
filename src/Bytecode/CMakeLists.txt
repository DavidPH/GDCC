##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2015 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-bytecode.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_subdirectory(MgC)
add_subdirectory(ZDACS)

##
## gdcc-bytecode-lib
##
add_library(gdcc-bytecode-lib ${GDCC_SHARED_DECL}
   Info.cpp
   Info/moveArg.cpp
   Info/optStmnt.cpp

   ../../inc/Bytecode/Info.hpp
)

target_link_libraries(gdcc-bytecode-lib gdcc-ir-lib)

GDCC_INSTALL_PART(bytecode FALSE TRUE)

## EOF

