##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2014 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-irdump.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-irdump
##
add_executable(gdcc-irdump
   main_irdump.cpp

   Put.cpp
   Put/Arg.cpp
   Put/Exp.cpp
   Put/Function.cpp
   Put/Program.cpp
   Put/Type.cpp
   Put/Value.cpp

   ../../inc/IRDump/Put.hpp
)

target_link_libraries(gdcc-irdump gdcc-ir-lib)

## EOF

