##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2016 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-bc-zdacs.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

if(GDCC_BC_ZDACS)
   ##
   ## gdcc-bc-zdacs-lib
   ##
   add_library(gdcc-bc-zdacs-lib ${GDCC_SHARED_DECL}
      Info.cpp
      Info/Stmnt/Add.cpp
      Info/Stmnt/Bitwise.cpp
      Info/Stmnt/Call.cpp
      Info/Stmnt/Cmp.cpp
      Info/Stmnt/Div.cpp
      Info/Stmnt/InvU.cpp
      Info/Stmnt/Jump.cpp
      Info/Stmnt/Move.cpp
      Info/Stmnt/Mul.cpp
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
      Info/putExtra.cpp
      Info/putIniti.cpp
      Info/putStmnt.cpp
      Info/tr.cpp
      Info/trStmnt.cpp

      ../../../inc/BC/ZDACS/Info.hpp
      ../../../inc/BC/ZDACS/Code.hpp
   )

   target_link_libraries(gdcc-bc-zdacs-lib gdcc-bc-lib)

   GDCC_INSTALL_PART(bc-zdacs FALSE TRUE)
endif()

## EOF

