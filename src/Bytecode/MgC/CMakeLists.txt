##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2014 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-bytecode-mgc.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-bytecode-mgc-lib
##
add_library(gdcc-bytecode-mgc-lib ${GDCC_SHARED_DECL}
   Info.cpp
   Info/put.cpp
   Info/tr.cpp

   ../../../inc/Bytecode/MgC/Info.hpp
)

target_link_libraries(gdcc-bytecode-mgc-lib gdcc-bytecode-lib)

GDCC_INSTALL_PART(bytecode-mgc FALSE TRUE)

## EOF

