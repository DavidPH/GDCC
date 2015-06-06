##-----------------------------------------------------------------------------
##
## Copyright (C) 2015 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-acc.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-acc-lib
##
add_library(gdcc-acc-lib ${GDCC_SHARED_DECL}
   DirectiveTBuf.cpp
   IncludeDTBuf.cpp
   IStream.cpp
   MacroDTBuf.cpp
   PPTokenTBuf.cpp
   Parse.cpp
   Parse/Declaration.cpp
   Parse/Declaration/Enum.cpp
   Parse/Declaration/Function.cpp
   Parse/Declaration/Object.cpp
   Parse/Declaration/Print.cpp
   Parse/Declaration/Special.cpp
   Parse/Declaration/Struct.cpp
   Parse/Declarator.cpp
   Parse/DeclSpec.cpp
   Parse/Exp.cpp
   Parse/Exp/Prim.cpp
   Parse/Exp/Unar/print.cpp
   Parse/Statement.cpp
   Parse/TypeQual.cpp
   Parse/TypeSpec.cpp
   Pragma.cpp
   Scope.cpp

   ../../inc/ACC/DirectiveTBuf.hpp
   ../../inc/ACC/IncludeDTBuf.hpp
   ../../inc/ACC/IStream.hpp
   ../../inc/ACC/MacroDTBuf.hpp
   ../../inc/ACC/PPTokenTBuf
   ../../inc/ACC/Parse.hpp
   ../../inc/ACC/Pragma.hpp
   ../../inc/ACC/Print.hpp
   ../../inc/ACC/Scope.hpp
   ../../inc/ACC/TStream.hpp
)

##
## gdcc-acc
##
add_executable(gdcc-acc
   main_acc.cpp
)

target_link_libraries(gdcc-acc-lib
   gdcc-as-lib
   gdcc-ast-lib
   gdcc-cc-lib
   gdcc-cpp-lib
)

target_link_libraries(gdcc-acc gdcc-acc-lib)

GDCC_INSTALL_PART(acc TRUE TRUE)

## EOF

