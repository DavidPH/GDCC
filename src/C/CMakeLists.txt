##-----------------------------------------------------------------------------
##
## Copyright (C) 2013 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for C targets.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_library(gdcc-cpp-lib
   ConcatTBuf.cpp
   ConditionDTBuf.cpp
   DirectiveTBuf.cpp
   GetExpIR.cpp
   IStream.cpp
   IncludeDTBuf.cpp
   Macro.cpp
   MacroDTBuf.cpp
   MacroTBuf.cpp
   PPTokenTBuf.cpp
   Pragma.cpp
   PragmaDTBuf.cpp
)

add_executable(gdcc-cc
   main_cc.cpp
)

add_executable(gdcc-cpp
   main_cpp.cpp
)

target_link_libraries(gdcc-cpp-lib gdcc-ir)

target_link_libraries(gdcc-cc gdcc-cpp-lib)

target_link_libraries(gdcc-cpp gdcc-cpp-lib)

## EOF

