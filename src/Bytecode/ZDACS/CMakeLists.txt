##-----------------------------------------------------------------------------
##
## Copyright (C) 2013 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for Bytecode/ZDACS targets.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_library(gdcc-bc-zdacs
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

target_link_libraries(gdcc-bc-zdacs gdcc-bc)

## EOF

