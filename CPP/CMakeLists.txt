##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2022 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-cpp.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Variables                                                                  |
##

set(GDCC_CPP_H
   ConcatTBuf.hpp
   ConditionDTBuf.hpp
   DirectiveTBuf.hpp
   GetExp.hpp
   IncludeDTBuf.hpp
   IStream.hpp
   Macro.hpp
   MacroDTBuf.hpp
   MacroTBuf.hpp
   PPTokenTBuf.hpp
   Pragma.hpp
   PragmaDTBuf.hpp
   StringTBuf.hpp
   TSource.hpp
   TStream.hpp
   TrigraphBuf.hpp
   Types.hpp
   Warning.hpp
)


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-cpp-lib
##
add_library(gdcc-cpp-lib ${GDCC_SHARED_DECL}
   ${GDCC_CPP_H}
   ConcatTBuf.cpp
   ConditionDTBuf.cpp
   DirectiveTBuf.cpp
   GetExp.cpp
   IncludeDTBuf.cpp
   Macro.cpp
   MacroDTBuf.cpp
   MacroTBuf.cpp
   PPTokenTBuf.cpp
   Pragma.cpp
   PragmaDTBuf.cpp
   StringTBuf.cpp
   TSource.cpp
   Warning.cpp
)

##
## gdcc-cpp
##
add_executable(gdcc-cpp
   main_cpp.cpp
)

target_link_libraries(gdcc-cpp-lib gdcc-ir-lib)

target_link_libraries(gdcc-cpp gdcc-cpp-lib)

GDCC_INSTALL_PART(cpp CPP CPP TRUE TRUE)

## EOF

