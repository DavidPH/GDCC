##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2024 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-bc-zdacs.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Variables                                                                  |
##

set(GDCC_BC_ZDACS_H
   Info.hpp
   Code.hpp
   Types.hpp
)


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-bc-zdacs-lib
##
add_library(gdcc-bc-zdacs-lib ${GDCC_SHARED_DECL}
   ${GDCC_BC_ZDACS_H}
   Info.cpp
   Info/Stmnt/Add.cpp
   Info/Stmnt/Bit.cpp
   Info/Stmnt/Call.cpp
   Info/Stmnt/Cmp.cpp
   Info/Stmnt/Div.cpp
   Info/Stmnt/Jump.cpp
   Info/Stmnt/Log.cpp
   Info/Stmnt/Move.cpp
   Info/Stmnt/Mul.cpp
   Info/Stmnt/Neg.cpp
   Info/Stmnt/Sh.cpp
   Info/Stmnt/Tr.cpp
   Info/chk.cpp
   Info/gen.cpp
   Info/genIniti.cpp
   Info/genSpace.cpp
   Info/genStmnt.cpp
   Info/lenChunk.cpp
   Info/pre.cpp
   Info/put.cpp
   Info/putChunk.cpp
   Info/putExtra.cpp
   Info/tr.cpp
   Module.hpp
)

target_link_libraries(gdcc-bc-zdacs-lib gdcc-bc-lib)

GDCC_INSTALL_PART(bc-zdacs BC_ZDACS BC/ZDACS FALSE TRUE)

## EOF

