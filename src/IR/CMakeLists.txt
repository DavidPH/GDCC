##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2014 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-ir.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-ir-lib
##
add_library(gdcc-ir-lib
   Addr.cpp
   Arg.cpp
   Block.cpp
   CallType.cpp
   Code.cpp
   Exp.cpp
   Exp/Binary.cpp
   Exp/Branch.cpp
   Exp/Glyph.cpp
   Exp/Multi.cpp
   Exp/Unary.cpp
   Exp/Value.cpp
   Function.cpp
   Glyph.cpp
   IArchive.cpp
   Import.cpp
   Linkage.cpp
   OArchive.cpp
   Object.cpp
   Program.cpp
   ScriptType.cpp
   Space.cpp
   Statement.cpp
   StrEnt.cpp
   Type.cpp
   Value.cpp
   Value/Add.cpp
   Value/And.cpp
   Value/Cast.cpp
   Value/Cmp.cpp
   Value/Div.cpp
   Value/Mod.cpp
   Value/Mul.cpp
   Value/OrI.cpp
   Value/OrX.cpp
   Value/ShL.cpp
   Value/ShR.cpp
   Value/Sub.cpp
   Value/Unary.cpp

   ../../inc/IR/Addr.hpp
   ../../inc/IR/AddrList.hpp
   ../../inc/IR/Arg.hpp
   ../../inc/IR/Block.hpp
   ../../inc/IR/CallType.hpp
   ../../inc/IR/Code.hpp
   ../../inc/IR/CodeList.hpp
   ../../inc/IR/CodeSet/Arith.hpp
   ../../inc/IR/CodeSet/Bitwise.hpp
   ../../inc/IR/CodeSet/Cmp.hpp
   ../../inc/IR/CodeSet/Unary.hpp
   ../../inc/IR/Exp.hpp
   ../../inc/IR/Exp/Binary.hpp
   ../../inc/IR/Exp/Branch.hpp
   ../../inc/IR/Exp/Glyph.hpp
   ../../inc/IR/Exp/Multi.hpp
   ../../inc/IR/Exp/Unary.hpp
   ../../inc/IR/Exp/Value.hpp
   ../../inc/IR/Function.hpp
   ../../inc/IR/Glyph.hpp
   ../../inc/IR/IArchive.hpp
   ../../inc/IR/Import.hpp
   ../../inc/IR/Linkage.hpp
   ../../inc/IR/OArchive.hpp
   ../../inc/IR/Object.hpp
   ../../inc/IR/Program.hpp
   ../../inc/IR/ScriptType.hpp
   ../../inc/IR/Space.hpp
   ../../inc/IR/Statement.hpp
   ../../inc/IR/StrEnt.hpp
   ../../inc/IR/Type.hpp
   ../../inc/IR/TypeList.hpp
   ../../inc/IR/Value.hpp
)

target_link_libraries(gdcc-ir-lib gdcc-core-lib gdcc-bytecode-lib)

## EOF

