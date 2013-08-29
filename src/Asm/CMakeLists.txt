##-----------------------------------------------------------------------------
##
## CMake file for Asm executable.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_executable(gdcc-as
   main.cpp

   IStream.cpp
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

