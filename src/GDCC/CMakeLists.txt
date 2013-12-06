##-----------------------------------------------------------------------------
##
## Copyright (C) 2013 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for GDCC targets.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_subdirectory(AST)
add_subdirectory(IR)

add_library(gdcc
   Option.cpp
   Origin.cpp
   ParseNumber.cpp
   ParseString.cpp
   Path.cpp
   String.cpp
)

target_link_libraries(gdcc gdcc-option gmpxx gmp)

## EOF

