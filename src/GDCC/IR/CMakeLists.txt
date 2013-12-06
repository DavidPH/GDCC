##-----------------------------------------------------------------------------
##
## Copyright (C) 2013 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for GDCC/IR targets.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_library(gdcc-ir
   Addr.cpp
   Arg.cpp
   Block.cpp
   Code.cpp
   Exp.cpp
   Exp/Binary.cpp
   Exp/Branch.cpp
   Exp/Unary.cpp
   Exp/ValueGlyph.cpp
   Exp/ValueMulti.cpp
   Exp/ValueRoot.cpp
   Function.cpp
   Glyph.cpp
   IArchive.cpp
   Import.cpp
   Linkage.cpp
   Object.cpp
   Program.cpp
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

target_link_libraries(gdcc-ir gdcc)

## EOF

