##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2019 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-sr.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Variables                                                                  |
##

set(GDCC_SR_H
   Arg.hpp
   Attribute.hpp
   Exp.hpp
   Exp/Arg.hpp
   Exp/Arith.hpp
   Exp/Binary.hpp
   Exp/Bitwise.hpp
   Exp/Cmp.hpp
   Exp/Cnd.hpp
   Exp/Convert.hpp
   Exp/Defer.hpp
   Exp/IRExp.hpp
   Exp/Logical.hpp
   Exp/MulSize.hpp
   Exp/Unary.hpp
   ExpCode.hpp
   Function.hpp
   GenStmnt.hpp
   GenStmnt/Move.hpp
   Object.hpp
   Space.hpp
   Statement.hpp
   Statement/Empty.hpp
   Statement/Exp.hpp
   Statement/Multi.hpp
   Storage.hpp
   Temporary.hpp
   Type.hpp
   Type/Array.hpp
   Type/Bitfield.hpp
   Type/Function.hpp
   Type/Pointer.hpp
   Type/Primary.hpp
   Type/Wrapper.hpp
   Types.hpp
   Warning.hpp
)


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-sr-lib
##
add_library(gdcc-sr-lib ${GDCC_SHARED_DECL}
   ${GDCC_SR_H}
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
   GenStmnt/Point.cpp
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
   Warning.cpp
)

target_link_libraries(gdcc-sr-lib gdcc-ir-lib)

GDCC_INSTALL_PART(sr SR SR FALSE TRUE)

## EOF

