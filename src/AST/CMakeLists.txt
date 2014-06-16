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

add_library(gdcc-ast-lib
   Arg.cpp
   Attribute.cpp
   Exp.cpp
   Exp/Arg.cpp
   Exp/Arith.cpp
   Exp/Binary.cpp
   Exp/Bitwise.cpp
   Exp/Cmp.cpp
   Exp/Convert.cpp
   Exp/IRExp.cpp
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
)

target_link_libraries(gdcc-ast-lib gdcc-bytecode-lib gdcc-ir-lib)

## EOF

