##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2014 David Hill
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

##
## gdcc-as-lib
##
add_library(gdcc-as-lib
   IStream.cpp
   Macro.cpp
   Parse.cpp
   Parse/Arg.cpp
   Parse/Block.cpp
   Parse/Declaration.cpp
   Parse/Exp.cpp
   Parse/Function.cpp
   Parse/Glyph.cpp
   Parse/Import.cpp
   Parse/Object.cpp
   Parse/Space.cpp
   Parse/StrEnt.cpp
   Parse/Type.cpp
   Parse/Value.cpp

   ../../inc/AS/IStream.hpp
   ../../inc/AS/Macro.hpp
   ../../inc/AS/Parse.hpp
   ../../inc/AS/TStream.hpp
)

##
## gdcc-as
##
add_executable(gdcc-as
   main_as.cpp
)

target_link_libraries(gdcc-as-lib gdcc-ir-lib)
target_link_libraries(gdcc-as gdcc-as-lib)

## EOF

