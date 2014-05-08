##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2014 David Hill
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
   Number.cpp
   Option.cpp
   Origin.cpp
   ParseNumber.cpp
   ParseString.cpp
   Path.cpp
   String.cpp
   StringGen.cpp
)

target_link_libraries(gdcc-core-lib gdcc-option-lib gmpxx gmp)

## EOF

