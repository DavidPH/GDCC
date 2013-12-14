##-----------------------------------------------------------------------------
##
## Copyright (C) 2013 David Hill
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

add_executable(gdcc-ld
   main_ld.cpp
)

target_link_libraries(gdcc-ld gdcc-ir-lib
   gdcc-bytecode-mgc-lib
   gdcc-bytecode-zdacs-lib
)

## EOF

