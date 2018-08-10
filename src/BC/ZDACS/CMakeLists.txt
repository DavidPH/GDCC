##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2018 David Hill
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
      Info/Stmnt/Bit.cpp
      Info/Stmnt/Call.cpp
      Info/Stmnt/Cmp.cpp
      Info/Stmnt/Div.cpp
      Info/Stmnt/Jump.cpp
      Info/Stmnt/Move.cpp
      Info/Stmnt/Mul.cpp
      Info/Stmnt/Neg.cpp
      Info/Stmnt/Sh.cpp
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
      Info/putIniti.cpp
      Info/putStmnt.cpp
      Info/tr.cpp

      ../../../inc/BC/ZDACS/Info.hpp
      ../../../inc/BC/ZDACS/Code.hpp
      ../../../inc/BC/ZDACS/Types.hpp
   )

   target_link_libraries(gdcc-bc-zdacs-lib gdcc-bc-lib)

   GDCC_INSTALL_PART(bc-zdacs FALSE TRUE)
endif()

## EOF

