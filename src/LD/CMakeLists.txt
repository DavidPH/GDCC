##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2018 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-ld.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-ld-lib
##
add_library(gdcc-ld-lib ${GDCC_SHARED_DECL}
   Linker.cpp

   ../../inc/LD/Linker.hpp
   ../../inc/LD/Types.hpp
)

target_link_libraries(gdcc-ld-lib gdcc-bc-lib)

if(GDCC_BC_DGE)
   target_link_libraries(gdcc-ld-lib gdcc-bc-dge-lib)
endif()

if(GDCC_BC_ZDACS)
   target_link_libraries(gdcc-ld-lib gdcc-bc-zdacs-lib)
endif()

##
## gdcc-ld
##
add_executable(gdcc-ld
   main_ld.cpp
)

target_link_libraries(gdcc-ld gdcc-ld-lib)

GDCC_INSTALL_PART(ld TRUE TRUE)

## EOF

