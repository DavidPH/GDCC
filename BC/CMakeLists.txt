##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2019 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-bc.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Variables                                                                  |
##

set(GDCC_BC_H
   AddFunc.hpp
   Info.hpp
   Types.hpp
)


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

if(GDCC_BC_DGE)
   add_subdirectory(DGE)
endif()

if(GDCC_BC_ZDACS)
   add_subdirectory(ZDACS)
endif()

##
## gdcc-bc-lib
##
add_library(gdcc-bc-lib ${GDCC_SHARED_DECL}
   ${GDCC_BC_H}
   Info.cpp
   Info/Stmnt/Add.cpp
   Info/Stmnt/Bit.cpp
   Info/Stmnt/Cmp.cpp
   Info/Stmnt/Div.cpp
   Info/Stmnt/Mul.cpp
   Info/Stmnt/Neg.cpp
   Info/Stmnt/Sh.cpp
   Info/Stmnt/Tr.cpp
   Info/addFunc.cpp
   Info/chk.cpp
   Info/getWord.cpp
   Info/moveArg.cpp
   Info/optStmnt.cpp
   Info/put.cpp
   Info/trStmnt.cpp
)

target_link_libraries(gdcc-bc-lib gdcc-ir-lib)

GDCC_INSTALL_PART(bc BC BC FALSE TRUE)

## EOF

