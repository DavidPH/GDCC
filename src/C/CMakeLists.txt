##-----------------------------------------------------------------------------
##
## CMake file for GDCC::C executable.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_executable(gdcc-cc
   main.cpp

   ConditionDTBuf.cpp
   DirectiveTBuf.cpp
   IStream.cpp
   IncludeDTBuf.cpp
   Macro.cpp
   MacroDTBuf.cpp
   MacroTBuf.cpp
   Pragma.cpp
)

target_link_libraries(gdcc-cc gdcc-ir)

## EOF

