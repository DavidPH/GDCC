##-----------------------------------------------------------------------------
##
## CMake file for GDCC-IR library.
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
   Exp/ValueRoot.cpp
   Function.cpp
   Glyph.cpp
   IArchive.cpp
   Import.cpp
   Linkage.cpp
   Object.cpp
   Statement.cpp
   StrEnt.cpp
   Type.cpp
   Value.cpp
   Value/Add.cpp
   Value/And.cpp
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

target_link_libraries(gdcc-ir gdcc gmpxx gmp)

## EOF

