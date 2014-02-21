##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2014 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-cpp.
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
   StringTBuf.cpp
)

add_executable(gdcc-cpp
   main_cpp.cpp
)

target_link_libraries(gdcc-cpp-lib gdcc-ir-lib)

target_link_libraries(gdcc-cpp gdcc-cpp-lib)

## EOF

