##-----------------------------------------------------------------------------
##
## Copyright (C) 2013 David Hill
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

add_library(gdcc-bytecode-zdacs-lib
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
)

target_link_libraries(gdcc-bytecode-zdacs-lib gdcc-bytecode-lib)

## EOF

