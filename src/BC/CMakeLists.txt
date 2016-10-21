##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2016 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-bc.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

if(EXISTS "${CMAKE_SOURCE_DIR}/src/BC/DGE")
   add_subdirectory(DGE)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/BC/MgC")
   add_subdirectory(MgC)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/BC/ZDACS")
   add_subdirectory(ZDACS)
endif()

##
## gdcc-bc-lib
##
add_library(gdcc-bc-lib ${GDCC_SHARED_DECL}
   Info.cpp
   Info/Stmnt/Add.cpp
   Info/Stmnt/Bit.cpp
   Info/Stmnt/Cmp.cpp
   Info/Stmnt/Div.cpp
   Info/Stmnt/Mul.cpp
   Info/Stmnt/Sh.cpp
   Info/addFunc.cpp
   Info/moveArg.cpp
   Info/optStmnt.cpp
   Info/put.cpp
   Info/trStmnt.cpp

   ../../inc/BC/AddFunc.hpp
   ../../inc/BC/Info.hpp
)

target_link_libraries(gdcc-bc-lib gdcc-ir-lib)

GDCC_INSTALL_PART(bc FALSE TRUE)

## EOF

