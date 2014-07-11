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
)

target_link_libraries(gdcc-ir-lib gdcc-core-lib gdcc-bytecode-lib)

## EOF

