##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2014 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-bytecode-zdacs.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-bytecode-zdacs-lib
##
add_library(gdcc-bytecode-zdacs-lib ${GDCC_SHARED_DECL}
   Info.cpp
   gen.cpp
   genSpace.cpp
   genStmnt.cpp
   lenChunk.cpp
   pre.cpp
   put.cpp
   putChunk.cpp
   putStmnt.cpp
   tr.cpp
   trStmnt.cpp

   ../../../inc/Bytecode/ZDACS/Info.hpp
)

target_link_libraries(gdcc-bytecode-zdacs-lib gdcc-bytecode-lib)

## EOF

