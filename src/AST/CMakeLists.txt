##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2014 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-ast.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-ast-lib
##
add_library(gdcc-ast-lib ${GDCC_SHARED_DECL}
   Arg.cpp
   Attribute.cpp
   Exp.cpp
   Exp/Arg.cpp
   Exp/Arith.cpp
   Exp/Binary.cpp
   Exp/Bitwise.cpp
   Exp/Cmp.cpp
   Exp/Cnd.cpp
   Exp/Convert.cpp
   Exp/Defer.cpp
   Exp/IRExp.cpp
   Exp/Logical.cpp
   Exp/MulSize.cpp
   Exp/Unary.cpp
   Function.cpp
   GenStmnt/ArithEq.cpp
   GenStmnt/Convert.cpp
   GenStmnt/Move.cpp
   Object.cpp
   Space.cpp
   Statement.cpp
   Statement/Empty.cpp
   Statement/Exp.cpp
   Statement/Multi.cpp
   Temporary.cpp
   Type.cpp
   Type/Array.cpp
   Type/Bitfield.cpp
   Type/Function.cpp
   Type/Pointer.cpp
   Type/Primary.cpp
   Type/Wrapper.cpp

   ../../inc/AST/Arg.hpp
   ../../inc/AST/Attribute.hpp
   ../../inc/AST/Exp.hpp
   ../../inc/AST/Exp/Arg.hpp
   ../../inc/AST/Exp/Arith.hpp
   ../../inc/AST/Exp/Binary.hpp
   ../../inc/AST/Exp/Bitwise.hpp
   ../../inc/AST/Exp/Cmp.hpp
   ../../inc/AST/Exp/Cnd.hpp
   ../../inc/AST/Exp/Convert.hpp
   ../../inc/AST/Exp/Defer.hpp
   ../../inc/AST/Exp/IRExp.hpp
   ../../inc/AST/Exp/Logical.hpp
   ../../inc/AST/Exp/MulSize.hpp
   ../../inc/AST/Exp/Unary.hpp
   ../../inc/AST/ExpCode.hpp
   ../../inc/AST/Function.hpp
   ../../inc/AST/GenStmnt.hpp
   ../../inc/AST/GenStmnt/Move.hpp
   ../../inc/AST/Object.hpp
   ../../inc/AST/Space.hpp
   ../../inc/AST/Statement.hpp
   ../../inc/AST/Statement/Empty.hpp
   ../../inc/AST/Statement/Exp.hpp
   ../../inc/AST/Statement/Multi.hpp
   ../../inc/AST/Storage.hpp
   ../../inc/AST/Temporary.hpp
   ../../inc/AST/Type.hpp
   ../../inc/AST/Type/Array.hpp
   ../../inc/AST/Type/Bitfield.hpp
   ../../inc/AST/Type/Function.hpp
   ../../inc/AST/Type/Pointer.hpp
   ../../inc/AST/Type/Primary.hpp
   ../../inc/AST/Type/Wrapper.hpp
)

target_link_libraries(gdcc-ast-lib gdcc-ir-lib)

## EOF

