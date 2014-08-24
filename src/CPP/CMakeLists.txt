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

##
## gdcc-cpp-lib
##
add_library(gdcc-cpp-lib ${GDCC_SHARED_DECL}
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

   ../../inc/CPP/ConcatTBuf.hpp
   ../../inc/CPP/ConditionDTBuf.hpp
   ../../inc/CPP/DirectiveTBuf.hpp
   ../../inc/CPP/GetExpIR.hpp
   ../../inc/CPP/IncludeDTBuf.hpp
   ../../inc/CPP/IStream.hpp
   ../../inc/CPP/Macro.hpp
   ../../inc/CPP/MacroDTBuf.hpp
   ../../inc/CPP/MacroTBuf.hpp
   ../../inc/CPP/PPTokenTBuf.hpp
   ../../inc/CPP/Pragma.hpp
   ../../inc/CPP/PragmaDTBuf.hpp
   ../../inc/CPP/StringTBuf.hpp
   ../../inc/CPP/TrigraphBuf.hpp
   ../../inc/CPP/TStream.hpp
)

##
## gdcc-cpp
##
add_executable(gdcc-cpp
   main_cpp.cpp
)

target_link_libraries(gdcc-cpp-lib gdcc-ir-lib)

target_link_libraries(gdcc-cpp gdcc-cpp-lib)

GDCC_INSTALL_PART(cpp TRUE TRUE)

## EOF

