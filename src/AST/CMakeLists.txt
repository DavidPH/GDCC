##-----------------------------------------------------------------------------
##
## Copyright (C) 2013 David Hill
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
   Exp/Binary.cpp
   Exp/BinaryMulSize.cpp
   Exp/ValueArg.cpp
   Exp/ValueIRExp.cpp
   Exp/genStmntMove.cpp
   Function.cpp
   Object.cpp
   Space.cpp
   Statement.cpp
   Statement/Empty.cpp
   Statement/Exp.cpp
   Statement/Multi.cpp
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

