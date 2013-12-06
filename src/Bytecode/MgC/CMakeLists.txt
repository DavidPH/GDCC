##-----------------------------------------------------------------------------
##
## Copyright (C) 2013 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for Bytecode/MgC targets.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_library(gdcc-bc-mgc
   Info.cpp
   put.cpp
   tr.cpp
)

target_link_libraries(gdcc-bc-mgc gdcc-bc)

## EOF

