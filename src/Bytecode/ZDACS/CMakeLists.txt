##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2014 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-bytecode-zdacs.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-bytecode-zdacs-lib
##
add_library(gdcc-bytecode-zdacs-lib ${GDCC_SHARED_DECL}
   Info.cpp
   Info/Stmnt/Add.cpp
   Info/Stmnt/Bitwise.cpp
   Info/Stmnt/Cmp.cpp
   Info/Stmnt/Div.cpp
   Info/Stmnt/InvU.cpp
   Info/Stmnt/Move.cpp
   Info/Stmnt/NegI.cpp
   Info/Stmnt/Shift.cpp
   Info/Stmnt/Swap.cpp
   Info/gen.cpp
   Info/genIniti.cpp
   Info/genSpace.cpp
   Info/genStmnt.cpp
   Info/lenChunk.cpp
   Info/pre.cpp
   Info/put.cpp
   Info/putChunk.cpp
   Info/putIniti.cpp
   Info/putStmnt.cpp
   Info/tr.cpp
   Info/trStmnt.cpp

   ../../../inc/Bytecode/ZDACS/Info.hpp
   ../../../inc/Bytecode/ZDACS/Code.hpp
)

target_link_libraries(gdcc-bytecode-zdacs-lib gdcc-bytecode-lib)

GDCC_INSTALL_PART(bytecode-zdacs FALSE TRUE)

## EOF

