##-----------------------------------------------------------------------------
##
## Copyright (C) 2013 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-core.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_library(gdcc-core-lib
   Exception.cpp
   Option.cpp
   Origin.cpp
   ParseNumber.cpp
   ParseString.cpp
   Path.cpp
   String.cpp
)

target_link_libraries(gdcc-core-lib gdcc-option-lib gmpxx gmp)

## EOF

