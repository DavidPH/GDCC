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
   ParseObject.cpp
)

target_link_libraries(gdcc-as gdcc-ir)

## EOF

