##-----------------------------------------------------------------------------
##
## Copyright (C) 2013 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-as.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_library(gdcc-as-lib
   IStream.cpp
   Macro.cpp
   Parse.cpp
   Parse/Arg.cpp
   Parse/Block.cpp
   Parse/Exp.cpp
   Parse/Function.cpp
   Parse/Import.cpp
   Parse/Object.cpp
   Parse/Space.cpp
   Parse/StrEnt.cpp
   Parse/Type.cpp
)

add_executable(gdcc-as
   main_as.cpp
)

target_link_libraries(gdcc-as-lib gdcc-ir-lib)
target_link_libraries(gdcc-as gdcc-as-lib)

## EOF

