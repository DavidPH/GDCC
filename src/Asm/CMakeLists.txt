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
   ParseExp.cpp
   ParseFunction.cpp
)

target_link_libraries(gdcc-as gdcc-ir)

## EOF

