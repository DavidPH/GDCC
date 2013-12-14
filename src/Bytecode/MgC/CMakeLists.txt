##-----------------------------------------------------------------------------
##
## Copyright (C) 2013 David Hill
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

add_library(gdcc-bytecode-mgc-lib
   Info.cpp
   put.cpp
   tr.cpp
)

target_link_libraries(gdcc-bytecode-mgc-lib gdcc-bytecode-lib)

## EOF

