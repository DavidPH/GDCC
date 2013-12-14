##-----------------------------------------------------------------------------
##
## Copyright (C) 2013 David Hill
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

add_library(gdcc-bytecode-lib
   Info.cpp
   Platform.cpp
)

target_link_libraries(gdcc-bytecode-lib gdcc-ir-lib)

## EOF

