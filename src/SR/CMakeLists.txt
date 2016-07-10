##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2014 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-sr.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-sr-lib
##
add_library(gdcc-sr-lib ${GDCC_SHARED_DECL}
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

   ../../inc/SR/Arg.hpp
   ../../inc/SR/Attribute.hpp
   ../../inc/SR/Exp.hpp
   ../../inc/SR/Exp/Arg.hpp
   ../../inc/SR/Exp/Arith.hpp
   ../../inc/SR/Exp/Binary.hpp
   ../../inc/SR/Exp/Bitwise.hpp
   ../../inc/SR/Exp/Cmp.hpp
   ../../inc/SR/Exp/Cnd.hpp
   ../../inc/SR/Exp/Convert.hpp
   ../../inc/SR/Exp/Defer.hpp
   ../../inc/SR/Exp/IRExp.hpp
   ../../inc/SR/Exp/Logical.hpp
   ../../inc/SR/Exp/MulSize.hpp
   ../../inc/SR/Exp/Unary.hpp
   ../../inc/SR/ExpCode.hpp
   ../../inc/SR/Function.hpp
   ../../inc/SR/GenStmnt.hpp
   ../../inc/SR/GenStmnt/Move.hpp
   ../../inc/SR/Object.hpp
   ../../inc/SR/Space.hpp
   ../../inc/SR/Statement.hpp
   ../../inc/SR/Statement/Empty.hpp
   ../../inc/SR/Statement/Exp.hpp
   ../../inc/SR/Statement/Multi.hpp
   ../../inc/SR/Storage.hpp
   ../../inc/SR/Temporary.hpp
   ../../inc/SR/Type.hpp
   ../../inc/SR/Type/Array.hpp
   ../../inc/SR/Type/Bitfield.hpp
   ../../inc/SR/Type/Function.hpp
   ../../inc/SR/Type/Pointer.hpp
   ../../inc/SR/Type/Primary.hpp
   ../../inc/SR/Type/Wrapper.hpp
   ../../inc/SR/Warning.hpp
)

target_link_libraries(gdcc-sr-lib gdcc-ir-lib)

GDCC_INSTALL_PART(sr FALSE TRUE)

## EOF

