##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2019 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-ir.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Variables                                                                  |
##

set(GDCC_IR_H
   Arg.hpp
   Block.hpp
   Code.hpp
   CodeList.hpp
   DJump.hpp
   Exception.hpp
   Exp.hpp
   Exp/Binary.hpp
   Exp/Branch.hpp
   Exp/Glyph.hpp
   Exp/Multi.hpp
   Exp/Unary.hpp
   Exp/Value.hpp
   Function.hpp
   Glyph.hpp
   IArchive.hpp
   Import.hpp
   Linkage.hpp
   OArchive.hpp
   Object.hpp
   Program.hpp
   Space.hpp
   Statement.hpp
   StrEnt.hpp
   Type.hpp
   TypeList.hpp
   Types.hpp
   Value.hpp
)


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-ir-lib
##
add_library(gdcc-ir-lib ${GDCC_SHARED_DECL}
   ${GDCC_IR_H}
   Arg.cpp
   Block.cpp
   Code.cpp
   DJump.cpp
   Exception.cpp
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

target_link_libraries(gdcc-ir-lib gdcc-target-lib)

GDCC_INSTALL_PART(ir IR IR FALSE TRUE)

## EOF

