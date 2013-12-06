##-----------------------------------------------------------------------------
##
## Copyright (C) 2013 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for Asm targets.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_executable(gdcc-as
   main_asm.cpp

   IStream.cpp
   Macro.cpp
   Parse.cpp
   ParseArg.cpp
   ParseBlock.cpp
   ParseExp.cpp
   ParseFunction.cpp
   ParseImport.cpp
   ParseObject.cpp
   ParseSpace.cpp
   ParseStrEnt.cpp
   ParseType.cpp
)

target_link_libraries(gdcc-as gdcc-ir)

## EOF

