##-----------------------------------------------------------------------------
##
## Copyright (C) 2016-2018 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-bc-dge.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

if(GDCC_BC_DGE)
   ##
   ## gdcc-bc-dge-lib
   ##
   add_library(gdcc-bc-dge-lib ${GDCC_SHARED_DECL}
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
      Info/chk.cpp
      Info/gen.cpp
      Info/pre.cpp
      Info/put.cpp
      Info/putStmnt.cpp
      Info/tr.cpp

      ../../../inc/BC/DGE/Info.hpp
      ../../../inc/BC/DGE/Types.hpp
   )

   target_link_libraries(gdcc-bc-dge-lib gdcc-bc-lib)

   GDCC_INSTALL_PART(bc-dge FALSE TRUE)
endif()

## EOF

