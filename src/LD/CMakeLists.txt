##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2015 David Hill
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
)

target_link_libraries(gdcc-ld-lib gdcc-bytecode-lib)

if(GDCC_Bytecode_MgC)
   target_link_libraries(gdcc-ld-lib gdcc-bytecode-mgc-lib)
endif()

if(GDCC_Bytecode_ZDACS)
   target_link_libraries(gdcc-ld-lib gdcc-bytecode-zdacs-lib)
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

