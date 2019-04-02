##-----------------------------------------------------------------------------
##
## Copyright (C) 2015-2019 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-acc.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Variables                                                                  |
##

set(GDCC_ACC_H
   DirectiveTBuf.hpp
   Factory.hpp
   IncludeDTBuf.hpp
   Macro.hpp
   MacroDTBuf.hpp
   PPTokenTBuf.hpp
   Parse.hpp
   Pragma.hpp
   PropDecl.hpp
   Scope.hpp
   TSource.hpp
   TStream.hpp
   Types.hpp
)


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-acc-lib
##
add_library(gdcc-acc-lib ${GDCC_SHARED_DECL}
   ${GDCC_ACC_H}
   DirectiveTBuf.cpp
   Exp.cpp
   IncludeDTBuf.cpp
   Macro.cpp
   MacroDTBuf.cpp
   PPTokenTBuf.cpp
   Parse.cpp
   Parse/Declaration.cpp
   Parse/Declaration/CreateTrans.cpp
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
   Parse/Exp/Unar/createtranslation.cpp
   Parse/Exp/Unar/print.cpp
   Parse/Statement.cpp
   Parse/TypeQual.cpp
   Parse/TypeSpec.cpp
   ParseFile.cpp
   Pragma.cpp
   PropDecl.cpp
   Scope.cpp
   TSource.cpp
)

##
## gdcc-acc
##
add_executable(gdcc-acc
   main_acc.cpp
)

target_link_libraries(gdcc-acc-lib
   gdcc-as-lib
   gdcc-cc-lib
   gdcc-cpp-lib
   gdcc-sr-lib
)

target_link_libraries(gdcc-acc gdcc-acc-lib gdcc-ld-lib)

GDCC_INSTALL_PART(acc ACC ACC TRUE TRUE)

## EOF

